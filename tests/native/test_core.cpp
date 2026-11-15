// Host-side unit tests for history, trend and JSON builders. Build: see tests/Makefile
#include <stdio.h>
#include <string.h>
#include <fstream>
#include "../../include/air_quality.h"
#include "../../include/history.h"
#include "../../include/json_api.h"
#include "../../include/log_format.h"
#include "../../include/trend.h"

using namespace ipas;
static int g_fail = 0, g_checks = 0;
#define CHECK(c) do { ++g_checks; if (!(c)) { ++g_fail; printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #c); } } while (0)
#define NEAR(a, b, tol) CHECK(fabs((double)(a) - (double)(b)) <= (tol))

static Reading mk(float t, float h, float p, float g) { Reading r = {t, h, p, g}; return r; }

static void test_ring() {
  Ring<4> r;
  for (uint32_t i = 1; i <= 6; ++i) { Sample s = {}; s.t = i; r.push(s); }
  CHECK(r.size() == 4);
  CHECK(r.at(0).t == 3 && r.at(3).t == 6);  // oldest first, wrapped
}

static void test_plausible() {
  CHECK(plausibleReading(mk(22, 50, 1013, 100000)));
  CHECK(!plausibleReading(mk(NAN, 50, 1013, 1)));
  CHECK(!plausibleReading(mk(22, 101, 1013, 1)));
  CHECK(!plausibleReading(mk(22, 50, 20, 1)));
  CHECK(!plausibleReading(mk(120, 50, 1013, 1)));
  CHECK(plausibleReading(mk(22, 50, 1013, 0)));  // gas unavailable is allowed
}

static void test_accumulator() {
  Accumulator a(300);
  Sample out;
  CHECK(!a.add(10, mk(20, 40, 1000, 50000), &out));
  CHECK(!a.add(20, mk(22, 60, 1002, 0), &out));          // gas unavailable -> excluded from gas mean
  Sample part;
  CHECK(a.partial(&part));
  NEAR(part.temp_c100, 2100, 1);
  NEAR(part.hum_c100, 5000, 1);
  NEAR(part.press_pa, 100100, 1);
  CHECK(part.gas_ohm == 50000);
  CHECK(a.add(305, mk(30, 30, 1010, 10000), &out));      // new bucket completes the first
  CHECK(out.t == 15);
  NEAR(out.temp_c100, 2100, 1);
  CHECK(a.partial(&part) && part.t == 305);
}

static void test_history_capacity() {
  History h;
  const uint32_t hours = 26;
  for (uint32_t t = 0; t < hours * 3600; t += 3) h.add(t, mk(20, 50, 1000, 80000), nullptr);
  CHECK(h.fine().size() == kFineCount);        // bounded: exactly 24 h
  CHECK(h.coarse().size() == hours * 2 - 1 || h.coarse().size() == hours * 2);
  CHECK(h.fine().at(0).t < h.fine().at(1).t);
  const uint32_t now = hours * 3600;
  std::string j = historyJson(h, HistoryRange::Day, now, false);
  CHECK(j.find("\"range\":\"24h\"") != std::string::npos);
  CHECK(j.find("\"interval_s\":300") != std::string::npos);
  // memory budget check: all rings together stay below 16 kB
  CHECK(sizeof(History) < 16 * 1024);
}

static void fill_pressure(History& h, double hours, double slope_hpa_per_3h, double base = 1010) {
  for (uint32_t t = 0; t < hours * 3600; t += 3)
    h.add(t, mk(20, 50, (float)(base + slope_hpa_per_3h * (t / 10800.0)), 80000), nullptr);
}

