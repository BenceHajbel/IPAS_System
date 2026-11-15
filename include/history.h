// Bounded in-memory measurement history. Pure C++ (no Arduino dependencies),
// so it is unit-tested on the host. All data is LOST on reset or power loss.
#pragma once
#include <math.h>
#include <stddef.h>
#include <stdint.h>

namespace ipas {

// Three tiers, averaged from raw readings.
//   minute : 120 x 60 s   = 2 h   (RAM only, used for the 1 h graph)
//   fine   : 288 x 300 s  = 24 h  (also stored on the SD card, used for 12 h / 24 h)
//   coarse : 336 x 1800 s = 7 d   (rebuilt from the fine samples on the SD card)
// Timestamps are "timeline seconds": device uptime until the browser has set the
// clock, Unix time afterwards. Values >= kEpochMin are Unix time.
constexpr uint32_t kEpochMin = 1000000000u;
constexpr uint32_t kMinuteIntervalS = 60;
constexpr size_t kMinuteCount = 120;
constexpr uint32_t kFineIntervalS = 300;
constexpr size_t kFineCount = 288;
constexpr uint32_t kCoarseIntervalS = 1800;
constexpr size_t kCoarseCount = 336;

struct Reading {
  float temperature_c;
  float humidity_pct;
  float pressure_hpa;
  float gas_ohm;  // <= 0 or NaN means "gas value unavailable"
};

// 16 bytes per stored point.
struct Sample {
  uint32_t t;         // uptime seconds (average time of the samples in the bucket)
  int16_t temp_c100;  // 0.01 degC
  uint16_t hum_c100;  // 0.01 %RH
  uint32_t press_pa;  // Pa
  uint32_t gas_ohm;   // ohm, 0 = unavailable
};

// Range check for values coming from the BME680 (datasheet operating ranges).
inline bool plausibleReading(const Reading& r) {
  return isfinite(r.temperature_c) && r.temperature_c >= -40.f && r.temperature_c <= 85.f &&
         isfinite(r.humidity_pct) && r.humidity_pct >= 0.f && r.humidity_pct <= 100.f &&
         isfinite(r.pressure_hpa) && r.pressure_hpa >= 300.f && r.pressure_hpa <= 1100.f;
}

// Stricter than plausibleReading(): also requires a valid gas measurement. A misbehaving
// sensor was seen returning 33.9 C / 100 %RH / 761 hPa / gas 0, which is plausible by range
// but not a real measurement; such samples must not enter the history or the SD log.
inline bool acceptReading(const Reading& r) {
  return plausibleReading(r) && isfinite(r.gas_ohm) && r.gas_ohm > 0.f;
}

template <size_t N>
class Ring {
 public:
  void push(const Sample& s) {
    if (count_ < N) {
      buf_[(head_ + count_) % N] = s;
      ++count_;
    } else {
      buf_[head_] = s;
      head_ = (head_ + 1) % N;
    }
  }
  size_t size() const { return count_; }
  static constexpr size_t capacity() { return N; }
  const Sample& at(size_t i) const { return buf_[(head_ + i) % N]; }  // 0 = oldest
  Sample& atMut(size_t i) { return buf_[(head_ + i) % N]; }
  void clear() { head_ = 0; count_ = 0; }

 private:
  Sample buf_[N];
  size_t head_ = 0;
  size_t count_ = 0;
};

// Averages readings that fall into the same fixed time bucket.
class Accumulator {
 public:
  explicit Accumulator(uint32_t interval_s) : interval_(interval_s) {}

  // Adds a reading. If this reading starts a new bucket, the finished
  // bucket is written to *out and true is returned.
  bool add(uint32_t t, const Reading& r, Sample* out) {
    const uint32_t b = t / interval_;
    bool completed = false;
    if (n_ > 0 && b != bucket_) {
      *out = make();
      completed = true;
      reset();
    }
    bucket_ = b;
    ++n_;
    sum_t_ += t;
    sum_temp_ += r.temperature_c;
    sum_hum_ += r.humidity_pct;
    sum_press_ += r.pressure_hpa;
    if (isfinite(r.gas_ohm) && r.gas_ohm > 0.f) {
      sum_gas_ += r.gas_ohm;
      ++gas_n_;
    }
    return completed;
  }

