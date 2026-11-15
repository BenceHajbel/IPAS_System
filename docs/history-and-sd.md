# History, clock and SD card

[🇭🇺 Magyar](history-and-sd.hu.md) · [← README](../README.md)

## Three history tiers

| Tier | Resolution | Capacity | Stored on SD | Used for |
|---|---|---|---|---|
| minute | 1 min | 120 (2 h) | no (RAM only) | 1 h graph |
| fine | 5 min | 288 (24 h) | yes | 12 h and 24 h graphs |
| coarse | 30 min | 336 (7 d) | rebuilt from the fine samples | 7 d graph |

All tiers are averages of the 3-second readings, kept in ring buffers (about 12 kB of RAM). Readings that fail validation (out of range or gas resistance 0) never enter the history. Missing periods are drawn as **gaps**.

Without an SD card (or after a card error) the history is in RAM only and **is lost on restart or power loss**; the dashboard says which case applies.

## The clock (no RTC)

The ESP32 has no real-time clock. Samples carry **timeline seconds**: uptime until the browser has set the clock, Unix time afterwards. Every time the dashboard is opened while the device clock is not set, it sends the browser time once (`POST /api/time`). The firmware then converts all samples of the current boot, including those measured earlier in the same boot, to real time.

Consequences:
- A boot during which **no browser connected** cannot be placed on the time line and is skipped when restoring after the next boot (`storage.discarded_unsynced` counts such samples).
- Times are only as correct as the phone/computer clock that set them; the device crystal drifts by seconds per day.
- An RTC module (for example DS3231) would remove this limitation.

## SD log

File `/ipas_log.csv` (FAT32), text, one record per line. Every line ends with `*`, so a line cut short by a power loss is ignored when loading.

```
B,<boot>*                                                  a boot started
S,<boot>,<t>,<temp_c100>,<hum_c100>,<press_pa>,<gas_ohm>*  one 5-minute sample
T,<boot>,<uptime_s>,<unix_s>*                              the browser set the clock during this boot
```

Samples written before the clock was set carry uptime (< 1,000,000,000); the following `T` record gives the offset. The log grows about 15 kB per day. At boot the newest 256 kB are read and the 12 h / 24 h / 7 d tiers are rebuilt; power-off periods stay gaps. At most the last 5 minutes before a power cut are lost.

## Mounting the card

A reset can leave a card in the middle of a command, and the module's regulator needs a moment after power-up. The firmware therefore:

1. tries up to **6 times** per boot, alternating 4 MHz and 1 MHz SPI, with a resynchronisation pulse train before each try;
2. restarts itself, **at most 2 times in a row**, if the card is still not found (a restart often finds it), then continues without SD;
3. after a write error retries mounting every 30 s.

`GET /api/status` -> `storage.mount_attempts` and `storage.restarts` show what was needed. Insert the card **before** power-up; hot-plugging a card into a running board is not supported.

Tuning: `IPAS_SD_MOUNT_ATTEMPTS`, `IPAS_SD_AUTO_RESTARTS`, `IPAS_SD_FREQ_HZ`, `IPAS_SD_FREQ_FALLBACK_HZ` in `include/config.h`.
