// SD-card log format and loader. Pure C++ (unit-tested on the host).
// One text line per record, each ending with '*' so a line cut short by a power loss
// is detected and ignored:
//   B,<boot>*                                        a boot started
//   S,<boot>,<t>,<temp_c100>,<hum_c100>,<press_pa>,<gas_ohm>*   one 5-minute sample
//   T,<boot>,<uptime_s>,<unix_s>*                    the browser set the clock during this boot
// Samples written before the clock was set carry uptime seconds (< kEpochMin); the
// following T record gives the offset that places them on the real time line. A boot
// whose clock was never set cannot be placed and is skipped when loading.
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include "history.h"

namespace ipas {

inline std::string fmtBoot(uint32_t boot) {
  char b[32]; snprintf(b, sizeof b, "B,%u*\n", (unsigned)boot); return b;
}
inline std::string fmtSample(uint32_t boot, const Sample& s) {
  char b[96];
  snprintf(b, sizeof b, "S,%u,%u,%d,%u,%u,%u*\n", (unsigned)boot, (unsigned)s.t, (int)s.temp_c100,
           (unsigned)s.hum_c100, (unsigned)s.press_pa, (unsigned)s.gas_ohm);
  return b;
}
inline std::string fmtSync(uint32_t boot, uint32_t uptime_s, uint32_t unix_s) {
  char b[64]; snprintf(b, sizeof b, "T,%u,%u,%u*\n", (unsigned)boot, (unsigned)uptime_s, (unsigned)unix_s); return b;
}

struct LoadStats {
  uint32_t lines = 0;
  uint32_t bad_lines = 0;
  uint32_t samples = 0;             // samples restored into the history
  uint32_t discarded_unsynced = 0;  // samples of boots whose clock was never set
  uint32_t max_boot = 0;
};

class LogLoader {
 public:
  explicit LogLoader(History& h) : h_(h) {}
  const LoadStats& stats() const { return st_; }

  void feedLine(const char* line) {
    ++st_.lines;
    char kind = 0;
    long long f[8];
    int nf = parse(line, &kind, f, 8);
    if (nf < 0) { ++st_.bad_lines; return; }
    if (kind == 'B' && nf == 1 && f[0] > 0) {
      switchBoot((uint32_t)f[0]);
    } else if (kind == 'S' && nf == 6 && f[0] > 0 && f[1] >= 0) {
      switchBoot((uint32_t)f[0]);
      Sample s;
      s.t = (uint32_t)f[1];
      if (f[2] < -4000 || f[2] > 8500 || f[3] < 0 || f[3] > 10000 || f[4] < 30000 || f[4] > 110000 || f[5] < 0 || f[5] > 4000000000LL) {
        ++st_.bad_lines; return;
      }
      s.temp_c100 = (int16_t)f[2]; s.hum_c100 = (uint16_t)f[3]; s.press_pa = (uint32_t)f[4]; s.gas_ohm = (uint32_t)f[5];
      if (s.t >= kEpochMin) accept(s);
      else if (synced_) { s.t += offset_; accept(s); }
      else pending_.push_back(s);
    } else if (kind == 'T' && nf == 3 && f[0] > 0 && f[2] >= (long long)kEpochMin && f[1] >= 0 && f[2] > f[1]) {
      switchBoot((uint32_t)f[0]);
      if (!synced_) {
        offset_ = (uint32_t)(f[2] - f[1]);
        synced_ = true;
        for (Sample s : pending_) { s.t += offset_; accept(s); }
        pending_.clear();
      }
    } else {
      ++st_.bad_lines;
    }
    if ((uint32_t)(f[0] > 0 ? f[0] : 0) > st_.max_boot) st_.max_boot = (uint32_t)f[0];
  }

  // Call after the last line.
  void finish() {
    st_.discarded_unsynced += (uint32_t)pending_.size();
    pending_.clear();
  }

 private:
  // Returns the number of numeric fields, or -1 if the line is malformed or incomplete.
  static int parse(const char* line, char* kind, long long* f, int maxf) {
    char buf[128];
    size_t n = strlen(line);
    while (n && (line[n - 1] == '\n' || line[n - 1] == '\r')) --n;
    if (n < 3 || n >= sizeof buf || line[n - 1] != '*') return -1;
    memcpy(buf, line, n - 1);
    buf[n - 1] = 0;
    if (buf[1] != ',') return -1;
    *kind = buf[0];
    int cnt = 0;
    char* p = buf + 2;
    while (*p) {
      char* end = nullptr;
      long long v = strtoll(p, &end, 10);
      if (end == p || cnt >= maxf) return -1;
      f[cnt++] = v;
      if (*end == ',') p = end + 1;
      else if (*end == 0) break;
      else return -1;
    }
    return cnt;
  }

  void switchBoot(uint32_t b) {
    if (b == cur_boot_) return;
    st_.discarded_unsynced += (uint32_t)pending_.size();
    pending_.clear();
    cur_boot_ = b;
    synced_ = false;
    offset_ = 0;
  }

  void accept(const Sample& s) {
    if (s.t <= last_t_) { ++st_.bad_lines; return; }  // keep the history strictly in time order
    last_t_ = s.t;
    h_.loadFine(s);
    ++st_.samples;
  }

  History& h_;
  LoadStats st_;
  uint32_t cur_boot_ = 0;
  uint32_t last_t_ = 0;
  uint32_t offset_ = 0;
  bool synced_ = false;
  std::vector<Sample> pending_;
};

}  // namespace ipas
