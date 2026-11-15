// Minimal API-shaped stubs so src/main.cpp can be syntax-checked on a host. NOT a simulator.
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdarg.h>
struct String : std::string { using std::string::string; String() {} String(const std::string& s) : std::string(s) {}
  bool startsWith(const char* p) const { return rfind(p, 0) == 0; } const char* c_str_() const { return c_str(); } };
struct IPAddress { IPAddress() {} IPAddress(uint8_t, uint8_t, uint8_t, uint8_t) {} String toString() const { return String(); } };
struct SerialStub { void begin(int) {} void print(const char*) {} void println(const char* = "") {} void printf(const char*, ...) {} };
extern SerialStub Serial;
uint32_t millis(); void delay(uint32_t);
struct EspStub { uint32_t getFreeHeap(); void restart() {} }; extern EspStub ESP;

#define INPUT_PULLUP 5
#define OUTPUT 3
#define LOW 0
#define HIGH 1
void pinMode(int, int); void digitalWrite(int, int); void delayMicroseconds(uint32_t);

#define RTC_NOINIT_ATTR
