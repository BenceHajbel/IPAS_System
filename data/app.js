/* =============================================================
   IPAS DASHBOARD - live data from the ESP32-C3 (same origin)
   GET /api/status   latest reading, sensor state, pressure trend
   GET /api/history  ?range=24h|7d  averaged history points
   Nothing is simulated unless the page is opened with ?demo=1,
   which shows a permanent DEMO banner.
============================================================= */
const L = window.IPASLogic;
const DEMO = new URLSearchParams(location.search).get('demo') === '1';
const POLL_STATUS_MS = 2000;
const POLL_HISTORY_MS = 30000;
const RANGES = ['1h', '12h', '24h', '7d'];
const EXPECTED_API = 2;   // must match kApiVersion in the firmware (include/json_api.h)

const translations = {
  hu: {
    school_name: "Szent Ferenc Katolikus Általános Iskola és Gimnázium",
    product_full: "Intelligens Levegőszennyezettség-elemző Rendszer",
    online: "ONLINE",
    offline: "OFFLINE",
    s1_title: "Aktuális Állapot",
    s2_title: "Mérési Előzmények",
    s3_title: "Légnyomás & Időjárási Trend",
    time_range: "Időtartomány",
    measurement: "Mérés",
    m_temperature: "HŐMÉRSÉKLET",
    m_humidity: "PÁRATARTALOM",
    m_pressure: "LÉGNYOMÁS",
    m_airquality: "LEVEGŐMINŐSÉG",
    air_pressure: "LÉGNYOMÁS",
    weather_trend: "IDŐJÁRÁSI TREND",
    deteriorating_weather: "Csökkenő légnyomás-trend",
    improving_weather: "Emelkedő légnyomás-trend",
    stable_weather: "Stabil légnyomás-trend",
    pressure_note_falling: "A légnyomás csökken.",
    pressure_note_rising: "A légnyomás emelkedik.",
    pressure_note_stable: "A légnyomás változása kicsi.",
    weather_desc_falling: "A tartós légnyomáscsökkenés időjárási rendszer vagy front közeledésére utalhat.",
    weather_desc_rising: "A tartós légnyomásemelkedés az időjárás javulásával lehet összefüggésben.",
    weather_desc_stable: "A kis légnyomásváltozás stabilabb időjárási helyzetre utalhat.",
    pressure_disclaimer: "Ez légnyomás-alapú trendjelzés, nem biztos időjárás-előrejelzés.",
    pressure_chart_title: "LÉGNYOMÁS — TREND",           
    st_stale: "ELAVULT ADAT",
    st_no_data: "NINCS ADAT",
    st_sensor_error: "SZENZORHIBA",
    st_unreachable: "NINCS KAPCSOLAT",
    st_demo: "DEMÓ ADAT",
    banner_stale: "A legutóbbi érvényes mérés {s} másodperce készült, az értékek elavultak.",
    banner_no_data: "Várakozás az első érvényes mérésre…",
    banner_sensor_error: "A BME680 szenzor nem válaszol. Ellenőrizd a bekötést (SDA/SCL, tápfeszültség).",
    banner_unreachable: "Az eszköz nem érhető el. Ellenőrizd, hogy az IPAS Wi-Fi hálózathoz vagy csatlakozva.",
    banner_demo: "DEMÓ MÓD: a megjelenített adatok szimuláltak, NEM valódi mérések.",
    graph_empty: "Ehhez az időtartományhoz még nincs mért adat.",
    history_note: "Az előzmények az eszköz memóriájában vannak, újraindításkor vagy áramszünetkor elvesznek.",
    history_coverage: "Elérhető előzmény: {d}",
    pressure_note_insufficient: "Nincs elég adat a trend meghatározásához.",
    trend_falling_fast: "Gyorsan csökkenő légnyomás",
    trend_rising_fast: "Gyorsan emelkedő légnyomás",
    weather_desc_falling_fast: "Gyors légnyomáscsökkenés: erősebb időjárási változásra utalhat, de nem előrejelzés.",
    weather_desc_rising_fast: "Gyors légnyomás-emelkedés: gyors időjárási változásra utalhat, de nem előrejelzés.",
    trend_insufficient_title: "Még nincs elég adat",
    trend_insufficient_desc: "A trendhez legalább 1 óra mérési adat kell (jelenleg: {m} perc).",
    banner_version: "A firmware és a weboldal verziója nem egyezik. Töltsd fel mindkettőt: pio run -t upload, majd pio run -t uploadfs.",
    aq_very_bad: "NAGYON ROSSZ",
    aq_bad: "ROSSZ",
    aq_medium: "KÖZEPES",
    aq_good: "JÓ",
    aq_very_good: "NAGYON JÓ",
    aq_calibrating: "KALIBRÁLÁS",
    aq_detail_ok: "{gas} kΩ · {score}/100 pont",
    aq_detail_calibrating: "{gas} kΩ · {p}/{n} adatpont az alapszinthez",
    aq_scale_title: "Osztályok (pontszám 0–100)",
    aq_desc: "Relatív levegőminőség-értékelés a BME680 gázellenállásából és a páratartalomból; nem Bosch IAQ és nem kalibrált index. Pontszám = 75 × min(1; gázellenállás / alapszint) + 25 × páratartalom-tag (40 %RH-nál 25, 0 és 100 %RH felé 0-ra csökken). Alapszint: a tárolt 30 perces gázellenállás-átlagok 95. percentilise (legfeljebb 7 nap; legalább 6 óra adat kell hozzá, addig KALIBRÁLÁS látszik). A módszer a Pimoroni BME680-példaprogram elvén alapul; az osztályhatárok ennek a projektnek saját választásai, nem szabvány. A Bosch hivatalos IAQ-skálája (0–500) a zárt BSEC könyvtárat igényli, ezért itt nincs használva.",
    sd_info: "SD: {n} minta visszatöltve indításkor · napló: {kb} kB · írási hiba: {e} · csatlakozási kísérlet: {a}",
    history_note_sd: "Az előzmények az SD-kártyára is mentődnek; a hiányzó időszak (pl. áramszünet) szünetként látszik a grafikonon.",
    history_note_ram: "Nincs SD-kártya: az előzmények csak az eszköz memóriájában vannak, újraindításkor vagy áramszünetkor elvesznek.",
    history_note_sd_error: "SD-kártya hiba: az új mérések nem mentődnek.",
    history_note_1h: "Az 1 órás nézet csak a memóriából készül, újraindítás után üres.",
    history_note_unsynced: "Az óra beállítása folyamatban…"
  },
  en: {
    school_name: "Saint Francis Catholic Primary School and High School",
    product_full: "Intelligent Pollution Analyser System",
    online: "ONLINE",
    offline: "OFFLINE",
    s1_title: "Current Conditions",
    s2_title: "Measurement History",
    s3_title: "Air Pressure & Weather Trend",
    time_range: "Time range",
    measurement: "Measurement",
    m_temperature: "TEMPERATURE",
    m_humidity: "HUMIDITY",
    m_pressure: "PRESSURE",
    m_airquality: "AIR QUALITY",
    air_pressure: "AIR PRESSURE",
    weather_trend: "WEATHER TREND",
    deteriorating_weather: "Falling pressure trend",
    improving_weather: "Rising pressure trend",
    stable_weather: "Steady pressure trend",
    pressure_note_falling: "Atmospheric pressure is falling.",
    pressure_note_rising: "Atmospheric pressure is rising.",
    pressure_note_stable: "Atmospheric pressure change is small.",
    weather_desc_falling: "A sustained pressure decrease may indicate an approaching weather system or front.",
    weather_desc_rising: "A sustained pressure increase may be associated with improving weather.",
    weather_desc_stable: "A small pressure change may indicate a more stable weather pattern.",
    pressure_disclaimer: "This is a pressure-based trend indication, not a guaranteed weather forecast.",
    pressure_chart_title: "AIR PRESSURE — TREND",           
    st_stale: "STALE DATA",
    st_no_data: "NO DATA",
    st_sensor_error: "SENSOR ERROR",
    st_unreachable: "NO CONNECTION",
    st_demo: "DEMO DATA",
    banner_stale: "The last valid measurement is {s} seconds old; values are stale.",
    banner_no_data: "Waiting for the first valid measurement…",
    banner_sensor_error: "The BME680 sensor is not responding. Check the wiring (SDA/SCL, power).",
    banner_unreachable: "Cannot reach the device. Check that you are connected to the IPAS Wi-Fi network.",
    banner_demo: "DEMO MODE: the displayed data is simulated, NOT real measurements.",
    graph_empty: "No measured data for this range yet.",
    history_note: "History is kept in the device memory and is lost on restart or power loss.",
    history_coverage: "History available: {d}",
    pressure_note_insufficient: "Not enough data to determine a trend.",
    trend_falling_fast: "Rapidly falling pressure",
    trend_rising_fast: "Rapidly rising pressure",
    weather_desc_falling_fast: "Rapid pressure fall: may indicate a stronger weather change, but it is not a forecast.",
    weather_desc_rising_fast: "Rapid pressure rise: may indicate a quick weather change, but it is not a forecast.",
    trend_insufficient_title: "Not enough data yet",
    trend_insufficient_desc: "At least 1 hour of measurements is needed for a trend (currently: {m} min).",
    banner_version: "Firmware and web page versions do not match. Upload both: pio run -t upload, then pio run -t uploadfs.",
    aq_very_bad: "VERY BAD",
    aq_bad: "BAD",
    aq_medium: "MEDIUM",
    aq_good: "GOOD",
    aq_very_good: "VERY GOOD",
    aq_calibrating: "CALIBRATING",
    aq_detail_ok: "{gas} kΩ · score {score}/100",
    aq_detail_calibrating: "{gas} kΩ · {p}/{n} data points for baseline",
    aq_scale_title: "Classes (score 0–100)",
    aq_desc: "Relative air-quality rating from the BME680 gas resistance and humidity; not Bosch IAQ and not a calibrated index. Score = 75 × min(1, gas resistance / baseline) + 25 × humidity term (25 at 40 %RH, falling to 0 towards 0 and 100 %RH). Baseline: 95th percentile of the stored 30-minute gas resistance averages (up to 7 days; at least 6 hours of data are needed, until then CALIBRATING is shown). The method follows the Pimoroni BME680 example program; the class limits are this project's own choice, not a standard. Bosch's official IAQ scale (0–500) needs the closed-source BSEC library and is not used here.",
    sd_info: "SD: {n} samples restored at start · log: {kb} kB · write errors: {e} · mount attempts: {a}",
    history_note_sd: "History is also saved to the SD card; a missing period (e.g. a power outage) shows as a gap in the graph.",
    history_note_ram: "No SD card: history is kept only in the device memory and is lost on restart or power loss.",
    history_note_sd_error: "SD card error: new measurements are not being saved.",
    history_note_1h: "The 1 hour view comes from memory only and starts empty after a restart.",
    history_note_unsynced: "Setting the clock…"
  }
};

