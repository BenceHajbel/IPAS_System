# HTTP API (2-es verzió)

[🇬🇧 English](api.md) · [← README](../README.hu.md)

A dashboardot és az API-t az ESP32 szolgálja ki a `http://192.168.4.1/` címen. A dashboard nem fut más API-verzióval (`api` mező), és ezt az oldalon jelzi: a firmware-t **és** a fájlrendszert együtt töltsd fel.

| Metódus és útvonal | Cél |
|---|---|
| `GET /api/status` | legutóbbi mérés, szenzorállapot, nyomástrend, levegőminőség, óra, tárolás, előzmények telítettsége, heap |
| `GET /api/history?range=1h\|12h\|24h\|7d` | átlagolt előzménypontok |
| `POST /api/time` törzs `{"epoch_s": <unix másodperc>}` | indításonként egyszer beállítja az órát (ha már be van állítva, figyelmen kívül hagyja) |
| `GET /`, `/app.js`, `/ipas-logic.js`, `/logo.png` | a dashboard fájljai a LittleFS-ről |

A `/api/` alatt nem szereplő ismeretlen útvonalakat a `/`-re irányítja át (captive portal); az ismeretlen `/api/` útvonalak 404-et adnak.

## `/api/status` (példa)

```json
{
  "api": 2,
  "uptime_s": 10800,
  "stale_after_s": 15,
  "sensor": {
    "state": "ok",
    "i2c_addr": "0x77",
    "last_ok_age_s": 2,
    "reads_ok": 3600,
    "read_errors": 0
  },
  "latest": {
    "temperature_c": 23.46,
    "humidity_pct": 54.2,
    "pressure_hpa": 1008.44,
    "gas_ohm": 123456
  },
  "pressure_trend": {
    "state": "falling",
    "delta_hpa_3h": -2.0,
    "span_s": 10500,
    "points": 36
  },
  "air_quality": {
    "state": "ok",
    "level": "good",
    "score": 76.4,
    "baseline_ohm": 150000,
    "calibration_points": 40,
    "calibration_needed": 12
  },
  "time": {
    "synced": true,
    "now_s": 1793530000
  },
  "storage": {
    "state": "ok",
    "boot": 3,
    "loaded_samples": 118,
    "discarded_unsynced": 0,
    "file_bytes": 21300,
    "write_errors": 0,
    "mount_attempts": 1,
    "restarts": 0
  },
  "history": {
    "minute_count": 120,
    "fine_count": 35,
    "fine_capacity": 288,
    "coarse_count": 5,
    "coarse_capacity": 336
  },
  "system": {
    "heap_free": 0,
    "stations": 0
  }
}
```

- `sensor.state`: `ok`, `no_data` (inicializálva, még nincs érvényes mérés), `read_failed`, `init_failed`. A `last_ok_age_s` az első érvényes mérésig `null`; a dashboard a `stale_after_s` után **elavultnak** jelöli az adatot.
- A `latest.gas_ohm` `null`, ha a gázérték nem elérhető.
- `pressure_trend.state`: `insufficient`, `steady`, `rising`, `falling`, `rising_fast`, `falling_fast`; a `delta_hpa_3h` `null`, amíg nincs elég adat. Trendjelző, **nem időjárás-előrejelzés**.
- `air_quality.state`: `ok`, `calibrating`, `unavailable`; `level`: `very_bad`, `bad`, `medium`, `good`, `very_good` (lásd [levegőminőség](air-quality.hu.md)).
- `time.synced`: beállította-e a böngésző az órát; a `now_s` az idővonal "mostja" (a szinkronig üzemidő, utána Unix-idő).
- `storage.state`: `ok`, `no_card`, `error`, `disabled`.

## `/api/history?range=1h` (példa, rövidítve)

```json
{
  "api": 2,
  "range": "1h",
  "now_s": 10800,
  "time_synced": false,
  "interval_s": 60,
  "points": [
    [
      7229,
      20.0,
      50.0,
      1008.66,
      80000
    ],
    [
      7289,
      20.0,
      50.0,
      1008.65,
      80000
    ],
    [
      7349,
      20.0,
      50.0,
      1008.64,
      80000
    ]
  ]
}
```

A `points` elemei: `[idővonal_s, hőmérséklet_c, páratartalom_pct, légnyomás_hpa, gáz_ohm | null]`. A pont a valós idővonalon `(a válasz megérkezésének ideje) - (now_s - idővonal_s)` helyre kerül. Az utolsó pont a még be nem fejezett intervallum lehet. Amíg az óra nincs beállítva (`time_synced: false`), csak az aktuális indítás adatait adja vissza.
