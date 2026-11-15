const test = require('node:test');
const assert = require('node:assert');
const fs = require('node:fs');
const path = require('node:path');
const L = require('../../data/ipas-logic.js');

const FIX = path.join(__dirname, '..', 'fixtures');
const fx = n => JSON.parse(fs.readFileSync(path.join(FIX, n), 'utf8'));
const okStatus = () => fx('status_ok.json');

test('fixtures from the C++ builders are valid JSON with the documented shape', () => {
  const s = okStatus();
  assert.strictEqual(s.api, 2);
  assert.strictEqual(s.sensor.state, 'ok');
  assert.ok(typeof s.latest.temperature_c === 'number');
  assert.strictEqual(s.pressure_trend.state, 'falling');
  const h = fx('history_24h.json');
  assert.ok(h.points.length > 10);
  assert.strictEqual(h.points[0].length, 5);
  assert.strictEqual(s.air_quality.state, 'ok');
  assert.strictEqual(typeof s.time.synced, 'boolean');
  assert.ok(['ok', 'no_card', 'error', 'disabled'].includes(s.storage.state));
});

test('live when fresh', () => {
  const s = okStatus(); s.sensor.last_ok_age_s = 2;
  const v = L.deriveState({ status: s, receivedAtMs: 1000, nowMs: 2000, failures: 0 });
  assert.strictEqual(v.state, 'live'); assert.ok(v.showValues);
});

test('stale once the reading is older than the device limit (also by local clock)', () => {
  const s = okStatus(); s.sensor.last_ok_age_s = 2;
  const v = L.deriveState({ status: s, receivedAtMs: 0, nowMs: 20000, failures: 0 });
  assert.strictEqual(v.state, 'stale'); assert.ok(v.ageS > 15);
});

test('device reports stale itself', () => {
  const s = okStatus(); s.sensor.last_ok_age_s = 40;
  assert.strictEqual(L.deriveState({ status: s, receivedAtMs: 0, nowMs: 0, failures: 0 }).state, 'stale');
});

test('unreachable: never shown as live, last value only as stale/dimmed', () => {
  assert.deepStrictEqual(L.deriveState({ status: null, failures: 5, nowMs: 0, receivedAtMs: 0 }),
    { state: 'unreachable', showValues: false, ageS: null });
  const s = okStatus();
  const v = L.deriveState({ status: s, receivedAtMs: 0, nowMs: 60000, failures: 2 });
  assert.strictEqual(v.state, 'unreachable'); assert.ok(v.ageS >= 60);
});

test('one failed poll does not flip to unreachable', () => {
  const s = okStatus(); s.sensor.last_ok_age_s = 1;
  assert.strictEqual(L.deriveState({ status: s, receivedAtMs: 0, nowMs: 1000, failures: 1 }).state, 'live');
});

test('sensor error and no data show no values', () => {
  const e = fx('status_empty.json');
  const v = L.deriveState({ status: e, receivedAtMs: 0, nowMs: 0, failures: 0 });
  assert.strictEqual(v.state, 'sensor_error'); assert.strictEqual(v.showValues, false);
  e.sensor.state = 'no_data';
  assert.strictEqual(L.deriveState({ status: e, receivedAtMs: 0, nowMs: 0, failures: 0 }).state, 'no_data');
});

test('read_failed keeps the last real value visible but flagged as error', () => {
  const s = okStatus(); s.sensor.state = 'read_failed'; s.sensor.last_ok_age_s = 30;
  const v = L.deriveState({ status: s, receivedAtMs: 0, nowMs: 0, failures: 0 });
  assert.strictEqual(v.state, 'sensor_error'); assert.ok(v.showValues);
});

test('demo is only active when explicitly requested', () => {
  assert.strictEqual(L.deriveState({ demo: true }).state, 'demo');
  assert.notStrictEqual(L.deriveState({ status: null, failures: 0, nowMs: 0, receivedAtMs: 0 }).state, 'demo');
});

test('wall-clock reconstruction from device uptime', () => {
  assert.strictEqual(L.toWallMs(900, 1000, 5_000_000), 5_000_000 - 100_000);
});

test('series built from real history; null gas points are skipped, not invented', () => {
  const h = { now_s: 1000, points: [[100, 20, 50, 1000, 50000], [400, 21, 51, 1001, null], [700, 22, 52, 1002, 70000]] };
  assert.deepStrictEqual(L.buildSeries(h, 'airquality', 0).values, [50, 70]);
  assert.deepStrictEqual(L.buildSeries(h, 'temperature', 0).values, [20, 21, 22]);
  assert.strictEqual(L.buildSeries({ now_s: 1, points: [] }, 'pressure', 0).values.length, 0);
});

test('real history fixtures produce series on both ranges', () => {
  for (const f of ['history_1h.json', 'history_12h.json', 'history_24h.json', 'history_7d.json']) {
    const h = fx(f);
    for (const m of L.METRICS) {
      const s = L.buildSeries(h, m, Date.now());
      assert.ok(s.values.length > 0, `${f} ${m}`);
      assert.ok(s.stamps.every(d => !isNaN(d.getTime())));
    }
  }
});

test('trend view and formatting', () => {
  assert.strictEqual(L.trendView({ state: 'insufficient', delta_hpa_3h: null }).key, 'insufficient');
  const v = L.trendView({ state: 'falling', delta_hpa_3h: -2.35 });
  assert.strictEqual(v.arrow, '\u2193');
  assert.strictEqual(L.formatDelta(-2.35), '\u22122.4');
  assert.strictEqual(L.formatDelta(0.02), '+0.0');
  assert.strictEqual(L.formatValue('pressure', null), '\u2014');
});

test('axis ticks stay inside the window', () => {
  const end = Date.UTC(2026, 9, 27, 14, 32);
  for (const r of ['1h', '12h', '24h', '7d']) {
    const t = L.axisTicks(r, end);
    assert.ok(t.length >= 5, r + ' ticks');
    assert.ok(t.every(x => x <= end && x >= end - L.RANGE_S[r] * 1000));
  }
});