static void test_trend() {
  { History h; Trend t = computeTrend(h, 100);
    CHECK(t.state == TrendState::Insufficient); }
  { History h; fill_pressure(h, 0.5, -3); Trend t = computeTrend(h, 1800);
    CHECK(t.state == TrendState::Insufficient); }                       // < 1 h of data
  { History h; fill_pressure(h, 3, 0.0); Trend t = computeTrend(h, 3 * 3600);
    CHECK(t.state == TrendState::Steady); NEAR(t.delta_hpa_3h, 0, 0.05); }
  { History h; fill_pressure(h, 3, -2.0); Trend t = computeTrend(h, 3 * 3600);
    CHECK(t.state == TrendState::Falling); NEAR(t.delta_hpa_3h, -2.0, 0.15); }
  { History h; fill_pressure(h, 3, -4.5); Trend t = computeTrend(h, 3 * 3600);
    CHECK(t.state == TrendState::FallingFast); }
  { History h; fill_pressure(h, 3, 1.5); Trend t = computeTrend(h, 3 * 3600);
    CHECK(t.state == TrendState::Rising); NEAR(t.delta_hpa_3h, 1.5, 0.15); }
  { History h; fill_pressure(h, 3, 3.5); Trend t = computeTrend(h, 3 * 3600);
    CHECK(t.state == TrendState::RisingFast); }
  { // old data (6 h ago) is outside the window -> insufficient
    History h; fill_pressure(h, 1.5, -3); Trend t = computeTrend(h, 8 * 3600);
    CHECK(t.state == TrendState::Insufficient); }
}


static void test_minute_tier_and_ranges() {
  History h;
  for (uint32_t t = 0; t < 2 * 3600; t += 3) h.add(t, mk(20, 50, 1000, 80000), nullptr);
  CHECK(h.minute().size() >= 118 && h.minute().size() <= kMinuteCount);
  const uint32_t now = 2 * 3600;
  std::string j1 = historyJson(h, HistoryRange::Hour, now, false);
  CHECK(j1.find("\"range\":\"1h\"") != std::string::npos && j1.find("\"interval_s\":60") != std::string::npos);
  size_t commas = 0; for (char c : j1) if (c == '[') ++commas;
  CHECK(commas >= 58 && commas <= 63);                    // about 60 one-minute points (+1 for the outer array)
  std::string j12 = historyJson(h, HistoryRange::HalfDay, now, false);
  CHECK(j12.find("\"range\":\"12h\"") != std::string::npos);
}

static void test_rebase() {
  History h;
  for (uint32_t t = 0; t < 4000; t += 3) h.add(t, mk(20, 50, 1000, 80000), nullptr);
  const uint32_t offset = 1760000000u - 4000;
  Sample fl;
  bool flushed = h.rebase(offset, &fl);
  CHECK(flushed && fl.t < kEpochMin);                      // returned sample is still uptime-based
  CHECK(h.fine().at(0).t >= kEpochMin && h.fine().at(h.fine().size() - 1).t >= kEpochMin);
  CHECK(h.minute().at(0).t >= kEpochMin);
  // new data after the sync continues on the Unix time line
  h.add(1760000000u + 3, mk(20, 50, 1000, 80000), nullptr);
  std::string j = historyJson(h, HistoryRange::Day, 1760000010u, true);
  CHECK(j.find("\"time_synced\":true") != std::string::npos);
  CHECK(j.find("\"points\":[]") == std::string::npos);
  // while the clock is NOT set, restored (Unix time) data must not leak into the response
  History h2;
  Sample s = {1760000000u, 2000, 5000, 100000, 50000};
  h2.loadFine(s);
  h2.add(10, mk(20, 50, 1000, 80000), nullptr);
  std::string ju = historyJson(h2, HistoryRange::Day, 20, false);
  CHECK(ju.find("1760000000") == std::string::npos);
}

static void feed(LogLoader& l, const std::string& s) {
  size_t a = 0;
  while (a < s.size()) { size_t b = s.find('\n', a); if (b == std::string::npos) b = s.size(); l.feedLine(s.substr(a, b - a).c_str()); a = b + 1; }
}

