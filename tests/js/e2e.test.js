// Loads the real index.html through <script src> from a mock of the ESP32's HTTP server
// (same routes, same 302 behaviour for unknown paths) - no hand-injected scripts.
const test = require('node:test');
const assert = require('node:assert');
const fs = require('node:fs');
const http = require('node:http');
const path = require('node:path');
let JSDOM; try { ({ JSDOM } = require('jsdom')); } catch (e) { JSDOM = null; }

const DATA = path.join(__dirname, '..', '..', 'data');
const FIX = path.join(__dirname, '..', 'fixtures');
const fx = n => JSON.parse(fs.readFileSync(path.join(FIX, n), 'utf8'));
const MIME = { '.html': 'text/html', '.js': 'application/javascript', '.png': 'image/png' };

function mockDevice(opts = {}) {
  let synced = false;
  const posts = [];
  const server = http.createServer((req, res) => {
    const u = new URL(req.url, 'http://x');
    const json = (o, code = 200) => { res.writeHead(code, { 'Content-Type': 'application/json' }); res.end(JSON.stringify(o)); };
    const file = (f) => { res.writeHead(200, { 'Content-Type': MIME[path.extname(f)] }); res.end(fs.readFileSync(path.join(DATA, f))); };
    if (u.pathname === '/' || u.pathname === '/index.html') return file('index.html');
    if (u.pathname === '/ipas-logic.js') return file('ipas-logic.js');
    if (u.pathname === '/logo.png') return file('logo.png');
    if (u.pathname === '/app.js' && !opts.noAppRoute) return file('app.js');   // old firmware has no /app.js route
    if (u.pathname === '/api/status') {
      const s = fx('status_ok.json');
      if (opts.oldFirmware) { s.api = 1; delete s.time; delete s.air_quality; delete s.storage; }
      else s.time = { synced, now_s: s.uptime_s };
      return json(s);
    }
    if (u.pathname === '/api/time' && req.method === 'POST') {
      let b = ''; req.on('data', c => b += c); req.on('end', () => { posts.push(b); synced = true; json({ synced: true }); });
      return;
    }
    if (u.pathname === '/api/history') {
      const r = u.searchParams.get('range');
      if (opts.oldFirmware) {
        if (r !== '24h' && r !== '7d') return json({ error: 'range must be 24h or 7d' }, 400);
        const h = fx(`history_${r}.json`); h.api = 1; h.uptime_s = h.now_s; delete h.now_s; return json(h);
      }
      return json(fx(`history_${r}.json`));
    }
    res.writeHead(302, { Location: opts.redirectAway ? 'http://192.0.2.1/' : '/' }); res.end();   // firmware: captive-portal redirect to the dashboard
  });
  return new Promise(r => server.listen(0, '127.0.0.1', () => r({ server, posts, base: `http://127.0.0.1:${server.address().port}/` })));
}

async function open(base, extraWaitMs = 0) {
  const dom = await JSDOM.fromURL(base, {
    runScripts: 'dangerously', resources: 'usable', pretendToBeVisual: true,
    beforeParse(w) {
      w.matchMedia = () => ({ matches: false });
      w.fetch = (u, o) => fetch(new URL(u, base), o);
    }
  });
  await new Promise(r => setTimeout(r, 400 + extraWaitMs));
  return dom;
}
const vals = d => [...d.window.document.querySelectorAll('.m-value')].map(e => e.textContent);

test('real script loading: cards, pill, graph and all four range buttons work', { skip: !JSDOM && 'jsdom not installed' }, async () => {
  const { server, posts, base } = await mockDevice();
  const dom = await open(base);
  const doc = dom.window.document;
  assert.strictEqual(dom.window.__ipasStarted, true);
  assert.ok(doc.getElementById('boot-error').hidden);
  assert.strictEqual(doc.querySelectorAll('.measurement-card').length, 4, 'four measurement cards');
  assert.deepStrictEqual(vals(dom), ['23.5', '54.2', '1008.4', 'JÓ']);
  assert.strictEqual(doc.getElementById('online-status-text').textContent, 'ONLINE');
  assert.strictEqual(posts.length, 1, 'browser clock sent once');
  for (const r of ['1h', '12h', '24h', '7d']) {
    doc.querySelector(`[data-range="${r}"]`).click();
    await new Promise(x => setTimeout(x, 300));
    assert.ok(doc.querySelector(`[data-range="${r}"]`).classList.contains('active'), r + ' active');
    assert.ok(doc.querySelectorAll('#history-graph circle').length > 5, r + ' has points');
  }
  dom.window.close(); server.close();
});

test('firmware without the /app.js route: the failure is shown on the page, not silent', { skip: !JSDOM && 'jsdom not installed' }, async () => {
  const { server, base } = await mockDevice({ noAppRoute: true });          // /app.js answered with the HTML page
  const dom = await open(base);
  const b = dom.window.document.getElementById('boot-error');
  assert.ok(!dom.window.__ipasStarted);
  assert.strictEqual(b.hidden, false);
  assert.ok(/uploadfs/.test(b.textContent));
  dom.window.close(); server.close();
});

test('a script that never arrives is reported after a few seconds', { skip: !JSDOM && 'jsdom not installed' }, async () => {
  const { server, base } = await mockDevice({ noAppRoute: true, redirectAway: true });   // redirect to an unreachable host
  const dom = await open(base, 4500);
  const b = dom.window.document.getElementById('boot-error');
  assert.strictEqual(b.hidden, false);
  assert.ok(/timeout/.test(b.textContent));
  dom.window.close(); server.close();
});

test('old firmware (API v1) with the new page: version mismatch is reported', { skip: !JSDOM && 'jsdom not installed' }, async () => {
  const { server, base } = await mockDevice({ oldFirmware: true });
  const dom = await open(base);
  const banner = dom.window.document.getElementById('status-banner');
  assert.strictEqual(banner.hidden, false);
  assert.ok(/nem egyezik/.test(banner.textContent));
  dom.window.close(); server.close();
});
