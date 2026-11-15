# Bekötés

[🇬🇧 English](wiring.md) · [← README](../README.hu.md)

Panel: **ESP32-C3-SUPERMINI**. Minden láb megváltoztatható az `include/config_local.h` fájlban (másold az `include/config_local.example.h` fájlt).

## BME680 (I2C) – a szerző paneljén igazolt

| BME680-M modul lába | ESP32-C3 SuperMini | Megjegyzés |
|---|---|---|
| VCC | 3V3 | A chip 1,71–3,6 V-os: **soha ne kösd az 5V lábra** |
| GND | GND | |
| SDA | GPIO8 | `IPAS_I2C_SDA` |
| SCL | GPIO9 | `IPAS_I2C_SCL` |
| CS | nincs bekötve | ezen a modulon működik |
| SDO | nincs bekötve | ezen a modulon működik; a firmware a 0x77 és a 0x76 címet is kipróbálja |

A fenti modullal (lábak: `SCL, SDA, SDO, CS, VCC, GND`) és bekötetlen CS/SDO lábbal működőnek jelezték. **Más modulokon ez eltérhet**: ha a modul SPI-módban marad, kösd a `CS`-t a 3V3-ra; az I2C-cím rögzítéséhez az `SDO` GND-re (0x76) vagy 3V3-ra (0x77) köthető. Sok SuperMini panelen a GPIO8 a beépített LED-et hajtja, a GPIO9 pedig a BOOT gomb; I2C-ként itt működnek, de indításkor ne tartsd lenyomva a BOOT gombot.

Ha a szenzor nem található, a soros monitor kiírja az I2C-szkennelés eredményét (`I2C scan: 0x77 ...` vagy "no devices answered").

## microSD kártya modul (SPI)

| MICROSD-M láb | ESP32-C3 SuperMini | Konfigurációs makró |
|---|---|---|
| GND | GND | |
| VCC | **5V** | lásd lent |
| SCK | GPIO4 | `IPAS_SD_SCK` |
| MISO | GPIO5 | `IPAS_SD_MISO` |
| MOSI | GPIO6 | `IPAS_SD_MOSI` |
| CS | GPIO7 | `IPAS_SD_CS` |

- A forgalmazó leírása szerint ennek a modulnak beépített 3,3 V-os stabilizátora és 5 V–3,3 V szintillesztője van, ezért a **VCC az 5V lábra** megy (USB 5 V). Az olyan modult, amelyen **nincs** stabilizátor és szintillesztő, a 3V3 lábról kell táplálni; bekötés előtt ellenőrizd a sajátodat, mert az 5 V tönkreteheti a kártyát.
- A GPIO4–7 kiosztást a projekt választotta (szabad lábak, az I2C-lábaktól távol). A szerző paneljén ezzel a bekötéssel az SD-kártya felcsatolódott, és az 5 perces minták íródtak. Más panelen ellenőrizd a lábakat a saját bekötésed szerint.
- **FAT32** formátumú microSD-kártyát használj, és a panel bekapcsolása vagy újraindítása **előtt** told be.
- Tartsd rövidnek az SPI-vezetékeket. A laza érintkezés a leggyakoribb oka az "SD nem található" hibának.

## Táp

A rendszer 5 V-os fali adapterről, az USB-csatlakozón át futott. Rossz USB-kábel vagy gyenge port Wi-Fi- és szenzorhibákat okozhat; először próbálj másik kábelt vagy adaptert.