static void test_log_roundtrip() {
  // boot 1: clock set after 2 samples (uptime based), then 2 epoch samples; power cut;
  // boot 2: never synced (must be skipped); boot 3: synced, 1 sample; last line cut by power loss.
  std::string log = fmtBoot(1);
  Sample a = {300, 2100, 5000, 101000, 90000}, b = {600, 2110, 5010, 101010, 91000};
  log += fmtSample(1, a) + fmtSample(1, b);
  log += fmtSync(1, 700, 1760000700u);                       // offset = 1760000000
  Sample c = {1760000900u, 2120, 5020, 101020, 92000}, d = {1760001200u, 2130, 5030, 101030, 93000};
  log += fmtSample(1, c) + fmtSample(1, d);
  log += fmtBoot(2) + fmtSample(2, a);                      // boot 2: no sync record
  log += fmtBoot(3);
  Sample e = {120, 2200, 6000, 100500, 80000};
  log += fmtSample(3, e) + fmtSync(3, 200, 1760100200u);
  std::string cut = fmtSample(3, d); cut.resize(cut.size() - 3);   // truncated, no '*'
  log += cut;
  History h; LogLoader l(h);
  feed(l, log); l.finish();
  CHECK(l.stats().max_boot == 3);
  CHECK(l.stats().samples == 5);                             // a, b, c, d, e
  CHECK(l.stats().discarded_unsynced == 1);                 // boot 2
  CHECK(l.stats().bad_lines >= 1);                           // the cut line
  CHECK(h.fine().size() == 5);
  CHECK(h.fine().at(0).t == 1760000300u && h.fine().at(1).t == 1760000600u);   // converted through the sync record
  CHECK(h.fine().at(4).t == 1760100120u);
  // gap between boot 1 and boot 3 is preserved as a time gap, not bridged
  CHECK(h.fine().at(4).t - h.fine().at(3).t > 3 * kFineIntervalS);
  // garbage and out-of-range lines are rejected
  History h3; LogLoader l3(h3);
  l3.feedLine("hello"); l3.feedLine("S,1,2,3*"); l3.feedLine("S,1,1760000000,99999,5000,101000,0*"); l3.feedLine("S,1,1760000000,2000,5000,101000,0");
  l3.finish();
  CHECK(l3.stats().samples == 0 && l3.stats().bad_lines == 4);
}

static void test_log_history_integration() {
  // samples written by a running History are restored into the same shape, 7-day tier included
  History src;
  std::string log = fmtBoot(1);
  uint32_t base = 1760000000u;
  for (uint32_t t = base; t < base + 8 * 3600; t += 3) {
    Sample f;
    if (src.add(t, mk(21, 45, 1005, 70000), &f)) log += fmtSample(1, f);
  }
  History dst; LogLoader l(dst);
  feed(l, log); l.finish();
  CHECK(dst.fine().size() >= 90);
  CHECK(dst.coarse().size() >= 14);                          // 30-minute tier rebuilt from 5-minute samples
}

