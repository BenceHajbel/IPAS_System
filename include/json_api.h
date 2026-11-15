// JSON builders for the HTTP API. Pure C++ (std::string), unit-tested on the host.
#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include "history.h"
#include "trend.h"
#include "air_quality.h"
#include "log_format.h"

namespace ipas {

constexpr int kApiVersion = 2;  // the dashboard in data/ refuses to run against another version

enum class SensorState : uint8_t { InitFailed, NoData, Ok, ReadFailed };

inline const char* sensorStateName(SensorState s) {
  switch (s) {
    case SensorState::Ok: return "ok";
    case SensorState::NoData: return "no_data";
    case SensorState::ReadFailed: return "read_failed";
    default: return "init_failed";
  }
}

enum class StorageState : uint8_t { Disabled, NoCard, Ok, Error };
inline const char* storageStateName(StorageState s) {
  switch (s) {
    case StorageState::Ok: return "ok";
    case StorageState::NoCard: return "no_card";
    case StorageState::Error: return "error";
    default: return "disabled";
  }
}
struct StorageInfo {
  StorageState state = StorageState::Disabled;
  uint32_t boot = 0;
  uint32_t loaded_samples = 0;
  uint32_t discarded_unsynced = 0;
  uint32_t file_bytes = 0;
  uint32_t write_errors = 0;
  uint32_t mount_attempts = 0;   // tries needed in this boot
  uint32_t restarts = 0;         // automatic restarts done because the card was not found
};

struct DeviceSnapshot {
  uint32_t uptime_s = 0;
  SensorState sensor = SensorState::InitFailed;
  uint8_t i2c_addr = 0;          // 0 = not found
  int32_t last_ok_age_s = -1;    // -1 = no valid reading yet
  uint32_t read_errors = 0;
  uint32_t reads_ok = 0;
  bool has_latest = false;
  Reading latest = {0, 0, 0, 0};
  uint32_t heap_free = 0;
  int stations = 0;
  uint32_t stale_after_s = 15;
  bool time_synced = false;   // browser has set the clock (timeline = Unix time)
  uint32_t now_s = 0;         // timeline "now" (uptime until synced, Unix time afterwards)
  StorageInfo storage;
};

inline void appendf(std::string& out, const char* fmt, ...) {
  char buf[160];
  va_list ap;
  va_start(ap, fmt);
  int n = vsnprintf(buf, sizeof buf, fmt, ap);
  va_end(ap);
  if (n > 0) out.append(buf, n < (int)sizeof buf ? n : (int)sizeof buf - 1);
}

inline void appendNum(std::string& out, double v, int decimals) {
  if (!isfinite(v)) { out += "null"; return; }
  appendf(out, "%.*f", decimals, v);
}

inline std::string statusJson(const DeviceSnapshot& d, const History& h, const Trend& tr, const AirQuality& aq) {
  std::string o;
  o.reserve(512);
  appendf(o, "{\"api\":%d,\"uptime_s\":%u,\"stale_after_s\":%u,", kApiVersion, (unsigned)d.uptime_s, (unsigned)d.stale_after_s);
  appendf(o, "\"sensor\":{\"state\":\"%s\",\"i2c_addr\":", sensorStateName(d.sensor));
  if (d.i2c_addr) appendf(o, "\"0x%02X\"", d.i2c_addr); else o += "null";
  o += ",\"last_ok_age_s\":";
  if (d.last_ok_age_s >= 0) appendf(o, "%d", (int)d.last_ok_age_s); else o += "null";
  appendf(o, ",\"reads_ok\":%u,\"read_errors\":%u},", (unsigned)d.reads_ok, (unsigned)d.read_errors);
  o += "\"latest\":";
  if (d.has_latest) {
    o += "{\"temperature_c\":"; appendNum(o, d.latest.temperature_c, 2);
    o += ",\"humidity_pct\":";  appendNum(o, d.latest.humidity_pct, 2);
    o += ",\"pressure_hpa\":";  appendNum(o, d.latest.pressure_hpa, 2);
    o += ",\"gas_ohm\":";
    if (isfinite(d.latest.gas_ohm) && d.latest.gas_ohm > 0) appendf(o, "%.0f", d.latest.gas_ohm); else o += "null";
    o += "}";
  } else {
    o += "null";
  }
  appendf(o, ",\"pressure_trend\":{\"state\":\"%s\",\"delta_hpa_3h\":", trendName(tr.state));
  if (tr.state == TrendState::Insufficient) o += "null"; else appendNum(o, tr.delta_hpa_3h, 2);
  appendf(o, ",\"span_s\":%u,\"points\":%u},", (unsigned)tr.span_s, (unsigned)tr.points);
  appendf(o, "\"air_quality\":{\"state\":\"%s\",\"level\":", aqStateName(aq.state));
  if (aq.state == AqState::Ok) appendf(o, "\"%s\",\"score\":%.1f,\"baseline_ohm\":%.0f", aqLevelName(aq.level), aq.score, aq.baseline_ohm);
  else o += "null,\"score\":null,\"baseline_ohm\":null";
  appendf(o, ",\"calibration_points\":%u,\"calibration_needed\":%u},", (unsigned)aq.points, (unsigned)kAqMinBaselinePoints);
  appendf(o, "\"time\":{\"synced\":%s,\"now_s\":%u},", d.time_synced ? "true" : "false", (unsigned)d.now_s);
  appendf(o, "\"storage\":{\"state\":\"%s\",\"boot\":%u,\"loaded_samples\":%u,\"discarded_unsynced\":%u,\"file_bytes\":%u,\"write_errors\":%u,\"mount_attempts\":%u,\"restarts\":%u},",
          storageStateName(d.storage.state), (unsigned)d.storage.boot, (unsigned)d.storage.loaded_samples,
          (unsigned)d.storage.discarded_unsynced, (unsigned)d.storage.file_bytes, (unsigned)d.storage.write_errors,
          (unsigned)d.storage.mount_attempts, (unsigned)d.storage.restarts);
  appendf(o, "\"history\":{\"minute_count\":%u,\"fine_count\":%u,\"fine_capacity\":%u,\"coarse_count\":%u,\"coarse_capacity\":%u},",
          (unsigned)h.minute().size(), (unsigned)h.fine().size(), (unsigned)kFineCount, (unsigned)h.coarse().size(), (unsigned)kCoarseCount);
  appendf(o, "\"system\":{\"heap_free\":%u,\"stations\":%d}}", (unsigned)d.heap_free, d.stations);
  return o;
}

enum class HistoryRange : uint8_t { Hour, HalfDay, Day, Week };

inline const char* rangeName(HistoryRange r) {
  switch (r) {
    case HistoryRange::Hour: return "1h";
    case HistoryRange::HalfDay: return "12h";
    case HistoryRange::Week: return "7d";
    default: return "24h";
  }
}

inline void appendPoint(std::string& o, const Sample& s, bool first) {
  if (!first) o += ',';
  appendf(o, "[%u,%.2f,%.2f,%.2f,", (unsigned)s.t, s.temp_c100 / 100.0, s.hum_c100 / 100.0, s.press_pa / 100.0);
  if (s.gas_ohm) appendf(o, "%u]", (unsigned)s.gas_ohm); else o += "null]";
}

// points: [timeline_s, temperature_c, humidity_pct, pressure_hpa, gas_ohm|null]
// now_s is the timeline "now" (see History); the browser places a point at
// (time of response) - (now_s - t). The final point may be the still incomplete interval.
// While the clock is not set (synced == false) only data of the current boot is returned.
inline std::string historyJson(const History& h, HistoryRange range, uint32_t now_s, bool synced) {
  uint32_t window = 24u * 3600u, interval = kFineIntervalS;
  switch (range) {
    case HistoryRange::Hour: window = 3600u; interval = kMinuteIntervalS; break;
    case HistoryRange::HalfDay: window = 12u * 3600u; break;
    case HistoryRange::Week: window = 7u * 24u * 3600u; interval = kCoarseIntervalS; break;
    default: break;
  }
  const uint32_t from = now_s > window ? now_s - window : 0;
  std::string o;
  o.reserve(range == HistoryRange::Week ? 16000 : 12000);
  appendf(o, "{\"api\":%d,\"range\":\"%s\",\"now_s\":%u,\"time_synced\":%s,\"interval_s\":%u,\"points\":[", kApiVersion,
          rangeName(range), (unsigned)now_s, synced ? "true" : "false", (unsigned)interval);
  bool first = true;
  uint32_t last_t = 0;
  auto emit = [&](const Sample& s) {
    if (s.t < from || s.t > now_s) return;
    if (!synced && s.t >= kEpochMin) return;
    if (!first && s.t <= last_t) return;
    appendPoint(o, s, first);
    first = false;
    last_t = s.t;
  };
  Sample p;
  bool has_p = false;
  switch (range) {
    case HistoryRange::Hour:
      for (size_t i = 0; i < h.minute().size(); ++i) emit(h.minute().at(i));
      has_p = h.minutePartial(&p);
      break;
    case HistoryRange::Week:
      for (size_t i = 0; i < h.coarse().size(); ++i) emit(h.coarse().at(i));
      has_p = h.coarsePartial(&p);
      break;
    default:
      for (size_t i = 0; i < h.fine().size(); ++i) emit(h.fine().at(i));
      has_p = h.finePartial(&p);
      break;
  }
  if (has_p) emit(p);
  o += "]}";
  return o;
}

}  // namespace ipas
