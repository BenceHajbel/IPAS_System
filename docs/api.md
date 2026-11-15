# HTTP API (version 2)

[🇭🇺 Magyar](api.hu.md) · [← README](../README.md)

The dashboard and the API are served by the ESP32 at `http://192.168.4.1/`. The dashboard refuses to run against another API version (`api` field) and says so on the page: upload firmware **and** file system together.

| Method and path | Purpose |
|---|---|
| `GET /api/status` | latest reading, sensor state, pressure trend, air quality, clock, storage, history fill, heap |
| `GET /api/history?range=1h\|12h\|24h\|7d` | averaged history points |
| `POST /api/time` body `{"epoch_s": <unix seconds>}` | sets the clock once per boot (ignored if already set) |
| `GET /`, `/app.js`, `/ipas-logic.js`, `/logo.png` | dashboard files from LittleFS |

Unknown paths outside `/api/` are redirected to `/` (captive portal); unknown `/api/` paths answer 404.

## `/api/status` (example)

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

- `sensor.state`: `ok`, `no_data` (initialised, no valid reading yet), `read_failed`, `init_failed`. `last_ok_age_s` is `null` until the first valid reading; the dashboard shows data as **stale** after `stale_after_s`.
- `latest.gas_ohm` is `null` when the gas value is not available.
- `pressure_trend.state`: `insufficient`, `steady`, `rising`, `falling`, `rising_fast`, `falling_fast`; `delta_hpa_3h` is `null` while insufficient. A trend indicator, **not a weather forecast**.
- `air_quality.state`: `ok`, `calibrating`, `unavailable`; `level`: `very_bad`, `bad`, `medium`, `good`, `very_good` (see [air quality](air-quality.md)).
- `time.synced`: whether the browser has set the clock; `now_s` is the timeline "now" (uptime until synced, Unix time afterwards).
- `storage.state`: `ok`, `no_card`, `error`, `disabled`.

## `/api/history?range=1h` (example, shortened)

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

`points` are `[timeline_s, temperature_c, humidity_pct, pressure_hpa, gas_ohm | null]`. A point is placed on the real time line at `(time the response arrived) - (now_s - timeline_s)`. The last point may be the still incomplete interval. While the clock is not set (`time_synced: false`) only data of the current boot is returned.