static void test_air_quality() {
  NEAR(aqScore(100000, 100000, 40), 100, 0.01);            // perfect: at baseline, ideal humidity
  NEAR(aqScore(150000, 100000, 40), 100, 0.01);            // above baseline does not exceed the cap
  NEAR(aqScore(50000, 100000, 40), 62.5, 0.01);            // half of baseline -> gas part 37.5 + humidity 25
  NEAR(aqScore(100000, 100000, 100), 75, 0.01);            // very humid: humidity part 0
  NEAR(aqScore(100000, 100000, 0), 75, 0.01);              // very dry: humidity part 0
  CHECK(aqLevelFromScore(100) == AqLevel::VeryGood && aqLevelFromScore(90) == AqLevel::VeryGood);
  CHECK(aqLevelFromScore(89.9f) == AqLevel::Good && aqLevelFromScore(75) == AqLevel::Good);
  CHECK(aqLevelFromScore(74.9f) == AqLevel::Medium && aqLevelFromScore(55) == AqLevel::Medium);
  CHECK(aqLevelFromScore(54.9f) == AqLevel::Bad && aqLevelFromScore(40) == AqLevel::Bad);
  CHECK(aqLevelFromScore(39.9f) == AqLevel::VeryBad && aqLevelFromScore(0) == AqLevel::VeryBad);

  History h;
  Reading r = mk(22, 40, 1000, 100000);
  AirQuality aq = computeAirQuality(h, true, r, 100);
  CHECK(aq.state == AqState::Calibrating);                  // no history yet
  CHECK(computeAirQuality(h, false, r, 100).state == AqState::Unavailable);
  CHECK(computeAirQuality(h, true, mk(22, 40, 1000, 0), 100).state == AqState::Unavailable);
  for (uint32_t t = 0; t < 8 * 3600; t += 3) h.add(t, mk(22, 40, 1000, 100000), nullptr);
  aq = computeAirQuality(h, true, r, 8 * 3600);
  CHECK(aq.state == AqState::Ok && aq.level == AqLevel::VeryGood);
  NEAR(aq.baseline_ohm, 100000, 1);
  aq = computeAirQuality(h, true, mk(22, 40, 1000, 10000), 8 * 3600);   // gas at 10 % of baseline
  CHECK(aq.state == AqState::Ok && aq.level == AqLevel::VeryBad);
  std::string js = statusJson(DeviceSnapshot(), h, Trend(), aq);
  CHECK(js.find("\"air_quality\":{\"state\":\"ok\",\"level\":\"very_bad\"") != std::string::npos);
}


// Mirrors src/main.cpp: boot without clock, the browser sets the clock after 11 s, then 12 h of
// readings every 3 s; every finished 5-minute sample is appended to the log like SdStore does.
static void test_main_flow_simulation() {
  History h;
  std::string log = fmtBoot(1);
  const uint32_t unix_at_sync = 1793470522u;
  uint32_t offset = 0;
  bool synced = false;
  for (uint32_t up = 3; up < 12 * 3600; up += 3) {
    if (!synced && up >= 11) {
      offset = unix_at_sync - up;
      Sample fl;
      if (h.rebase(offset, &fl)) log += fmtSample(1, fl);
      log += fmtSync(1, up, unix_at_sync);
      synced = true;
    }
    Sample f;
    if (h.add(up + (synced ? offset : 0), mk(22, 50, 1010, 90000), &f)) log += fmtSample(1, f);
  }
  size_t s_lines = 0;
  for (size_t i = 0; (i = log.find("\nS,", i)) != std::string::npos; ++i) ++s_lines;
  CHECK(s_lines >= 140 && s_lines <= 146);                 // about one line per 5 minutes for 12 hours
  // power cut, new boot: everything written so far comes back
  History r; LogLoader l(r);
  feed(l, log); l.finish();
  CHECK(l.stats().samples == s_lines);
  CHECK(r.fine().size() >= 140);
  CHECK(r.coarse().size() >= 20);
  CHECK(r.fine().at(r.fine().size() - 1).t > unix_at_sync + 11 * 3600);
  // after a short outage the gap is visible, not bridged
  const uint32_t later = r.fine().at(r.fine().size() - 1).t + 1800;
  std::string j = historyJson(r, HistoryRange::HalfDay, later, true);
  CHECK(j.find("\"time_synced\":true") != std::string::npos);
}

// Readings with gas resistance 0 are what a misbehaving sensor returned in the field; they must not enter the history.
static void test_accept_reading() {
  CHECK(acceptReading(mk(22, 50, 1010, 9446)));
  CHECK(!acceptReading(mk(33.89f, 100, 761.08f, 0)));
  CHECK(!acceptReading(mk(22, 50, 1010, NAN)));
  CHECK(!acceptReading(mk(22, 50, 20, 9000)));
}