let currentLang = 'hu';

function t(key, vars){
  let s = translations[currentLang][key] || key;
  if (vars) Object.keys(vars).forEach(k => { s = s.replace('{' + k + '}', vars[k]); });
  return s;
}

function setLanguage(lang){
  currentLang = lang;
  document.documentElement.lang = lang;
  document.getElementById('lang-hu').classList.toggle('active', lang === 'hu');
  document.getElementById('lang-en').classList.toggle('active', lang === 'en');
  document.querySelectorAll('[data-i18n]').forEach(el => {
    const key = el.getAttribute('data-i18n');
    if (translations[lang][key]) el.textContent = translations[lang][key];
  });
  renderMeasurementCards();
  renderLive();
  updateGraphTitle();
  drawGraph();
  drawPressureGraph();
}

/* -------------------------------------------------------------
   METRIC DEFINITIONS (labels, units, descriptions)
------------------------------------------------------------- */
const measurementData = {
  temperature: {
    id: 'temperature', label: { hu: 'HŐMÉRSÉKLET', en: 'TEMPERATURE' }, unit: '°C', sensor: 'BME680',
    desc: { hu: 'A szenzor által mért környezeti léghőmérséklet.', en: 'Ambient air temperature measured by the sensor.' }
  },
  humidity: {
    id: 'humidity', label: { hu: 'PÁRATARTALOM', en: 'HUMIDITY' }, unit: '%', sensor: 'BME680',
    desc: { hu: 'Relatív páratartalom a levegőben.', en: 'Relative humidity present in the surrounding air.' }
  },
  pressure: {
    id: 'pressure', label: { hu: 'LÉGNYOMÁS', en: 'PRESSURE' }, unit: 'hPa', sensor: 'BME680',
    desc: { hu: 'Légköri nyomás, mely az időjárási trendek elemzéséhez is felhasználható.', en: 'Atmospheric pressure, also used to analyse local weather trends.' }
  },
  airquality: {
    id: 'airquality', label: { hu: 'LEVEGŐMINŐSÉG', en: 'AIR QUALITY' }, unit: 'kΩ', sensor: 'BME680',
    desc: { hu: '', en: '' }
  }
};