  // Emits the bucket being filled (if any) and starts over.
  bool flush(Sample* out) {
    if (n_ == 0) return false;
    *out = make();
    reset();
    return true;
  }

  // The bucket currently being filled (not yet in the ring buffer).
  bool partial(Sample* out) const {
    if (n_ == 0) return false;
    *out = make();
    return true;
  }

 private:
  Sample make() const {
    Sample s;
    s.t = static_cast<uint32_t>(lround(sum_t_ / n_));
    s.temp_c100 = static_cast<int16_t>(lround(sum_temp_ / n_ * 100.0));
    s.hum_c100 = static_cast<uint16_t>(lround(sum_hum_ / n_ * 100.0));
    s.press_pa = static_cast<uint32_t>(lround(sum_press_ / n_ * 100.0));
    s.gas_ohm = gas_n_ ? static_cast<uint32_t>(lround(sum_gas_ / gas_n_)) : 0;
    return s;
  }
  void reset() {
    n_ = 0; gas_n_ = 0;
    sum_t_ = sum_temp_ = sum_hum_ = sum_press_ = sum_gas_ = 0.0;
  }

  uint32_t interval_;
  uint32_t bucket_ = 0;
  uint32_t n_ = 0, gas_n_ = 0;
  double sum_t_ = 0, sum_temp_ = 0, sum_hum_ = 0, sum_press_ = 0, sum_gas_ = 0;
};

class History {
 public:
  History() : minute_acc_(kMinuteIntervalS), fine_acc_(kFineIntervalS), coarse_acc_(kCoarseIntervalS) {}

  // Only call with readings that passed plausibleReading().
  // Returns true when a 5-minute sample was completed; it is written to *fine_done
  // (the caller stores it on the SD card).
  bool add(uint32_t t, const Reading& r, Sample* fine_done) {
    Sample s;
    bool fine = false;
    if (minute_acc_.add(t, r, &s)) minute_.push(s);
    if (fine_acc_.add(t, r, &s)) {
      fine_.push(s);
      if (fine_done) *fine_done = s;
      fine = true;
    }
    if (coarse_acc_.add(t, r, &s)) coarse_.push(s);
    return fine;
  }

  // The browser has set the clock: convert every uptime-based entry to Unix time by
  // adding `offset` (= unix_now - uptime_now). Unfinished buckets are closed first.
  // Returns true and fills *flushed_fine with the still uptime-based 5-minute sample
  // that was closed, so it can be logged before the sync record.
  bool rebase(uint32_t offset, Sample* flushed_fine) {
    Sample s;
    bool flushed = false;
    if (minute_acc_.flush(&s)) minute_.push(s);
    if (fine_acc_.flush(&s)) {
      fine_.push(s);
      if (flushed_fine) *flushed_fine = s;
      flushed = true;
    }
    if (coarse_acc_.flush(&s)) coarse_.push(s);
    shift(minute_, offset);
    shift(fine_, offset);
    shift(coarse_, offset);
    return flushed;
  }

  // Restores a 5-minute sample from the SD card (oldest first, Unix time).
  void loadFine(const Sample& s) {
    fine_.push(s);
    Reading r = {s.temp_c100 / 100.0f, s.hum_c100 / 100.0f, s.press_pa / 100.0f, static_cast<float>(s.gas_ohm)};
    Sample c;
    if (coarse_acc_.add(s.t, r, &c)) coarse_.push(c);
  }

  const Ring<kMinuteCount>& minute() const { return minute_; }
  const Ring<kFineCount>& fine() const { return fine_; }
  const Ring<kCoarseCount>& coarse() const { return coarse_; }
  bool minutePartial(Sample* s) const { return minute_acc_.partial(s); }
  bool finePartial(Sample* s) const { return fine_acc_.partial(s); }
  bool coarsePartial(Sample* s) const { return coarse_acc_.partial(s); }

 private:
  template <size_t N>
  static void shift(Ring<N>& ring, uint32_t offset) {
    for (size_t i = 0; i < ring.size(); ++i) {
      Sample& s = ring.atMut(i);
      if (s.t < kEpochMin) s.t += offset;
    }
  }

  Ring<kMinuteCount> minute_;
  Ring<kFineCount> fine_;
  Ring<kCoarseCount> coarse_;
  Accumulator minute_acc_;
  Accumulator fine_acc_;
  Accumulator coarse_acc_;
};

}  // namespace ipas
