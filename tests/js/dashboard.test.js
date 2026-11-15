// Runs the real data/index.html in jsdom against a fake device API.
const test = require('node:test');
const { after } = require('node:test');
const windows = [];
after(() => windows.forEach(w => w.close()));  // stops the dashboard's polling timers
const assert = require('node:assert');
const fs = require('node:fs');
const path = require('node:path');
let JSDOM; try { ({ JSDOM } = require('jsdom')); } catch (e) { JSDOM = null; }

const DATA = path.join(__dirname, '..', '..', 'data');
const FIX = path.join(__dirname, '..', 'fixtures');
const fx = n => JSON.parse(fs.readFileSync(path.join(FIX, n), 'utf8'));

function boot({ url = 'http://192.168.4.1/', routes }) {
  const html = fs.readFileSync(path.join(DATA, 'index.html'), 'utf8').replace('<script src="/ipas-logic.js"></script>', '').replace('<script src="/app.js"></script>', '');
  const appjs = fs.readFileSync(path.join(DATA, 'app.js'), 'utf8');
  const logic = fs.readFileSync(path.join(DATA, 'ipas-logic.js'), 'utf8');
  const dom = new JSDOM(html, { url, runScripts: 'outside-only', pretendToBeVisual: true });
  const w = dom.window;
  windows.push(w);
  w.matchMedia = () => ({ matches: false });
  const calls = [];
  w.fetch = async (u, opts) => {
    calls.push({ url: u, method: (opts && opts.method) || 'GET', body: opts && opts.body });
    const r = routes(u, opts);
    if (r === 'fail') throw new Error('network');
    return { ok: true, json: async () => r };
  };
  w.eval(logic);
  const errors = [];
  w.addEventListener('error', e => errors.push(e.message));
  w.eval(appjs);
  return { w, errors, doc: w.document, calls };
}
const tick = (ms = 20) => new Promise(r => setTimeout(r, ms));
const txt = (doc, id) => doc.getElementById(id).textContent;

