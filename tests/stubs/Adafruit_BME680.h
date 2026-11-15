#pragma once
#include <stdint.h>
#include "Wire.h"
#define BME680_OS_2X 2
#define BME680_OS_4X 3
#define BME680_OS_8X 4
#define BME680_FILTER_SIZE_3 2
class Adafruit_BME680 { public:
  Adafruit_BME680(TwoWire* = &Wire) {} bool begin(uint8_t = 0x77, bool = true) { return true; }
  void setTemperatureOversampling(uint8_t) {} void setHumidityOversampling(uint8_t) {} void setPressureOversampling(uint8_t) {}
  void setIIRFilterSize(uint8_t) {} bool setGasHeater(uint16_t, uint16_t) { return true; }
  uint32_t beginReading() { return 0; } bool endReading() { return true; }
  float temperature, humidity, pressure, gas_resistance; };