const historySeries = {
  temperature: { unit: '°C' }, humidity: { unit: '%' }, pressure: { unit: 'hPa' }, airquality: { unit: 'kΩ' }
};

/* -------------------------------------------------------------
   LIVE STATE
------------------------------------------------------------- */
let lastStatus = null;          // last successful /api/status response
let statusReceivedAt = 0;       // browser time of that response
let failures = 0;               // consecutive failed polls
let lastUptime = null;
const histories = {};           // range -> { data, receivedAt }
let activeMetric = 'temperature';
let activeRange = '24h';

function view(){
  return L.deriveState({ demo: DEMO, status: lastStatus, receivedAtMs: statusReceivedAt, nowMs: Date.now(), failures: failures });
}

function latestValue(metric){
  if (!lastStatus || !lastStatus.latest) return null;
  const l = lastStatus.latest;
  switch (metric){
    case 'temperature': return l.temperature_c;
    case 'humidity': return l.humidity_pct;
    case 'pressure': return l.pressure_hpa;
    case 'airquality': return l.gas_ohm === null ? null : l.gas_ohm / 1000;
  }
  return null;
}

async function fetchJson(url, timeoutMs){
  const ctl = new AbortController();
  const timer = setTimeout(() => ctl.abort(), timeoutMs);
  try {
    const res = await fetch(url, { cache: 'no-store', signal: ctl.signal });
    if (!res.ok) throw new Error('HTTP ' + res.status);
    return await res.json();
  } finally { clearTimeout(timer); }
}

