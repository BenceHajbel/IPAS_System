#pragma once
#include <stdint.h>
struct TwoWire { void begin(int, int) {} void end() {} void setClock(uint32_t) {} void beginTransmission(uint8_t) {} uint8_t endTransmission() { return 0; } };
extern TwoWire Wire;
