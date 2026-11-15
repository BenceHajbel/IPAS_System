/* IPAS dashboard logic: pure functions with no DOM access, unit-tested with Node.
   Loaded by index.html (global IPASLogic) and by tests/js (module.exports). */
(function (root) {
  'use strict';

  var METRICS = ['temperature', 'humidity', 'pressure', 'airquality'];
  var RANGE_S = { '1h': 3600, '12h': 12 * 3600, '24h': 24 * 3600, '7d': 7 * 24 * 3600 };
  var TICK_S = { '1h': 600, '12h': 7200, '24h': 4 * 3600, '7d': 24 * 3600 };
  var MAX_POLL_FAILURES = 2;

  /* Connection / data state shown to the user.
     input: { demo, status (last good /api/status or null), receivedAtMs, nowMs, failures }
     result: { state, showValues, ageS }
       live        sensor ok and fresh
       stale       last valid reading is older than the device's stale limit
       no_data     sensor initialised, no valid reading yet
       sensor_error  BME680 not found / failing
       unreachable device not reachable over Wi-Fi (last values are shown as stale, never as live)
       demo        explicit demo mode (never automatic) */
  function deriveState(input) {
    if (input.demo) return { state: 'demo', showValues: true, ageS: 0 };
    var st = input.status;
    var unreachable = !st || input.failures >= MAX_POLL_FAILURES;
    if (!st) return { state: 'unreachable', showValues: false, ageS: null };
    var sinceFetchS = Math.max(0, (input.nowMs - input.receivedAtMs) / 1000);
    var hasValue = !!st.latest && st.sensor && st.sensor.last_ok_age_s !== null;
    var ageS = hasValue ? st.sensor.last_ok_age_s + sinceFetchS : null;
    if (unreachable) return { state: 'unreachable', showValues: hasValue, ageS: ageS };
    var s = st.sensor ? st.sensor.state : 'init_failed';
    if (s === 'init_failed') return { state: 'sensor_error', showValues: false, ageS: ageS };
    if (s === 'no_data') return { state: 'no_data', showValues: false, ageS: null };
    if (s === 'read_failed') return { state: 'sensor_error', showValues: hasValue, ageS: ageS };
    if (!hasValue) return { state: 'no_data', showValues: false, ageS: null };
    if (ageS > st.stale_after_s) return { state: 'stale', showValues: true, ageS: ageS };
    return { state: 'live', showValues: true, ageS: ageS };
  }

  /* Device timestamps are "timeline seconds": uptime until the browser has set the
     clock, Unix time afterwards (the ESP32 has no real-time clock). Wall-clock time is
     reconstructed as (time the response arrived) - (device now - sample time). */
  function toWallMs(sampleTimeS, deviceNowS, receivedAtMs) {
    return receivedAtMs - (deviceNowS - sampleTimeS) * 1000;
  }

  function metricValue(point, metric) {
    switch (metric) {
      case 'temperature': return point[1];
      case 'humidity': return point[2];
      case 'pressure': return point[3];
      case 'airquality': return point[4] === null ? null : point[4] / 1000; // kOhm
      default: return null;
    }
  }

  /* history: parsed /api/history response. Returns {values, stamps(Date)}.
     Points with a missing value are skipped, never interpolated or invented. */
  function buildSeries(history, metric, receivedAtMs) {
    var values = [], stamps = [];
    (history.points || []).forEach(function (p) {
      var v = metricValue(p, metric);
      if (v === null || !isFinite(v)) return;
      values.push(v);
      stamps.push(new Date(toWallMs(p[0], history.now_s, receivedAtMs)));
    });
    return { values: values, stamps: stamps };
  }

  function coverageSeconds(history) {
    var pts = history.points || [];
    if (pts.length < 2) return 0;
    return pts[pts.length - 1][0] - pts[0][0];
  }

  function formatValue(metric, v, lang) {
    if (v === null || v === undefined || !isFinite(v)) return '\u2014';
    if (metric === 'airquality') return v.toFixed(1);
    return v.toFixed(1);
  }

  function formatDuration(seconds, lang) {
    var m = Math.round(seconds / 60);
    var h = Math.floor(m / 60), r = m % 60;
    if (lang === 'hu') return h ? h + ' \u00f3ra ' + r + ' perc' : r + ' perc';
    return h ? h + ' h ' + r + ' min' : r + ' min';
  }

  /* Pressure trend view model from /api/status.pressure_trend. */
  function trendView(trend) {
    if (!trend || trend.state === 'insufficient' || trend.delta_hpa_3h === null) {
      return { key: 'insufficient', arrow: '\u2192', delta: null };
    }
    var d = trend.delta_hpa_3h;
    var arrow = d < -0.1 ? '\u2193' : d > 0.1 ? '\u2191' : '\u2192';
    return { key: trend.state, arrow: arrow, delta: d };
  }

  function formatDelta(d) {
    var sign = d < 0 ? '\u2212' : d > 0 ? '+' : '\u00b1';
    return sign + Math.abs(d).toFixed(1);
  }

  /* Time-axis tick positions (ms) inside [startMs, endMs]. */
  function axisTicks(range, endMs) {
    var step = TICK_S[range] * 1000;
    var startMs = endMs - RANGE_S[range] * 1000;
    var first = Math.ceil(startMs / step) * step;
    var ticks = [];
    for (var t = first; t <= endMs; t += step) ticks.push(t);
    return ticks;
  }

  /* ---- explicit demo data (only when the page is opened with ?demo=1) ---- */
  function demoStatus() {
    return {
      api: 2, uptime_s: 90000, stale_after_s: 15, time: { synced: true, now_s: 1760000000 },
      air_quality: { state: 'ok', level: 'good', score: 78.2, baseline_ohm: 110000, calibration_points: 40, calibration_needed: 12 },
      storage: { state: 'disabled' },
      sensor: { state: 'ok', i2c_addr: '0x77', last_ok_age_s: 1, reads_ok: 1, read_errors: 0 },
      latest: { temperature_c: 23.7, humidity_pct: 54.2, pressure_hpa: 1008.4, gas_ohm: 85000 },
      pressure_trend: { state: 'falling', delta_hpa_3h: -2.4, span_s: 10800, points: 36 },
      history: { fine_count: 288, fine_capacity: 288, coarse_count: 336, coarse_capacity: 336 },
      system: { heap_free: 0, stations: 1 }
    };
  }
  function demoHistory(range) {
    var n = { '1h': 60, '12h': 144, '24h': 288, '7d': 336 }[range], step = { '1h': 60, '12h': 300, '24h': 300, '7d': 1800 }[range], up = 1760000000, pts = [];
    for (var i = 0; i < n; i++) {
      var t = up - (n - 1 - i) * step;
      pts.push([t, +(23.7 + 2 * Math.sin(i / 20)).toFixed(2), +(54 + 5 * Math.sin(i / 15)).toFixed(2),
        +(1010 - 3 * (i / n)).toFixed(2), Math.round(85000 + 20000 * Math.sin(i / 10))]);
    }
    return { api: 2, range: range, now_s: up, time_synced: true, interval_s: step, points: pts };
  }

  var api = {
    METRICS: METRICS, RANGE_S: RANGE_S, MAX_POLL_FAILURES: MAX_POLL_FAILURES,
    deriveState: deriveState, toWallMs: toWallMs, metricValue: metricValue, buildSeries: buildSeries,
    coverageSeconds: coverageSeconds, formatValue: formatValue, formatDuration: formatDuration,
    trendView: trendView, formatDelta: formatDelta, axisTicks: axisTicks,
    demoStatus: demoStatus, demoHistory: demoHistory
  };
  if (typeof module !== 'undefined' && module.exports) module.exports = api;
  else root.IPASLogic = api;
})(typeof window !== 'undefined' ? window : this);