/* The ESP32 has no real-time clock: the browser sets it once per boot so that the
   (SD-card) history can be placed on the real time line. */
async function syncClock(){
  try {
    await fetch('/api/time', { method: 'POST', headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ epoch_s: Math.floor(Date.now() / 1000) }) });
  } catch (e) { /* retried on the next poll */ }
}

async function pollStatus(){
  try {
    let s = DEMO ? L.demoStatus() : await fetchJson('/api/status', 4000);
    if (!DEMO && s.time && !s.time.synced){
      await syncClock();
      s = await fetchJson('/api/status', 4000);
      Object.keys(histories).forEach(k => delete histories[k]);   // earlier answers used the uptime timeline
      setTimeout(() => RANGES.forEach(r => { if (r === '24h' || r === activeRange) pollHistory(r); }), 0);
    }
    const rebooted = lastUptime !== null && s.uptime_s < lastUptime;
    lastStatus = s; statusReceivedAt = Date.now(); failures = 0; lastUptime = s.uptime_s;
    if (rebooted) { Object.keys(histories).forEach(k => delete histories[k]); pollHistory('24h'); if (activeRange !== '24h') pollHistory(activeRange); }
  } catch (e) {
    failures++;
  }
  renderLive();
}

async function pollHistory(range){
  try {
    const data = DEMO ? L.demoHistory(range) : await fetchJson('/api/history?range=' + range, 8000);
    if (data.api !== EXPECTED_API || typeof data.now_s !== 'number') throw new Error('history format');
    histories[range] = { data: data, receivedAt: Date.now() };
  } catch (e) { /* keep the previous data; the connection banner explains the situation */ }
  if (range === activeRange) drawGraph();
  if (range === '24h') drawPressureGraph();
}

/* -------------------------------------------------------------
   STATUS PILL AND BANNER
------------------------------------------------------------- */
const PILL_TEXT = { live: 'online', stale: 'st_stale', no_data: 'st_no_data', sensor_error: 'st_sensor_error', unreachable: 'st_unreachable', demo: 'st_demo' };
const PILL_CLASS = { live: '', stale: 'warn', no_data: 'warn', demo: 'warn', sensor_error: 'offline', unreachable: 'offline' };
const BANNER_TEXT = { stale: 'banner_stale', no_data: 'banner_no_data', sensor_error: 'banner_sensor_error', unreachable: 'banner_unreachable', demo: 'banner_demo' };

function renderOnlineStatus(v){
  const pill = document.getElementById('online-status');
  pill.classList.remove('offline', 'warn');
  if (PILL_CLASS[v.state]) pill.classList.add(PILL_CLASS[v.state]);
  document.getElementById('online-status-text').textContent = t(PILL_TEXT[v.state]);

  const banner = document.getElementById('status-banner');
  if (lastStatus && lastStatus.api !== EXPECTED_API && !DEMO){
    banner.hidden = false;
    banner.className = 'status-banner offline';
    banner.textContent = t('banner_version');
    return;
  }
  if (v.state === 'live'){ banner.hidden = true; return; }
  banner.hidden = false;
  banner.className = 'status-banner ' + (PILL_CLASS[v.state] || 'warn');
  banner.textContent = t(BANNER_TEXT[v.state], { s: v.ageS === null ? '?' : Math.round(v.ageS) });
}

