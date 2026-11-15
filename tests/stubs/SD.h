#pragma once
#include "Arduino.h"
#include "LittleFS.h"
#include "SPI.h"
#define FILE_READ "r"
#define FILE_APPEND "a"
enum { CARD_NONE = 0 };
struct SDStub { bool begin(int, SPIClass&, uint32_t) { return true; } int cardType() { return 1; } void end() {}
  File open(const char*, const char*) { return File(); } };
extern SDStub SD;
