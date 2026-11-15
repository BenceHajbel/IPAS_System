// IPAS - Intelligent Pollution Analyser System
// ESP32-C3-SUPERMINI: BME680 acquisition + Wi-Fi access point + HTTP server + dashboard.
// No router, internet or other computer is required.

#include <Adafruit_BME680.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DNSServer.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <Wire.h>
#include <esp_timer.h>

#include "air_quality.h"
#include "config.h"
#include "history.h"
#include "json_api.h"
#include "sd_store.h"
#include "trend.h"

using namespace ipas;

static Adafruit_BME680 bme(&Wire);
static WebServer server(80);
static DNSServer dns;
static History history;
static SdStore sd;

// Survives a software restart (not a power cycle): counts automatic restarts after a failed SD mount.
RTC_NOINIT_ATTR static uint32_t g_rtc_magic;
RTC_NOINIT_ATTR static uint32_t g_sd_restarts;
#define IPAS_RTC_MAGIC 0x49504153u

// Timeline: uptime seconds until the browser sets the clock, Unix time afterwards.
static bool g_time_synced = false;
static uint32_t g_time_offset = 0;

// ---- sensor state ---------------------------------------------------------
struct SensorCtl {
  SensorState state = SensorState::InitFailed;
  uint8_t addr = 0;
  uint32_t consecutive_fail = 0;
  uint32_t next_init_ms = 0;
  uint32_t next_read_ms = 0;
  uint32_t ready_ms = 0;
  bool reading = false;
  bool has_ok = false;
  uint32_t last_ok_s = 0;
  uint32_t reads_ok = 0;
  uint32_t read_errors = 0;
  Reading latest = {0, 0, 0, 0};
} sensor;

static uint32_t uptimeS() { return (uint32_t)(esp_timer_get_time() / 1000000ULL); }
static uint32_t timelineNow() { return uptimeS() + (g_time_synced ? g_time_offset : 0); }

static void scanI2C() {
  Serial.print("[IPAS] I2C scan:");
  bool any = false;
  for (uint8_t a = 1; a < 127; ++a) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) { Serial.printf(" 0x%02X", a); any = true; }
  }
  Serial.println(any ? "" : " no devices answered (check wiring, power, SDA/SCL pins)");
}

// A sensor that lost its clock can hold SDA low; nine SCL pulses release it (standard I2C bus recovery).
static void i2cBusClear() {
  pinMode(IPAS_I2C_SDA, INPUT_PULLUP);
  pinMode(IPAS_I2C_SCL, OUTPUT);
  for (int i = 0; i < 9; ++i) {
    digitalWrite(IPAS_I2C_SCL, LOW);  delayMicroseconds(10);
    digitalWrite(IPAS_I2C_SCL, HIGH); delayMicroseconds(10);
  }
}

static bool tryInitSensor() {
  Wire.end();
  i2cBusClear();
  Wire.begin(IPAS_I2C_SDA, IPAS_I2C_SCL);
  Wire.setClock(IPAS_I2C_FREQ_HZ);
  const uint8_t addrs[] = {0x77, 0x76};
  for (uint8_t a : addrs) {
    if (bme.begin(a)) {
      bme.setTemperatureOversampling(BME680_OS_8X);
      bme.setHumidityOversampling(BME680_OS_2X);
      bme.setPressureOversampling(BME680_OS_4X);
      bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
      bme.setGasHeater(320, 150);  // 320 C for 150 ms
      sensor.addr = a;
      sensor.state = SensorState::NoData;
      sensor.consecutive_fail = 0;
      Serial.printf("[IPAS] BME680 found at 0x%02X (SDA=GPIO%d SCL=GPIO%d)\n", a, IPAS_I2C_SDA, IPAS_I2C_SCL);
      return true;
    }
  }
  sensor.addr = 0;
  sensor.state = SensorState::InitFailed;
  Serial.printf("[IPAS] BME680 NOT found (SDA=GPIO%d SCL=GPIO%d)\n", IPAS_I2C_SDA, IPAS_I2C_SCL);
  scanI2C();
  return false;
}

