# Troubleshooting

[🇭🇺 Magyar](troubleshooting.hu.md) · [← README](../README.md)

Open the serial monitor first (`pio device monitor`, 115200 baud); most problems print a clear line there.

## The page shows the header and logo, but no measurement cards and the buttons do nothing
The dashboard program (`app.js`) did not start. Almost always the **firmware and the file system do not match** (for example only `uploadfs` was run, or the firmware upload failed). Run **both** `pio run -t upload` and `pio run -t uploadfs`, then reload with Ctrl+F5. The page shows a red banner or a version-mismatch banner in this case. Check `http://192.168.4.1/app.js`: it must show JavaScript, not the dashboard.

## `pio run -t upload` fails with `sh: 1: Syntax error: "(" unexpected`
The project folder path contains parentheses or spaces (for example `ipas-esp32(4)` from a browser download). Move the project to a plain path such as `~/ipas` and run the commands there.

## The Wi-Fi network is visible but the phone cannot connect
- Some ESP32-C3 SuperMini boards cope badly with full TX power. In `include/config_local.h` set `#define IPAS_WIFI_TX_POWER WIFI_POWER_8_5dBm` (or lower) and re-upload the firmware.
- To rule out the password, temporarily set `#define IPAS_AP_PASSWORD ""` (open network). Set a password again afterwards.
- Try another USB cable or power adapter, "forget" the network on the phone, and try a second device. The serial log prints `client associated / got an IP address / disconnected` lines.

## The page does not open automatically after joining
Open `http://192.168.4.1/` yourself (type the full address with `http://`). Automatic opening depends on the phone: iOS usually opens a small window, Android often shows a "sign in to network" notification. The serial log prints `portal probe: <host><uri>` for every connectivity check a phone sends; if none appears after joining, the phone did not ask. Turn mobile data off and use a normal browser for long use.

## `BME680 NOT found` / `read failed: beginReading`
- Check wiring and power (3V3, GND, SDA, SCL); the serial log prints an I2C scan. Many boards are fine with CS and SDO unconnected, some are not (see [wiring](wiring.md)).
- Unplug USB completely for a few seconds, then reconnect (a plain reset may not release a stuck bus). After 10 failed readings the firmware re-initialises the sensor and releases the I2C bus itself.
- `invalid values (out of range or gas resistance 0)` means the sensor returned a reading that is not a real measurement; it is discarded.

## `SD: no card / mount failed`
- Insert a **FAT32** card **before** power-up; check VCC (5V for the MICROSD-M, see [wiring](wiring.md)), MISO/MOSI not swapped, short wires.
- The firmware already retries and restarts itself up to twice; `storage.mount_attempts` / `storage.restarts` in `/api/status` show what happened.
- History is restored only for boots during which a browser opened the dashboard (the clock must be set). See [history and SD card](history-and-sd.md).

## The graphs are empty after a restart
- Without SD the history is in RAM and is lost. With SD, only the 12 h / 24 h / 7 d graphs are restored; the 1 h graph is RAM-only and starts empty.
- Open the dashboard once after every power-up so the clock is set.

## Times on the graph are wrong
The clock comes from the browser that opened the dashboard first after the boot. Fix the phone/computer clock and restart the board.

## Air quality shows CALIBRATING
At least 6 hours of gas data are needed; the sensor needs 24-48 hours to settle. See [air quality](air-quality.md).
