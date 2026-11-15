// Relative air-quality rating from BME680 gas resistance (+ humidity).
// This is NOT Bosch's BSEC "IAQ" (0-500), which needs Bosch's closed-source library.
// Method: widely used open example approach (Pimoroni bme680-python "indoor-air-quality"):
//   score(0..100) = gas part (max 75) + humidity part (max 25)
//   gas part      = 75 * min(1, gas / baseline)       baseline = clean-air reference
//   humidity part = 25 * (1 at 40 %RH, falling linearly to 0 at 0 %RH and at 100 %RH)
// Differences from the example: the baseline is the 95th percentile of the stored
// 30-minute gas averages (up to 7 days) instead of a 5-minute burn-in, so it keeps
// adapting; and it needs >= 6 h of data before a rating is shown.
// The five class limits below are this project's own choice: no standard defines
// categories for this score. With ideal humidity (score = 25 + 75 * gas/baseline) they
// correspond to gas/baseline of >= 87 %, >= 67 %, >= 40 %, >= 20 % and < 20 %.
#pragma once
#include <algorithm>
#include "history.h"

namespace ipas {

enum class AqState : uint8_t { Unavailable, Calibrating, Ok };
enum class AqLevel : uint8_t { VeryBad, Bad, Medium, Good, VeryGood };

constexpr uint32_t kAqMinBaselinePoints = 12;  // 12 x 30 min = 6 h
constexpr float kAqBaselinePercentile = 0.95f;
constexpr float kAqHumBaseline = 40.0f;        // %RH treated as ideal
constexpr float kAqHumWeight = 0.25f;          // humidity share of the score
constexpr float kAqVeryGoodMin = 90.0f;
constexpr float kAqGoodMin = 75.0f;
constexpr float kAqMediumMin = 55.0f;
constexpr float kAqBadMin = 40.0f;             // below this: very bad

struct AirQuality {
  AqState state = AqState::Unavailable;
  AqLevel level = AqLevel::Medium;
  float score = 0.f;
  float baseline_ohm = 0.f;
  uint32_t points = 0;  // gas samples available for the baseline
};

inline const char* aqStateName(AqState s) {
  return s == AqState::Ok ? "ok" : s == AqState::Calibrating ? "calibrating" : "unavailable";
}
inline const char* aqLevelName(AqLevel l) {
  switch (l) {
    case AqLevel::VeryBad: return "very_bad";
    case AqLevel::Bad: return "bad";
    case AqLevel::Medium: return "medium";
    case AqLevel::Good: return "good";
    default: return "very_good";
  }
}

inline AqLevel aqLevelFromScore(float score) {
  if (score >= kAqVeryGoodMin) return AqLevel::VeryGood;
  if (score >= kAqGoodMin) return AqLevel::Good;
  if (score >= kAqMediumMin) return AqLevel::Medium;
  if (score >= kAqBadMin) return AqLevel::Bad;
  return AqLevel::VeryBad;
}

inline float aqScore(float gas_ohm, float baseline_ohm, float hum_pct) {
  const float gas_part = (gas_ohm >= baseline_ohm ? 1.0f : gas_ohm / baseline_ohm) * (100.0f * (1.0f - kAqHumWeight));
  const float hum_off = hum_pct - kAqHumBaseline;
  const float hum_part = hum_off > 0 ? (100.0f - kAqHumBaseline - hum_off) / (100.0f - kAqHumBaseline) * (100.0f * kAqHumWeight)
                                      : (kAqHumBaseline + hum_off) / kAqHumBaseline * (100.0f * kAqHumWeight);
  float s = gas_part + hum_part;
  return s < 0 ? 0 : (s > 100 ? 100 : s);
}

inline AirQuality computeAirQuality(const History& h, bool has_latest, const Reading& latest, uint32_t now_s) {
  AirQuality aq;
  static float v[kCoarseCount + 1];  // static: keeps the task stack small (single-threaded use)
  size_t n = 0;
  const auto& c = h.coarse();
  for (size_t i = 0; i < c.size(); ++i)
    if (c.at(i).gas_ohm > 0 && c.at(i).t <= now_s) v[n++] = static_cast<float>(c.at(i).gas_ohm);
  Sample p;
  if (h.coarsePartial(&p) && p.gas_ohm > 0 && p.t <= now_s) v[n++] = static_cast<float>(p.gas_ohm);
  aq.points = static_cast<uint32_t>(n);
  if (!has_latest || !isfinite(latest.gas_ohm) || latest.gas_ohm <= 0) return aq;  // Unavailable
  if (n < kAqMinBaselinePoints) { aq.state = AqState::Calibrating; return aq; }
  std::sort(v, v + n);
  aq.baseline_ohm = v[static_cast<size_t>(kAqBaselinePercentile * (n - 1))];
  aq.score = aqScore(latest.gas_ohm, aq.baseline_ohm, latest.humidity_pct);
  aq.level = aqLevelFromScore(aq.score);
  aq.state = AqState::Ok;
  return aq;
}

}  // namespace ipas