static void noteFailure(const char* why) {
  ++sensor.read_errors;
  ++sensor.consecutive_fail;
  sensor.reading = false;
  sensor.next_read_ms = millis() + IPAS_SAMPLE_PERIOD_MS;
  Serial.printf("[IPAS] read failed: %s (%u in a row)\n", why, (unsigned)sensor.consecutive_fail);
  if (sensor.consecutive_fail >= IPAS_REINIT_AFTER_FAILS) {
    sensor.state = SensorState::InitFailed;  // re-initialise I2C + sensor
    sensor.next_init_ms = millis() + 2000;
  } else if (sensor.consecutive_fail >= 3) {
    sensor.state = SensorState::ReadFailed;
  }
}

static void sensorTick() {
  const uint32_t now = millis();
  if (sensor.state == SensorState::InitFailed) {
    if ((int32_t)(now - sensor.next_init_ms) >= 0) {
      if (!tryInitSensor()) sensor.next_init_ms = now + 10000;
      else sensor.next_read_ms = now;
    }
    return;
  }
  if (!sensor.reading && (int32_t)(now - sensor.next_read_ms) >= 0) {
    uint32_t end = bme.beginReading();  // non-blocking; returns when the result is due
    if (end == 0) { noteFailure("beginReading"); return; }
    sensor.ready_ms = end;
    sensor.reading = true;
  }
  if (sensor.reading && (int32_t)(now - sensor.ready_ms) >= 0) {
    sensor.reading = false;
    sensor.next_read_ms = now + IPAS_SAMPLE_PERIOD_MS;
    if (!bme.endReading()) { noteFailure("endReading"); return; }
    Reading r;
    r.temperature_c = bme.temperature + IPAS_TEMP_OFFSET_C;
    r.humidity_pct = bme.humidity + IPAS_HUM_OFFSET_PCT;
    r.pressure_hpa = bme.pressure / 100.0f;
    r.gas_ohm = bme.gas_resistance;
    if (!acceptReading(r)) { noteFailure("invalid values (out of range or gas resistance 0)"); return; }
    sensor.consecutive_fail = 0;
    sensor.state = SensorState::Ok;
    sensor.has_ok = true;
    sensor.last_ok_s = uptimeS();
    sensor.latest = r;
    ++sensor.reads_ok;
    Sample fine;
    if (history.add(timelineNow(), r, &fine)) {
      sd.logSample(fine);
      const StorageInfo& si = sd.info();
      Serial.printf("[IPAS] 5-min sample -> SD: %s, log %u bytes, write errors %u\n", storageStateName(si.state),
                    (unsigned)si.file_bytes, (unsigned)si.write_errors);
    }
    Serial.printf("[IPAS] T=%.2fC RH=%.1f%% P=%.2fhPa gas=%.0fohm\n", r.temperature_c, r.humidity_pct, r.pressure_hpa, r.gas_ohm);
  }
}

// ---- HTTP -------------------------------------------------------------------
static void sendJson(const std::string& body) {
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", body.c_str());
}

static void serveFile(const char* path, const char* mime, const char* cache) {
  File f = LittleFS.open(path, "r");
  if (!f) {
    server.send(503, "text/plain",
                "IPAS: dashboard files missing in flash. Run: pio run -t uploadfs\n");
    return;
  }
  server.sendHeader("Cache-Control", cache);
  server.streamFile(f, mime);
  f.close();
}

static void handleStatus() {
  DeviceSnapshot d;
  d.uptime_s = uptimeS();
  d.sensor = sensor.state;
  d.i2c_addr = sensor.addr;
  d.last_ok_age_s = sensor.has_ok ? (int32_t)(d.uptime_s - sensor.last_ok_s) : -1;
  d.read_errors = sensor.read_errors;
  d.reads_ok = sensor.reads_ok;
  d.has_latest = sensor.has_ok;
  d.latest = sensor.latest;
  d.heap_free = ESP.getFreeHeap();
  d.stations = WiFi.softAPgetStationNum();
  d.stale_after_s = IPAS_STALE_AFTER_S;
  d.time_synced = g_time_synced;
  d.now_s = timelineNow();
  d.storage = sd.info();
  sendJson(statusJson(d, history, computeTrend(history, d.now_s),
                      computeAirQuality(history, sensor.has_ok, sensor.latest, d.now_s)));
}

