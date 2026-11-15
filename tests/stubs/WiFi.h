#pragma once
#include "Arduino.h"
typedef enum { WIFI_POWER_11dBm = 44, WIFI_POWER_8_5dBm = 34 } wifi_power_t;
typedef enum { ARDUINO_EVENT_WIFI_AP_STACONNECTED, ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED, ARDUINO_EVENT_WIFI_AP_STADISCONNECTED, ARDUINO_EVENT_OTHER } arduino_event_id_t;
#define WIFI_AP 2
typedef int WiFiEvent_t; typedef int WiFiEventInfo_t;
#define ARDUINO_EVENT_WIFI_AP_STACONNECTED 1
#define ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED 2
#define ARDUINO_EVENT_WIFI_AP_STADISCONNECTED 3
struct WiFiStub { template <class F> void onEvent(F) {} void onEvent(void (*)(arduino_event_id_t)) {} void mode(int) {} bool softAPConfig(IPAddress, IPAddress, IPAddress) { return true; }
  bool softAP(const char*, const char*, int, int, int) { return true; } void setTxPower(wifi_power_t) {}
  IPAddress softAPIP() { return IPAddress(); } int softAPgetStationNum() { return 0; } };
extern WiFiStub WiFi;