test('page contains no external requests, simulated numbers or ROS code', () => {
  const html = fs.readFileSync(path.join(DATA, 'index.html'), 'utf8') + fs.readFileSync(path.join(DATA, 'app.js'), 'utf8');
  assert.ok(!/https?:\/\/(?!www\.w3\.org)/.test(html.replace(/\/\*[\s\S]*?\*\//g, '')), 'external URL found');
  assert.ok(!/roslib|rosbridge|Math\.random/.test(html));
  assert.ok(!html.includes('23.7') && !html.includes('1008.4'));
  assert.ok(!html.includes('section-comparison') && !html.includes('th_official'));
});

test('live data is rendered from the API and states are honest', { skip: !JSDOM && 'jsdom not installed' }, async () => {
  const status = fx('status_ok.json');
  const { w, errors, doc } = boot({ routes: u => u.includes('/api/status') ? status : fx('history_' + (u.match(/range=(\w+)/) || [0, '24h'])[1] + '.json') });
  await tick(50);
  assert.deepStrictEqual(errors, []);
  assert.strictEqual(txt(doc, 'online-status-text'), 'ONLINE');
  assert.ok(doc.getElementById('status-banner').hidden);
  const vals = [...doc.querySelectorAll('.m-value')].map(e => e.textContent);
  assert.deepStrictEqual(vals, ['23.5', '54.2', '1008.4', 'JÓ']);
  assert.ok(/123\.5 kΩ/.test(doc.querySelectorAll('.m-unit')[3].textContent));
  assert.ok(doc.querySelector('.m-value.aq-good'), 'good = green class');
  assert.ok(doc.querySelectorAll('#history-graph circle').length > 10);
  assert.ok(txt(doc, 'weather-trend-title').length > 0);
  assert.ok(/hPa \/ 3h/.test(txt(doc, 'pressure-change')));
  // language switch
  w.setLanguage('en');
  assert.strictEqual(doc.documentElement.lang, 'en');
  assert.ok(/not a guaranteed weather forecast/.test(doc.querySelector('.pressure-disclaimer').textContent));
  assert.ok(/TEMPERATURE/.test(doc.querySelector('.m-label').textContent));
  // 7d range
  for (const r of ['1h', '12h', '7d']) {
    doc.querySelector(`[data-range="${r}"]`).click();
    await tick(50);
    assert.ok(new RegExp(r.toUpperCase()).test(txt(doc, 'graph-title')), r);
    assert.ok(doc.querySelectorAll('#history-graph circle').length > 5, r + ' points');
  }
  assert.ok(!doc.getElementById('section-comparison'));
});

test('unreachable device shows no live values and a connection error', { skip: !JSDOM && 'jsdom not installed' }, async () => {
  const { w, doc, errors } = boot({ routes: () => 'fail' });
  await tick(50);
  await w.pollStatus(); await w.pollStatus();
  assert.deepStrictEqual(errors, []);
  assert.strictEqual(txt(doc, 'online-status-text'), 'NINCS KAPCSOLAT');
  assert.ok(doc.getElementById('online-status').classList.contains('offline'));
  assert.ok(!doc.getElementById('status-banner').hidden);
  assert.deepStrictEqual([...doc.querySelectorAll('.m-value')].map(e => e.textContent), ['\u2014', '\u2014', '\u2014', '\u2014']);
  assert.ok(/nincs mért adat|No measured/.test(doc.getElementById('history-graph').textContent));
});

test('sensor error state', { skip: !JSDOM && 'jsdom not installed' }, async () => {
  const { doc } = boot({ routes: u => u.includes('/api/status') ? fx('status_empty.json') : { api: 1, uptime_s: 12, interval_s: 300, points: [] } });
  await tick(50);
  assert.strictEqual(txt(doc, 'online-status-text'), 'SZENZORHIBA');
  assert.ok(/BME680/.test(doc.getElementById('status-banner').textContent));
  assert.deepStrictEqual([...doc.querySelectorAll('.m-value')].map(e => e.textContent), ['\u2014', '\u2014', '\u2014', '\u2014']);
});

test('demo data only with ?demo=1 and always labelled', { skip: !JSDOM && 'jsdom not installed' }, async () => {
  const { doc } = boot({ url: 'http://192.168.4.1/?demo=1', routes: () => 'fail' });
  await tick(50);
  assert.strictEqual(txt(doc, 'online-status-text'), 'DEMÓ ADAT');
  assert.ok(/DEMÓ MÓD/.test(doc.getElementById('status-banner').textContent));
  const { doc: d2 } = boot({ routes: () => 'fail' });
  await tick(50);
  assert.notStrictEqual(txt(d2, 'online-status-text'), 'DEMÓ ADAT');
});

const AQ_CASES = [['very_bad', 'NAGYON ROSSZ', 'aq-very_bad'], ['bad', 'ROSSZ', 'aq-bad'], ['medium', 'KÖZEPES', 'aq-medium'], ['good', 'JÓ', 'aq-good'], ['very_good', 'NAGYON JÓ', 'aq-very_good']];
for (const [level, label, cls] of AQ_CASES) {
  test(`air quality ${level} is shown as text with its colour class`, { skip: !JSDOM && 'jsdom not installed' }, async () => {
    const status = fx('status_ok.json');
    status.air_quality = { state: 'ok', level, score: 50, baseline_ohm: 100000, calibration_points: 40, calibration_needed: 12 };
    const { doc, w } = boot({ routes: u => u.includes('/api/status') ? status : fx('history_24h.json') });
    await tick(50);
    const el = doc.querySelectorAll('.m-value')[3];
    assert.strictEqual(el.textContent, label);
    assert.ok(el.classList.contains(cls));
    w.setLanguage('en');
    assert.strictEqual(doc.querySelectorAll('.m-value')[3].textContent, { very_bad: 'VERY BAD', bad: 'BAD', medium: 'MEDIUM', good: 'GOOD', very_good: 'VERY GOOD' }[level]);
    // the info popup documents the scale
    const tip = doc.querySelectorAll('.m-tooltip')[3].textContent;
    assert.ok(/Pimoroni/.test(tip) && /90/.test(tip) && /BSEC/.test(tip));
  });
}

test('air quality while calibrating shows no rating', { skip: !JSDOM && 'jsdom not installed' }, async () => {
  const status = fx('status_ok.json');
  status.air_quality = { state: 'calibrating', level: null, score: null, baseline_ohm: null, calibration_points: 3, calibration_needed: 12 };
  const { doc } = boot({ routes: u => u.includes('/api/status') ? status : fx('history_24h.json') });
  await tick(50);
  assert.strictEqual(doc.querySelectorAll('.m-value')[3].textContent, 'KALIBRÁLÁS');
  assert.ok(/3\/12/.test(doc.querySelectorAll('.m-unit')[3].textContent));
});

test('browser clock is sent once when the device clock is not set', { skip: !JSDOM && 'jsdom not installed' }, async () => {
  let synced = false;
  const base = fx('status_ok.json');
  const { calls, doc } = boot({ routes: (u, o) => {
    if (u.includes('/api/time')) { synced = true; return { synced: true }; }
    if (u.includes('/api/status')) return Object.assign({}, base, { time: { synced, now_s: 1 } });
    return fx('history_24h.json');
  } });
  await tick(80);
  const posts = calls.filter(c => c.method === 'POST');
  assert.strictEqual(posts.length, 1);
  assert.ok(Math.abs(JSON.parse(posts[0].body).epoch_s - Date.now() / 1000) < 5);
  assert.strictEqual(doc.getElementById('online-status-text').textContent, 'ONLINE');
});

test('storage state decides the history note', { skip: !JSDOM && 'jsdom not installed' }, async () => {
  for (const [state, re] of [['ok', /SD-kártyára is mentődnek/], ['no_card', /Nincs SD-kártya/], ['error', /SD-kártya hiba/]]) {
    const st = fx('status_ok.json'); st.storage = { state, loaded_samples: 12, file_bytes: 2048, write_errors: 0, mount_attempts: 3 };
    const { doc } = boot({ routes: u => u.includes('/api/status') ? st : fx('history_24h.json') });
    await tick(50);
    assert.ok(re.test(doc.getElementById('history-note').textContent), state);
    if (state === 'ok') assert.ok(/SD: 12 minta visszatöltve.*2\.0 kB.*írási hiba: 0 · csatlakozási kísérlet: 3/.test(doc.getElementById('history-note').textContent));
  }
});
