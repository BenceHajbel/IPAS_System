# IPAS - Intelligent Pollution Analyser System

[🇭🇺 Magyar változat](README.hu.md)

A standalone environmental monitor built around an **ESP32-C3 SuperMini** and a **BME680** sensor. The ESP32 reads the sensor, creates **its own Wi-Fi network**, serves a bilingual (Hungarian / English) **dashboard**, and can store the history on a **microSD card**. No router, internet, server or extra computer is needed.

Developed as a school engineering research project at the Szent Ferenc Katolikus Általános Iskola és Gimnázium (Vecsés, Hungary).

## Features

- Live temperature, relative humidity, atmospheric pressure and gas resistance from the BME680
- Graphs for **1 h, 12 h, 24 h and 7 days**, with missing periods shown as gaps
- Honest states on the page: live, stale, no data, sensor error, no connection; demo data only with `?demo=1` and always labelled - nothing is simulated silently
- **Air-quality rating** (very bad ... very good) from gas resistance and humidity, with the method documented in the info popup ([details](docs/air-quality.md))
- **Pressure trend** over 3 hours (a trend indicator, **not a weather forecast**)
- **microSD logging**: graphs survive power cuts and show a gap while unpowered ([details](docs/history-and-sd.md))
- Own access point `IPAS-Sensor`, dashboard at `http://192.168.4.1/`, captive-portal redirect so phones can offer to open the page after joining
- Hungarian and English interface

## How it works

```
BME680 --I2C--> ESP32-C3 SuperMini --Wi-Fi AP / HTTP--> phone or laptop browser
microSD --SPI-> (sensor loop, history, SD log, web server, dashboard files in LittleFS)
```

## Project status

Be clear about what has and has not been checked on real hardware (one board, one author):

| Verified on hardware | Not yet verified |
|---|---|
| Firmware builds with PlatformIO (espressif32 6.13.0, Arduino core 2.0.17) | Restore of the history after a real power cut with the final firmware |
| BME680 found at 0x77, stable readings | Automatic page opening on phones (captive portal) |
| Wi-Fi access point reachable from a computer and a phone | SD auto-restart / retry behaviour over many boots |
| SD card mounts, 5-minute samples are written to the log | Sensor accuracy against a reference instrument |
| An earlier version (without SD) ran more than 24 h without errors | Air-quality class limits against real events; multi-day soak with SD |

The host-side tests (C++ unit tests, dashboard tests with a mock device, firmware syntax check) pass; they do **not** prove hardware behaviour.

## Quick start

**Hardware:** ESP32-C3 SuperMini, BME680 module (I2C), optional microSD module (SPI). Wiring: [docs/wiring.md](docs/wiring.md).

1. Install [PlatformIO](https://platformio.org/) (VS Code extension or `pip install platformio`).
2. Put the project in a path **without spaces or parentheses**.
3. Optional: copy `include/config_local.example.h` to `include/config_local.h` and set your own values (Wi-Fi password, TX power, pins). That file is git-ignored.
4. Connect the board by USB and run **both** commands:
   ```
   pio run -t upload      # firmware
   pio run -t uploadfs    # dashboard files from data/ -> LittleFS (repeat after changing data/)
   pio device monitor     # serial log, 115200 baud
   ```
   If the upload fails, hold BOOT while plugging in USB.
5. Join the Wi-Fi **`IPAS-Sensor`** (default password **`ipas1234`**, change it) and open **`http://192.168.4.1/`** (type the full address with `http://`). Stay connected if the phone says "no internet".

Insert the SD card (FAT32) **before** powering the board and open the dashboard once after every power-up so the clock is set.

## Configuration

Defaults are in `include/config.h`; put your changes in `include/config_local.h`:

```c
#define IPAS_AP_SSID "IPAS-Sensor"
#define IPAS_AP_PASSWORD "change-me-please"        // 8..63 characters, "" = open network
#define IPAS_WIFI_TX_POWER WIFI_POWER_8_5dBm       // lower it if phones cannot join
#define IPAS_TEMP_OFFSET_C -1.5f                   // calibration offsets
#define IPAS_I2C_SDA 8
#define IPAS_I2C_SCL 9
#define IPAS_SD_SCK 4
#define IPAS_SD_MISO 5
#define IPAS_SD_MOSI 6
#define IPAS_SD_CS 7
```

## Documentation

| Topic | English | Magyar |
|---|---|---|
| Wiring | [wiring.md](docs/wiring.md) | [wiring.hu.md](docs/wiring.hu.md) |
| Air-quality rating | [air-quality.md](docs/air-quality.md) | [air-quality.hu.md](docs/air-quality.hu.md) |
| History, clock, SD card | [history-and-sd.md](docs/history-and-sd.md) | [history-and-sd.hu.md](docs/history-and-sd.hu.md) |
| HTTP API | [api.md](docs/api.md) | [api.hu.md](docs/api.hu.md) |
| Troubleshooting | [troubleshooting.md](docs/troubleshooting.md) | [troubleshooting.hu.md](docs/troubleshooting.hu.md) |

## Tests

```
cd tests/js && npm install && cd ..   # once (jsdom)
make test                              # C++ unit tests + dashboard tests + firmware syntax check
```

They cover the history tiers, the clock conversion, the SD log format and restore (truncated lines, boots without a clock, outage gaps), the trend, air-quality scoring and classes, the JSON output, state classification, and the dashboard against a mock of the device (real `<script src>` loading, all ranges, clock sync, language switch, demo gating). GitHub Actions runs them and builds the firmware on every push.

## Repository layout

```
src/main.cpp          Arduino glue: access point, HTTP server, BME680 loop, SD startup
include/              portable logic (history, trend, air quality, log format, JSON) and Arduino parts (sd_store.h, config.h)
data/                 dashboard served from LittleFS: index.html, app.js, ipas-logic.js, logo.png
tests/                host tests, Arduino API stubs
docs/                 documentation (English + Hungarian)
```

## Known limitations

- No real-time clock: the browser sets the time once per boot (a boot without a browser visit cannot be placed on the time line); an RTC module would remove this.
- History tiers are averages; the 1 h graph is RAM-only; at most 5 minutes before a power cut are lost.
- The gas sensor gives a relative indication, not a calibrated air-quality index; the class limits are the project's own choice.
- Temperature and humidity read slightly off because of self-heating; use the offset settings after comparing with a reference.
- The Wi-Fi access point has no internet access by design (phones may warn about it).

## License and branding

Code and documentation: [MIT License](LICENSE). The school name and the logo in `data/logo.png` belong to the school and are **not** covered by the MIT license; if you fork this project, replace them with your own or make sure you have permission to use them.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) (English and Hungarian).