/* -------------------------------------------------------------
   MEASUREMENT CARDS (built once, values updated in place)
------------------------------------------------------------- */
function renderMeasurementCards(){
  const grid = document.getElementById('measurement-grid');
  grid.innerHTML = '';
  Object.values(measurementData).forEach(m => {
    const card = document.createElement('div');
    card.className = 'measurement-card';
    card.setAttribute('tabindex', '0');
    card.dataset.metric = m.id;
    const label = m.label[currentLang];
    card.innerHTML = `
      <div class="m-label">${label}</div>
      <div class="m-value-row"><span class="m-value">—</span></div>
      <span class="m-unit">${m.unit}</span>
      <div class="m-footer">
        <span class="m-sensor">${m.sensor}</span>
        <button class="info-icon" aria-label="info" type="button">i</button>
      </div>
      <div class="m-tooltip">
        <div class="tt-name">${label}</div>
        <div class="tt-value">—</div>
        <div class="tt-row">Sensor: ${m.sensor}</div>
        <div class="tt-row">Unit: ${m.unit}</div>
        <div class="tt-desc">${m.id === 'airquality' ? t('aq_desc') : m.desc[currentLang]}</div>
        ${m.id === 'airquality' ? aqScaleHtml() : ''}
      </div>`;
    grid.appendChild(card);
  });
  attachTooltipHandlers();
  updateMeasurementValues(view());
}

const AQ_LEVELS = ['very_good', 'good', 'medium', 'bad', 'very_bad'];
const AQ_LIMITS = { very_good: '≥ 90', good: '≥ 75', medium: '≥ 55', bad: '≥ 40', very_bad: '< 40' };

function aqScaleHtml(){
  const rows = AQ_LEVELS.map(k => `<div class="aq-${k}"><span class="sw"></span>${t('aq_' + k)} &nbsp;${AQ_LIMITS[k]}</div>`).join('');
  return `<div class="tt-scale"><div class="tt-row">${t('aq_scale_title')}</div>${rows}</div>`;
}

function updateAirQualityCard(card, v){
  const valueEl = card.querySelector('.m-value');
  const unitEl = card.querySelector('.m-unit');
  const ttValue = card.querySelector('.tt-value');
  AQ_LEVELS.concat(['calibrating']).forEach(k => { valueEl.classList.remove('aq-' + k); unitEl.classList.remove('aq-' + k); });
  valueEl.classList.add('aq-text');
  unitEl.classList.add('aq-detail');
  const aq = v.showValues && lastStatus ? lastStatus.air_quality : null;
  const gas = v.showValues ? latestValue('airquality') : null;
  const gasText = gas === null ? '—' : L.formatValue('airquality', gas, currentLang);
  if (!aq || aq.state === 'unavailable' || gas === null){
    valueEl.textContent = '—'; unitEl.textContent = 'kΩ'; ttValue.textContent = '—';
    return;
  }
  if (aq.state === 'calibrating'){
    valueEl.textContent = t('aq_calibrating');
    valueEl.classList.add('aq-calibrating');
    unitEl.textContent = t('aq_detail_calibrating', { gas: gasText, p: aq.calibration_points, n: aq.calibration_needed });
    ttValue.textContent = t('aq_calibrating');
    return;
  }
  valueEl.textContent = t('aq_' + aq.level);
  valueEl.classList.add('aq-' + aq.level);
  unitEl.textContent = t('aq_detail_ok', { gas: gasText, score: Math.round(aq.score) });
  ttValue.textContent = `${t('aq_' + aq.level)} (${Math.round(aq.score)}/100)`;
}

function updateMeasurementValues(v){
  document.querySelectorAll('.measurement-card').forEach(card => {
    const id = card.dataset.metric;
    const m = measurementData[id];
    if (id === 'airquality'){
      updateAirQualityCard(card, v);
      card.classList.toggle('dim', v.state !== 'live' && v.state !== 'demo');
      return;
    }
    const val = v.showValues ? latestValue(id) : null;
    const text = L.formatValue(id, val, currentLang);
    card.querySelector('.m-value').textContent = text;
    card.querySelector('.tt-value').textContent = val === null ? text : `${text} ${m.unit}`;
    card.classList.toggle('dim', v.state !== 'live' && v.state !== 'demo');
  });
}

/* Hover on desktop, tap/click on touch devices */
function attachTooltipHandlers(){
  const isTouch = window.matchMedia('(hover: none)').matches;
  document.querySelectorAll('.measurement-card').forEach(card => {
    const icon = card.querySelector('.info-icon');
    if (!isTouch){
      card.addEventListener('mouseenter', () => card.classList.add('tooltip-visible'));
      card.addEventListener('mouseleave', () => card.classList.remove('tooltip-visible'));
      card.addEventListener('focusin', () => card.classList.add('tooltip-visible'));
      card.addEventListener('focusout', () => card.classList.remove('tooltip-visible'));
    } else {
      icon.addEventListener('click', (e) => {
        e.stopPropagation();
        const wasVisible = card.classList.contains('tooltip-visible');
        document.querySelectorAll('.measurement-card').forEach(c => c.classList.remove('tooltip-visible'));
        if (!wasVisible) card.classList.add('tooltip-visible');
      });
    }
  });
}
if (window.matchMedia('(hover: none)').matches){
  document.addEventListener('click', () => {
    document.querySelectorAll('.measurement-card').forEach(c => c.classList.remove('tooltip-visible'));
  });
}

