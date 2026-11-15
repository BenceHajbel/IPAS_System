#pragma once
#include <functional>
#include "Arduino.h"
#include "LittleFS.h"
enum HTTPMethod { HTTP_GET, HTTP_POST };
struct WebServer { WebServer(int) {} typedef std::function<void()> H;
  void on(const char*, HTTPMethod, H) {} void onNotFound(H) {} void begin() {} void handleClient() {}
  void sendHeader(const char*, const char*, bool = false) {} void send(int, const char*, const char*) {}
  size_t streamFile(File&, const char*) { return 0; } bool hasArg(const char*) { return false; }
  String arg(const char*) { return String(); } String uri() { return String(); } String hostHeader() { return String(); } };
