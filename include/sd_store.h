// microSD persistence for the 5-minute samples (Arduino glue; the parsing lives in log_format.h).
#pragma once
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <string>
#include "config.h"
#include "json_api.h"
#include "log_format.h"

namespace ipas {

class SdStore {
 public:
  // Mounts the card (with retries), restores the history from the log and starts a new boot record.
  // Returns false when no card could be mounted.
  bool begin(History& history) {
#if IPAS_SD_ENABLED
    history_ = &history;
    if (!mountWithRetry()) {
      info_.state = StorageState::NoCard;
      Serial.println("[IPAS] SD: no card / mount failed (history is RAM only)");
      return false;
    }
    LogLoader loader(history);
    load(loader);
    info_.loaded_samples = loader.stats().samples;
    info_.discarded_unsynced = loader.stats().discarded_unsynced;
    info_.boot = loader.stats().max_boot + 1;
    Serial.printf("[IPAS] SD: restored %u samples (%u bad lines, %u from boots without clock skipped), this is boot %u, mount attempts %u\n",
                  (unsigned)loader.stats().samples, (unsigned)loader.stats().bad_lines,
                  (unsigned)loader.stats().discarded_unsynced, (unsigned)info_.boot, (unsigned)info_.mount_attempts);
    info_.state = StorageState::Ok;
    append(fmtBoot(info_.boot));
    return true;
#else
    (void)history;
    return true;
#endif
  }

  void setRestarts(uint32_t n) { info_.restarts = n; }

  void logSample(const Sample& s) { if (active()) append(fmtSample(info_.boot, s)); }
  void logSync(uint32_t uptime_s, uint32_t unix_s) { if (active()) append(fmtSync(info_.boot, uptime_s, unix_s)); }

  // Call regularly: after a write error try to mount the card again.
  void tick() {
#if IPAS_SD_ENABLED
    if (info_.state == StorageState::Error && info_.boot && millis() - last_retry_ms_ > IPAS_SD_RETRY_MS) {
      last_retry_ms_ = millis();
      SD.end();
      if (mountOnce(IPAS_SD_FREQ_FALLBACK_HZ)) { info_.state = StorageState::Ok; append(fmtBoot(info_.boot)); }
    }
#endif
  }

  const StorageInfo& info() const { return info_; }

 private:
  bool active() const { return info_.state == StorageState::Ok || info_.state == StorageState::Error; }

  // One mount try. A reset can leave the card in the middle of a command; 80 clock pulses with
  // CS high make it listen for a new command again before SD.begin() starts talking.
  bool mountOnce(uint32_t freq_hz) {
    spi_.end();
    spi_.begin(IPAS_SD_SCK, IPAS_SD_MISO, IPAS_SD_MOSI, IPAS_SD_CS);
    pinMode(IPAS_SD_CS, OUTPUT);
    digitalWrite(IPAS_SD_CS, HIGH);
    spi_.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
    for (int i = 0; i < 10; ++i) spi_.transfer(0xFF);
    spi_.endTransaction();
    return SD.begin(IPAS_SD_CS, spi_, freq_hz) && SD.cardType() != CARD_NONE;
  }

  // The module's regulator and the card need a moment after power-up or reset, so try several times,
  // alternating a normal and a slow SPI clock.
  bool mountWithRetry() {
    for (uint32_t a = 0; a < IPAS_SD_MOUNT_ATTEMPTS; ++a) {
      ++info_.mount_attempts;
      SD.end();
      delay(a == 0 ? 250 : 400);
      if (mountOnce(a % 2 == 0 ? IPAS_SD_FREQ_HZ : IPAS_SD_FREQ_FALLBACK_HZ)) return true;
      Serial.printf("[IPAS] SD: mount attempt %u of %u failed\n", (unsigned)(a + 1), (unsigned)IPAS_SD_MOUNT_ATTEMPTS);
    }
    return false;
  }

  void load(LogLoader& loader) {
    File f = SD.open(IPAS_SD_LOG_FILE, FILE_READ);
    if (!f) return;  // no log yet
    size_t size = f.size();
    bool skip_partial = false;
    if (size > IPAS_SD_MAX_LOAD_BYTES) { f.seek(size - IPAS_SD_MAX_LOAD_BYTES); skip_partial = true; }
    char line[128];
    size_t len = 0;
    bool overflow = false;
    uint8_t buf[512];
    int n;
    while ((n = f.read(buf, sizeof buf)) > 0) {
      for (int i = 0; i < n; ++i) {
        char c = (char)buf[i];
        if (c == '\n') {
          if (skip_partial) { skip_partial = false; }
          else if (!overflow) { line[len] = 0; loader.feedLine(line); }
          len = 0; overflow = false;
        } else if (len < sizeof line - 1) line[len++] = c;
        else overflow = true;
      }
    }
    f.close();
    loader.finish();
  }

  void append(const std::string& rec) {
#if IPAS_SD_ENABLED
    File f = SD.open(IPAS_SD_LOG_FILE, FILE_APPEND);
    if (!f) { fail(); return; }
    size_t w = f.print(rec.c_str());
    info_.file_bytes = (uint32_t)f.size();
    f.close();                       // close = data is flushed to the card
    if (w != rec.size()) { fail(); return; }
    if (info_.state == StorageState::Error) info_.state = StorageState::Ok;
#else
    (void)rec;
#endif
  }

  void fail() { ++info_.write_errors; info_.state = StorageState::Error; }

  StorageInfo info_;
  History* history_ = nullptr;
  uint32_t last_retry_ms_ = 0;
  SPIClass spi_{FSPI};
};

}  // namespace ipas