/* -------------------------------------------------------------
   HISTORY GRAPH (SVG, true time axis, gaps are shown as gaps)
------------------------------------------------------------- */
const GRAPH_W = 900;
const GRAPH_H = 280;
const PAD = { top: 20, right: 28, bottom: 38, left: 52 };
const SVG_NS = 'http://www.w3.org/2000/svg';

function formatTimestamp(date, range){
  const day = String(date.getDate()).padStart(2, '0');
  const months = currentLang === 'hu'
    ? ['jan','febr','márc','ápr','máj','jún','júl','aug','szept','okt','nov','dec']
    : ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];
  const hh = String(date.getHours()).padStart(2, '0');
  const mm = String(date.getMinutes()).padStart(2, '0');
  return range === '7d' ? `${day} ${months[date.getMonth()]}` : `${hh}:${mm}`;
}

function svgText(svg, x, y, anchor, size, text){
  const el = document.createElementNS(SVG_NS, 'text');
  el.setAttribute('x', x); el.setAttribute('y', y);
  el.setAttribute('text-anchor', anchor);
  el.setAttribute('font-family', 'IBM Plex Mono, monospace');
  el.setAttribute('font-size', size);
  el.setAttribute('fill', 'var(--text-muted)');
  el.textContent = text;
  svg.appendChild(el);
}

function drawLineGraph(svgId, series, stamps, cfg, currentTextId, options = {}){
  const svg = document.getElementById(svgId);
  if (!svg) return;
  svg.innerHTML = '';
  const width = GRAPH_W, height = GRAPH_H, pad = PAD;
  svg.setAttribute('viewBox', `0 0 ${width} ${height}`);
  const range = options.range || activeRange;
  const endMs = options.endMs || Date.now();
  const spanMs = L.RANGE_S[range] * 1000;
  const startMs = endMs - spanMs;
  const plotW = width - pad.left - pad.right;
  const plotH = height - pad.top - pad.bottom;
  const small = window.innerWidth <= 560;

  if (currentTextId){
    const latest = series.length ? series[series.length - 1] : null;
    document.getElementById(currentTextId).textContent = latest === null ? '—' : `${latest} ${cfg.unit}`;
  }
  if (series.length === 0){
    svgText(svg, width / 2, height / 2, 'middle', small ? 11 : 12, t('graph_empty'));
    return;
  }

  let min = Math.min(...series), max = Math.max(...series);
  if (max - min < 0.2){ const mid = (max + min) / 2; min = mid - 0.1; max = mid + 0.1; }
  const vr = max - min;
  const xFor = ms => pad.left + ((ms - startMs) / spanMs) * plotW;
  const yFor = v => pad.top + plotH - ((v - min) / vr) * plotH;

  for (let r = 0; r <= 4; r++){
    const y = pad.top + (r / 4) * plotH;
    const line = document.createElementNS(SVG_NS, 'line');
    line.setAttribute('x1', pad.left); line.setAttribute('x2', width - pad.right);
    line.setAttribute('y1', y); line.setAttribute('y2', y);
    line.setAttribute('stroke', 'rgba(255,255,255,0.06)');
    svg.appendChild(line);
    svgText(svg, pad.left - 9, y + 3, 'end', 10, (max - (r / 4) * vr).toFixed(1));
  }
  L.axisTicks(range, endMs).forEach(ms => {
    svgText(svg, xFor(ms), height - 9, 'middle', small ? 9 : 10, formatTimestamp(new Date(ms), range));
  });

  // split into segments at gaps (missing data is never bridged)
  const maxGapMs = (options.intervalS || 300) * 2.5 * 1000;
  const segs = [];
  let cur = [];
  stamps.forEach((d, i) => {
    if (cur.length && d.getTime() - stamps[i - 1].getTime() > maxGapMs){ segs.push(cur); cur = []; }
    cur.push(i);
  });
  if (cur.length) segs.push(cur);

  segs.forEach(seg => {
    const pts = seg.map(i => [xFor(stamps[i].getTime()), yFor(series[i])]);
    const d = pts.map((p, k) => `${k === 0 ? 'M' : 'L'} ${p[0].toFixed(1)} ${p[1].toFixed(1)}`).join(' ');
    const path = document.createElementNS(SVG_NS, 'path');
    path.setAttribute('d', d); path.setAttribute('fill', 'none');
    path.setAttribute('stroke', 'var(--accent)'); path.setAttribute('stroke-width', '1.75');
    svg.appendChild(path);
    if (pts.length > 1){
      const area = document.createElementNS(SVG_NS, 'path');
      area.setAttribute('d', `${d} L ${pts[pts.length - 1][0].toFixed(1)} ${pad.top + plotH} L ${pts[0][0].toFixed(1)} ${pad.top + plotH} Z`);
      area.setAttribute('fill', 'var(--accent)'); area.setAttribute('opacity', '0.06');
      svg.appendChild(area);
    }
  });

  series.forEach((v, i) => {
    const circle = document.createElementNS(SVG_NS, 'circle');
    circle.setAttribute('cx', xFor(stamps[i].getTime()).toFixed(1));
    circle.setAttribute('cy', yFor(v).toFixed(1));
    circle.setAttribute('r', series.length > 120 ? '2.2' : (small ? '3.5' : '4'));
    circle.classList.add('graph-point');
    if (options.tooltip === true){
      circle.addEventListener('mouseenter', e => showGraphTooltip(e, i, v, stamps[i]));
      circle.addEventListener('mousemove', e => positionGraphTooltip(e));
      circle.addEventListener('mouseleave', hideGraphTooltip);
      circle.addEventListener('click', e => showGraphTooltip(e, i, v, stamps[i]));
    }
    svg.appendChild(circle);
  });
}