static void handleHistory() {
  const String r = server.hasArg("range") ? server.arg("range") : String("24h");
  HistoryRange range;
  if (r == "1h") range = HistoryRange::Hour;
  else if (r == "12h") range = HistoryRange::HalfDay;
  else if (r == "24h") range = HistoryRange::Day;
  else if (r == "7d") range = HistoryRange::Week;
  else {
    server.send(400, "application/json", "{\"error\":\"range must be 1h, 12h, 24h or 7d\"}");
    return;
  }
  sendJson(historyJson(history, range, timelineNow(), g_time_synced));
}

// POST /api/time  {"epoch_s": <unix seconds>}  - the dashboard sends the browser clock once per boot.
// The ESP32 has no real-time clock; this places the history on the real time line.
static void handleTime() {
  const String body = server.arg("plain");
  const char* p = strstr(body.c_str(), "epoch_s");
  p = p ? strchr(p, ':') : nullptr;
  const unsigned long long epoch = p ? strtoull(p + 1, nullptr, 10) : 0;
  if (epoch < 1700000000ULL || epoch > 4000000000ULL) {
    server.send(400, "application/json", "{\"error\":\"epoch_s out of range\"}");
    return;
  }
  if (g_time_synced) {
    server.send(200, "application/json", "{\"synced\":true,\"changed\":false}");
    return;
  }
  const uint32_t up = uptimeS();
  const uint32_t offset = (uint32_t)epoch - up;
  Sample flushed;
  const bool has = history.rebase(offset, &flushed);
  g_time_offset = offset;
  g_time_synced = true;
  if (has) sd.logSample(flushed);          // still uptime based; the sync record below places it
  sd.logSync(up, (uint32_t)epoch);
  Serial.printf("[IPAS] clock set from browser: %llu\n", epoch);
  server.send(200, "application/json", "{\"synced\":true,\"changed\":true}");
}

// Phones, laptops and browsers request well-known URLs right after joining a Wi-Fi network to find out
// whether it has internet. Any answer other than their expected one (204 / "Success" / ...) makes them
// treat the network as a login portal and open the page we redirect to. The DNS catch-all (see setup)
// makes those requests arrive here.
static void redirectToDashboard() {
  Serial.printf("[IPAS] portal probe: %s%s\n", server.hostHeader().c_str(), server.uri().c_str());
  server.sendHeader("Cache-Control", "no-store");
  server.sendHeader("Location", "http://192.168.4.1/", true);
  server.send(302, "text/plain", "");
}