static void test_json(const char* outdir) {
  History h;
  DeviceSnapshot d; d.uptime_s = 12; d.sensor = SensorState::InitFailed;
  std::string empty_status = statusJson(d, h, computeTrend(h, 12), AirQuality());
  CHECK(empty_status.find("\"latest\":null") != std::string::npos);
  CHECK(empty_status.find("\"state\":\"init_failed\"") != std::string::npos);
  CHECK(empty_status.find("\"i2c_addr\":null") != std::string::npos);
  CHECK(empty_status.find("\"last_ok_age_s\":null") != std::string::npos);

  fill_pressure(h, 3, -2.0);
  d.uptime_s = 3 * 3600; d.sensor = SensorState::Ok; d.i2c_addr = 0x77; d.last_ok_age_s = 2;
  d.has_latest = true; d.latest = mk(23.456f, 54.2f, 1008.44f, 123456); d.reads_ok = 3600;
  std::string st = statusJson(d, h, computeTrend(h, d.uptime_s), AirQuality());
  CHECK(st.find("\"temperature_c\":23.46") != std::string::npos);
  CHECK(st.find("\"gas_ohm\":123456") != std::string::npos);
  CHECK(st.find("\"state\":\"falling\"") != std::string::npos);
  CHECK(st.find("\"i2c_addr\":\"0x77\"") != std::string::npos);
  d.storage.mount_attempts = 3; d.storage.restarts = 1;
  CHECK(statusJson(d, h, Trend(), AirQuality()).find("\"mount_attempts\":3,\"restarts\":1") != std::string::npos);

  d.latest.gas_ohm = 0;
  CHECK(statusJson(d, h, Trend(), AirQuality()).find("\"gas_ohm\":null") != std::string::npos);

  std::string day = historyJson(h, HistoryRange::Day, d.uptime_s, false);
  std::string wk = historyJson(h, HistoryRange::Week, d.uptime_s, false);
  CHECK(day.find("\"range\":\"24h\"") != std::string::npos && wk.find("\"range\":\"7d\"") != std::string::npos);
  CHECK(wk.find("\"interval_s\":1800") != std::string::npos);

  History none;
  CHECK(historyJson(none, HistoryRange::Day, 50, false).find("\"points\":[]") != std::string::npos);

  if (outdir) {  // fixtures for the JavaScript contract test
    std::string p(outdir);
    std::ofstream(p + "/status_ok.json") << st.c_str();
    std::ofstream(p + "/status_empty.json") << empty_status.c_str();
    std::ofstream(p + "/history_24h.json") << day.c_str();
    std::ofstream(p + "/history_7d.json") << wk.c_str();
    AirQuality aqf; aqf.state = AqState::Ok; aqf.level = AqLevel::Good; aqf.score = 76.4f; aqf.baseline_ohm = 150000; aqf.points = 40;
    d.latest.gas_ohm = 123456;
    std::ofstream(p + "/status_ok.json") << statusJson(d, h, computeTrend(h, d.uptime_s), aqf).c_str();
    std::ofstream(p + "/history_1h.json") << historyJson(h, HistoryRange::Hour, d.uptime_s, false).c_str();
    std::ofstream(p + "/history_12h.json") << historyJson(h, HistoryRange::HalfDay, d.uptime_s, false).c_str();
  }
}

int main(int argc, char** argv) {
  test_ring(); test_plausible(); test_accumulator(); test_history_capacity(); test_trend();
  test_minute_tier_and_ranges(); test_rebase(); test_log_roundtrip(); test_log_history_integration(); test_air_quality();
  test_main_flow_simulation(); test_accept_reading();
  test_json(argc > 1 ? argv[1] : nullptr);
  printf("%d checks, %d failed\n", g_checks, g_fail);
  printf("sizeof(History)=%u bytes, sizeof(Sample)=%u\n", (unsigned)sizeof(History), (unsigned)sizeof(Sample));
  return g_fail ? 1 : 0;
}