function seriesFor(range, metric){
  const h = histories[range];
  if (!h) return null;
  const s = L.buildSeries(h.data, metric, h.receivedAt);
  const dec = metric === 'airquality' ? 1 : 2;
  s.values = s.values.map(v => +v.toFixed(dec));
  return { series: s, intervalS: h.data.interval_s, h: h };
}

function drawGraph(){
  const cfg = historySeries[activeMetric];
  const r = seriesFor(activeRange, activeMetric);
  drawLineGraph('history-graph', r ? r.series.values : [], r ? r.series.stamps : [], cfg, 'graph-current',
    { tooltip: true, range: activeRange, intervalS: r ? r.intervalS : 300 });
  const note = document.getElementById('history-note');
  const parts = [];
  const st = lastStatus && lastStatus.storage ? lastStatus.storage.state : null;
  if (DEMO) parts.push(t('banner_demo'));
  else if (st === 'ok'){
    const sg = lastStatus.storage;
    parts.push(t('history_note_sd'));
    parts.push(t('sd_info', { n: sg.loaded_samples, kb: (sg.file_bytes / 1024).toFixed(1), e: sg.write_errors, a: sg.mount_attempts || 1 }));
  }
  else if (st === 'error') parts.push(t('history_note_sd_error'));
  else parts.push(t('history_note_ram'));
  if (activeRange === '1h') parts.push(t('history_note_1h'));
  if (r && r.h.data.points.length){
    const first = r.h.data.points[0][0];
    const covered = Math.min(r.h.data.now_s - first, L.RANGE_S[activeRange]);
    parts.unshift(t('history_coverage', { d: L.formatDuration(covered, currentLang) }));
  }
  note.textContent = parts.join(' · ');
}

function showGraphTooltip(evt, index, value, date){
  const tooltip = document.getElementById('graph-tooltip');
  const m = measurementData[activeMetric];
  document.getElementById('gt-date').textContent =
    `${formatTimestamp(date, activeRange)} · ${String(date.getHours()).padStart(2,'0')}:${String(date.getMinutes()).padStart(2,'0')}`;
  document.getElementById('gt-name').textContent = m.label[currentLang];
  document.getElementById('gt-value').textContent = `${value} ${historySeries[activeMetric].unit}`;
  document.getElementById('gt-sensor').textContent = `Sensor: ${m.sensor}`;
  document.getElementById('gt-desc').textContent = m.desc[currentLang];
  tooltip.classList.add('visible');
  positionGraphTooltip(evt);
}

function positionGraphTooltip(evt){
  const wrap = document.getElementById('graph-wrap');
  const tooltip = document.getElementById('graph-tooltip');
  const wrapRect = wrap.getBoundingClientRect();
  const tooltipWidth = Math.min(210, window.innerWidth - 32);
  const half = tooltipWidth / 2;
  let x = evt.clientX - wrapRect.left;
  let y = evt.clientY - wrapRect.top - 14;
  x = Math.max(half + 4, Math.min(x, wrapRect.width - half - 4));
  y = Math.max(tooltip.offsetHeight + 8, y);
  tooltip.style.left = `${x}px`;
  tooltip.style.top = `${y}px`;
}

