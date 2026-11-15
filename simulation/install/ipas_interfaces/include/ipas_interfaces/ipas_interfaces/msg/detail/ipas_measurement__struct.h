// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ipas_interfaces:msg/IPASMeasurement.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ipas_interfaces/msg/ipas_measurement.h"


#ifndef IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__STRUCT_H_
#define IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

/// Struct defined in msg/IPASMeasurement in the package ipas_interfaces.
typedef struct ipas_interfaces__msg__IPASMeasurement
{
  float temperature;
  float humidity;
  float pressure;
  float gas_resistance;
} ipas_interfaces__msg__IPASMeasurement;

// Struct for a sequence of ipas_interfaces__msg__IPASMeasurement.
typedef struct ipas_interfaces__msg__IPASMeasurement__Sequence
{
  ipas_interfaces__msg__IPASMeasurement * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ipas_interfaces__msg__IPASMeasurement__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__STRUCT_H_
