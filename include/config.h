// IPAS configuration. Every value can be overridden with -D flags in platformio.ini.
#pragma once

// Personal overrides (Wi-Fi password, TX power, pins, ...) go into include/config_local.h
// (see config_local.example.h). That file is not part of the project updates, so your
// settings survive when this file is replaced.
#if defined(__has_include)
#if __has_include("config_local.h")
#include "config_local.h"
#endif
#endif

// ---- Wi-Fi access point ------------------------------------------------
#ifndef IPAS_AP_SSID
#define IPAS_AP_SSID "IPAS-Sensor"
#endif
// WPA2 password: 8..63 characters. Use "" for an OPEN network (no password).
#ifndef IPAS_AP_PASSWORD
#define IPAS_AP_PASSWORD "ipas1234"
#endif
#ifndef IPAS_AP_CHANNEL
#define IPAS_AP_CHANNEL 1
#endif
#ifndef IPAS_AP_MAX_CLIENTS
#define IPAS_AP_MAX_CLIENTS 4
#endif
// Fixed address of the dashboard: http://192.168.4.1/
#define IPAS_AP_IP 192, 168, 4, 1
// Some ESP32-C3 SuperMini clones have Wi-Fi trouble at full TX power.
// Lower it if phones cannot join or keep dropping (see README, hardware checks).
#ifndef IPAS_WIFI_TX_POWER
#define IPAS_WIFI_TX_POWER WIFI_POWER_8_5dBm
#endif
// 1 = answer every DNS name with the AP address so any URL (and phone
// "sign in to network" prompts) lead to the dashboard.
#ifndef IPAS_CAPTIVE_DNS
#define IPAS_CAPTIVE_DNS 1
#endif

// ---- I2C wiring to the BME680 -------------------------------------------
// GPIO8 = SDA, GPIO9 = SCL: confirmed working on the author's ESP32-C3 SuperMini
// with a BME680-M module. Confirm them against YOUR wiring and override with -DIPAS_I2C_SDA / -DIPAS_I2C_SCL. GPIO8 also drives the
// on-board LED and GPIO9 is the BOOT button on many SuperMini boards.
#ifndef IPAS_I2C_SDA
#define IPAS_I2C_SDA 8
#endif
#ifndef IPAS_I2C_SCL
#define IPAS_I2C_SCL 9
#endif
#define IPAS_I2C_FREQ_HZ 100000   // conservative; breadboard wiring friendly

// ---- Sensor behaviour -----------------------------------------------------
#define IPAS_SAMPLE_PERIOD_MS 3000      // one BME680 measurement every 3 s
#define IPAS_STALE_AFTER_S 15           // data older than this is reported stale
#define IPAS_REINIT_AFTER_FAILS 10      // consecutive failures before I2C re-init
// Calibration offsets added to the raw sensor values (the gas heater warms
// the board, so temperature usually reads high and humidity low).
#ifndef IPAS_TEMP_OFFSET_C
#define IPAS_TEMP_OFFSET_C 0.0f
#endif
#ifndef IPAS_HUM_OFFSET_PCT
#define IPAS_HUM_OFFSET_PCT 0.0f
#endif

// ---- microSD card (SPI) -----------------------------------------------------
// The 5-minute samples are appended to /ipas_log.csv and restored at the next boot.
// UNVERIFIED pin choice (free pins, grouped away from I2C on GPIO8/9). Check against your wiring.
#ifndef IPAS_SD_ENABLED
#define IPAS_SD_ENABLED 1
#endif
#ifndef IPAS_SD_SCK
#define IPAS_SD_SCK 4
#endif
#ifndef IPAS_SD_MISO
#define IPAS_SD_MISO 5
#endif
#ifndef IPAS_SD_MOSI
#define IPAS_SD_MOSI 6
#endif
#ifndef IPAS_SD_CS
#define IPAS_SD_CS 7
#endif
#define IPAS_SD_FREQ_HZ 4000000                 // first attempt
#define IPAS_SD_FREQ_FALLBACK_HZ 1000000        // every second attempt (more tolerant of long wires)
#define IPAS_SD_MOUNT_ATTEMPTS 6                // tries per boot before giving up
#define IPAS_SD_AUTO_RESTARTS 2                 // automatic restarts when the card is still not found (0 = off)
#define IPAS_SD_LOG_FILE "/ipas_log.csv"
#define IPAS_SD_MAX_LOAD_BYTES (256u * 1024u)   // only the newest part of the log is read at boot
#define IPAS_SD_RETRY_MS 30000                  // remount attempt interval after an error