function hideGraphTooltip(){
  document.getElementById('graph-tooltip').classList.remove('visible');
}

function updateGraphTitle(){
  const keyOf = { temperature: 'm_temperature', humidity: 'm_humidity', pressure: 'm_pressure', airquality: 'm_airquality' }[activeMetric];
  const suffix = activeMetric === 'airquality' ? ' (kΩ)' : '';
  document.getElementById('graph-title').textContent = `${translations[currentLang][keyOf]}${suffix} — ${activeRange.toUpperCase()}`;
}

/* -------------------------------------------------------------
   PRESSURE TREND (computed on the device; indicator, not a forecast)
------------------------------------------------------------- */
function renderPressureText(v){
  const tv = v.showValues && lastStatus ? L.trendView(lastStatus.pressure_trend) : { key: 'insufficient', arrow: '→', delta: null };
  const cur = v.showValues ? latestValue('pressure') : null;
  document.getElementById('pressure-current').textContent = L.formatValue('pressure', cur, currentLang);
  document.getElementById('pressure-arrow').textContent = tv.arrow;
  document.getElementById('pressure-change').textContent = tv.delta === null ? '—' : `${L.formatDelta(tv.delta)} hPa / 3h`;

  const span = lastStatus && lastStatus.pressure_trend ? lastStatus.pressure_trend.span_s : 0;
  const k = tv.key;
  const noteKey = { falling: 'pressure_note_falling', falling_fast: 'pressure_note_falling', rising: 'pressure_note_rising',
    rising_fast: 'pressure_note_rising', steady: 'pressure_note_stable', insufficient: 'pressure_note_insufficient' }[k];
  const titleKey = { falling: 'deteriorating_weather', falling_fast: 'trend_falling_fast', rising: 'improving_weather',
    rising_fast: 'trend_rising_fast', steady: 'stable_weather', insufficient: 'trend_insufficient_title' }[k];
  const descKey = { falling: 'weather_desc_falling', falling_fast: 'weather_desc_falling_fast', rising: 'weather_desc_rising',
    rising_fast: 'weather_desc_rising_fast', steady: 'weather_desc_stable', insufficient: 'trend_insufficient_desc' }[k];
  document.getElementById('pressure-note').textContent = t(noteKey);
  document.getElementById('weather-trend-title').textContent = t(titleKey);
  document.getElementById('weather-trend-description').textContent = t(descKey, { m: Math.floor(span / 60) });
}

function drawPressureGraph(){
  const r = seriesFor('24h', 'pressure');
  drawLineGraph('pressure-graph', r ? r.series.values : [], r ? r.series.stamps : [], { unit: 'hPa' },
    'pressure-graph-current', { tooltip: false, range: '24h', intervalS: r ? r.intervalS : 300 });
}

/* -------------------------------------------------------------
   RENDER + WIRING
------------------------------------------------------------- */
function renderLive(){
  const v = view();
  renderOnlineStatus(v);
  updateMeasurementValues(v);
  renderPressureText(v);
}

document.getElementById('range-buttons').addEventListener('click', (e) => {
  const btn = e.target.closest('.ctrl-btn');
  if (!btn) return;
  document.querySelectorAll('#range-buttons .ctrl-btn').forEach(b => b.classList.remove('active'));
  btn.classList.add('active');
  activeRange = btn.dataset.range;
  updateGraphTitle();
  drawGraph();
  pollHistory(activeRange);
});

document.getElementById('metric-buttons').addEventListener('click', (e) => {
  const btn = e.target.closest('.ctrl-btn');
  if (!btn) return;
  document.querySelectorAll('#metric-buttons .ctrl-btn').forEach(b => b.classList.remove('active'));
  btn.classList.add('active');
  activeMetric = btn.dataset.metric;
  updateGraphTitle();
  drawGraph();
});

let resizeTimer;
window.addEventListener('resize', () => {
  clearTimeout(resizeTimer);
  resizeTimer = setTimeout(() => { drawGraph(); drawPressureGraph(); }, 100);
});

function init(){
  renderMeasurementCards();
  renderLive();
  updateGraphTitle();
  drawGraph();
  drawPressureGraph();
  window.__ipasStarted = true;
  pollStatus();
  pollHistory('24h');
  setInterval(pollStatus, POLL_STATUS_MS);
  setInterval(() => { pollHistory('24h'); if (activeRange !== '24h') pollHistory(activeRange); }, POLL_HISTORY_MS);
}

init();