static void handleNotFound() {
  if (server.uri().startsWith("/api/")) {
    server.send(404, "application/json", "{\"error\":\"not found\"}");
    return;
  }
  redirectToDashboard();
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n[IPAS] boot");

  const bool fs_ok = LittleFS.begin(false);
  Serial.println(fs_ok ? "[IPAS] LittleFS mounted" : "[IPAS] LittleFS mount FAILED - run: pio run -t uploadfs");

  // Log phones joining/leaving (helps to see whether a phone reached the access point at all).
  WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t) {
    switch (event) {
      case ARDUINO_EVENT_WIFI_AP_STACONNECTED: Serial.println("[IPAS] Wi-Fi: client associated"); break;
      case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED: Serial.println("[IPAS] Wi-Fi: client got an IP address"); break;
      case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED: Serial.println("[IPAS] Wi-Fi: client disconnected"); break;
      default: break;
    }
  });

  const IPAddress ip(IPAS_AP_IP), mask(255, 255, 255, 0);
  WiFi.onEvent([](arduino_event_id_t ev) {
    switch (ev) {
      case ARDUINO_EVENT_WIFI_AP_STACONNECTED: Serial.println("[IPAS] Wi-Fi: client associated"); break;
      case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED: Serial.println("[IPAS] Wi-Fi: client got an IP address"); break;
      case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED: Serial.println("[IPAS] Wi-Fi: client disconnected"); break;
      default: break;
    }
  });
  WiFi.mode(WIFI_AP);
  WiFi.setTxPower(IPAS_WIFI_TX_POWER);  // low power first: some SuperMini boards fail at full power
  WiFi.softAPConfig(ip, ip, mask);
  const bool open_net = strlen(IPAS_AP_PASSWORD) == 0;
  const bool ap_ok = WiFi.softAP(IPAS_AP_SSID, open_net ? nullptr : IPAS_AP_PASSWORD, IPAS_AP_CHANNEL, 0, IPAS_AP_MAX_CLIENTS);
  WiFi.setTxPower(IPAS_WIFI_TX_POWER);
  Serial.printf("[IPAS] AP \"%s\" %s, %s -> http://%s/\n", IPAS_AP_SSID, open_net ? "(open)" : "(WPA2)",
                ap_ok ? "started" : "FAILED", WiFi.softAPIP().toString().c_str());

#if IPAS_CAPTIVE_DNS
  dns.start(53, "*", ip);
#endif

  server.on("/", HTTP_GET, [] { serveFile("/index.html", "text/html", "no-cache"); });
  server.on("/index.html", HTTP_GET, [] { serveFile("/index.html", "text/html", "no-cache"); });
  server.on("/app.js", HTTP_GET, [] { serveFile("/app.js", "application/javascript", "no-cache"); });
  server.on("/ipas-logic.js", HTTP_GET, [] { serveFile("/ipas-logic.js", "application/javascript", "no-cache"); });
  server.on("/logo.png", HTTP_GET, [] { serveFile("/logo.png", "image/png", "max-age=86400"); });
  server.on("/api/status", HTTP_GET, handleStatus);
  server.on("/api/history", HTTP_GET, handleHistory);
  server.on("/api/time", HTTP_POST, handleTime);
  // connectivity-check URLs: Android, iOS/macOS, Windows, Firefox, Samsung
  static const char* const kProbes[] = {"/generate_204", "/gen_204", "/hotspot-detect.html", "/library/test/success.html",
                                        "/connecttest.txt", "/ncsi.txt", "/redirect", "/canonical.html", "/success.txt",
                                        "/check_network_status.txt", "/fwlink"};
  for (const char* uri : kProbes) server.on(uri, HTTP_GET, redirectToDashboard);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.printf("[IPAS] web server started, captive DNS %s\n", IPAS_CAPTIVE_DNS ? "ON" : "OFF");

  // SD card after the access point is up, so the network appears as early as possible.
  if (g_rtc_magic != IPAS_RTC_MAGIC) { g_rtc_magic = IPAS_RTC_MAGIC; g_sd_restarts = 0; }   // power-on: RTC memory is random
  sd.setRestarts(g_sd_restarts);
  const bool sd_ok = sd.begin(history);
  if (sd_ok) {
    g_sd_restarts = 0;
  } else if (IPAS_SD_ENABLED && g_sd_restarts < IPAS_SD_AUTO_RESTARTS) {
    ++g_sd_restarts;
    Serial.printf("[IPAS] SD not found - automatic restart %u of %u (a restart often finds the card)\n",
                  (unsigned)g_sd_restarts, (unsigned)IPAS_SD_AUTO_RESTARTS);
    delay(300);
    ESP.restart();
  }
  sd.setRestarts(g_sd_restarts);

  sensor.next_init_ms = millis();
  Serial.printf("[IPAS] free heap %u bytes\n", (unsigned)ESP.getFreeHeap());
}

void loop() {
  sensorTick();
  sd.tick();
#if IPAS_CAPTIVE_DNS
  dns.processNextRequest();
#endif
  server.handleClient();
  delay(2);
}
