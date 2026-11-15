# Wiring

[🇭🇺 Magyar](wiring.hu.md) · [← README](../README.md)

Board: **ESP32-C3-SUPERMINI**. All pins can be changed in `include/config_local.h` (copy `include/config_local.example.h`).

## BME680 (I2C) - verified on the author's board

| BME680-M module pin | ESP32-C3 SuperMini | Note |
|---|---|---|
| VCC | 3V3 | The chip is a 1.71-3.6 V part: **never connect it to 5V** |
| GND | GND | |
| SDA | GPIO8 | `IPAS_I2C_SDA` |
| SCL | GPIO9 | `IPAS_I2C_SCL` |
| CS | not connected | works on this module |
| SDO | not connected | works on this module; the firmware tries addresses 0x77 and 0x76 |

Reported working with the module listed above (pins `SCL, SDA, SDO, CS, VCC, GND`) and an unconnected CS/SDO. **Other breakout boards may differ**: if the module stays in SPI mode, tie `CS` to 3V3; to fix the I2C address tie `SDO` to GND (0x76) or 3V3 (0x77). On many SuperMini boards GPIO8 drives the on-board LED and GPIO9 is the BOOT button; they work as I2C here, but do not hold BOOT while the board starts.

If the sensor is not found, the serial monitor prints an I2C scan (`I2C scan: 0x77 ...` or "no devices answered").

## microSD card module (SPI)

| MICROSD-M pin | ESP32-C3 SuperMini | Config macro |
|---|---|---|
| GND | GND | |
| VCC | **5V** | see below |
| SCK | GPIO4 | `IPAS_SD_SCK` |
| MISO | GPIO5 | `IPAS_SD_MISO` |
| MOSI | GPIO6 | `IPAS_SD_MOSI` |
| CS | GPIO7 | `IPAS_SD_CS` |

- The module listed here is described by its vendor as having an on-board 3.3 V regulator and a 5 V-to-3.3 V level shifter, so **VCC goes to the 5V pin** (USB 5 V). A module **without** regulator/level shifter must be powered from 3V3 instead; check yours before connecting, 5 V can destroy a bare card.
- GPIO4-7 were chosen by the project (free pins, away from the I2C pins). The SD card mounted and 5-minute samples were written with this wiring on the author's board. Other boards: check pins against your own wiring.
- Use a **FAT32** microSD card and insert it **before** powering or resetting the board.
- Keep SPI wires short. A flaky contact is the most common reason for "SD not found".

## Power

The system was run from a 5 V wall adapter through the USB connector. A poor USB cable or a weak port can cause Wi-Fi and sensor problems; try another cable or adapter first.
