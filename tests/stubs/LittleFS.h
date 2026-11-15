#pragma once
#include "Arduino.h"
struct File { operator bool() const { return true; } void close() {} size_t size() { return 0; } bool seek(size_t) { return true; }
  int read(uint8_t*, size_t) { return 0; } size_t print(const char*) { return 0; } };
struct LittleFSStub { bool begin(bool = false) { return true; } File open(const char*, const char*) { return File(); } };
extern LittleFSStub LittleFS;
