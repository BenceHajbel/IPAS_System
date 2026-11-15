#pragma once
#include "Arduino.h"
struct DNSServer { bool start(uint16_t, const String&, const IPAddress&) { return true; } void processNextRequest() {} };
