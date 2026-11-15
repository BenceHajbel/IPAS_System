#pragma once
#include "Arduino.h"
#define FSPI 0
#define MSBFIRST 1
#define SPI_MODE0 0
struct SPISettings { SPISettings(uint32_t, int, int) {} };
struct SPIClass { SPIClass(int = 0) {} void begin(int, int, int, int) {} void end() {} void beginTransaction(SPISettings) {} void endTransaction() {} uint8_t transfer(uint8_t) { return 0; } };
