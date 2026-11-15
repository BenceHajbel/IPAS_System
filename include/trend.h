// Pressure trend from the in-memory history. A heuristic indicator, NOT a forecast.
#pragma once
#include "history.h"

namespace ipas {

enum class TrendState : uint8_t { Insufficient, Steady, Rising, Falling, RisingFast, FallingFast };

constexpr uint32_t kTrendWindowS = 3 * 3600;   // look at the last 3 h
constexpr uint32_t kTrendMinSpanS = 3600;      // need at least 1 h of data
constexpr uint32_t kTrendMinPoints = 6;
constexpr float kTrendSteadyBandHpa = 1.0f;    // |change per 3 h| below this = steady
constexpr float kTrendFastBandHpa = 3.0f;      // at or above this = rapid

struct Trend {
  TrendState state = TrendState::Insufficient;
  float delta_hpa_3h = 0.f;  // least-squares slope scaled to 3 h
  uint32_t span_s = 0;       // time covered by the data actually used
  uint32_t points = 0;
};

inline const char* trendName(TrendState s) {
  switch (s) {
    case TrendState::Steady: return "steady";
    case TrendState::Rising: return "rising";
    case TrendState::Falling: return "falling";
    case TrendState::RisingFast: return "rising_fast";
    case TrendState::FallingFast: return "falling_fast";
    default: return "insufficient";
  }
}

inline Trend computeTrend(const History& h, uint32_t now_s) {
  Trend tr;
  const uint32_t from = now_s > kTrendWindowS ? now_s - kTrendWindowS : 0;
  double st = 0, sp = 0;  // sums for regression (t in hours since first point)
  double t0 = -1;
  // two passes: collect up to kFineCount + 1 points
  static_assert(kFineCount + 1 <= 1024, "buffer");
  static double ts[kFineCount + 1], ps[kFineCount + 1];  // static: keep the task stack small (single-threaded use)
  size_t n = 0;
  const auto& fine = h.fine();
  for (size_t i = 0; i < fine.size(); ++i) {
    const Sample& s = fine.at(i);
    if (s.t < from || s.t > now_s) continue;
    ts[n] = s.t; ps[n] = s.press_pa / 100.0; ++n;
  }
  Sample part;
  if (h.finePartial(&part) && part.t >= from && part.t <= now_s && (n == 0 || part.t > ts[n - 1])) {
    ts[n] = part.t; ps[n] = part.press_pa / 100.0; ++n;
  }
  tr.points = static_cast<uint32_t>(n);
  if (n == 0) return tr;
  tr.span_s = static_cast<uint32_t>(ts[n - 1] - ts[0]);
  if (n < kTrendMinPoints || tr.span_s < kTrendMinSpanS) return tr;

  t0 = ts[0];
  double mt = 0, mp = 0;
  for (size_t i = 0; i < n; ++i) { mt += (ts[i] - t0); mp += ps[i]; }
  mt /= n; mp /= n;
  for (size_t i = 0; i < n; ++i) {
    const double dt = (ts[i] - t0) - mt;
    st += dt * dt;
    sp += dt * (ps[i] - mp);
  }
  if (st <= 0) return tr;
  const double slope_per_s = sp / st;
  tr.delta_hpa_3h = static_cast<float>(slope_per_s * 10800.0);
  const float a = fabsf(tr.delta_hpa_3h);
  if (a < kTrendSteadyBandHpa) tr.state = TrendState::Steady;
  else if (tr.delta_hpa_3h > 0) tr.state = a >= kTrendFastBandHpa ? TrendState::RisingFast : TrendState::Rising;
  else tr.state = a >= kTrendFastBandHpa ? TrendState::FallingFast : TrendState::Falling;
  return tr;
}

}  // namespace ipas
