// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from ipas_interfaces:msg/IPASMeasurement.idl
// generated code does not contain a copyright notice

#include "ipas_interfaces/msg/detail/ipas_measurement__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_ipas_interfaces
const rosidl_type_hash_t *
ipas_interfaces__msg__IPASMeasurement__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xc4, 0x16, 0x2b, 0xe2, 0x34, 0x4a, 0x77, 0xb0,
      0x5d, 0xa3, 0xd2, 0xcb, 0x6e, 0xb0, 0x5e, 0x65,
      0x68, 0x6d, 0xf9, 0xfd, 0x0c, 0x89, 0x5b, 0x08,
      0x22, 0xeb, 0xb3, 0xd6, 0xe9, 0x0a, 0x19, 0x68,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char ipas_interfaces__msg__IPASMeasurement__TYPE_NAME[] = "ipas_interfaces/msg/IPASMeasurement";

// Define type names, field names, and default values
static char ipas_interfaces__msg__IPASMeasurement__FIELD_NAME__temperature[] = "temperature";
static char ipas_interfaces__msg__IPASMeasurement__FIELD_NAME__humidity[] = "humidity";
static char ipas_interfaces__msg__IPASMeasurement__FIELD_NAME__pressure[] = "pressure";
static char ipas_interfaces__msg__IPASMeasurement__FIELD_NAME__gas_resistance[] = "gas_resistance";

static rosidl_runtime_c__type_description__Field ipas_interfaces__msg__IPASMeasurement__FIELDS[] = {
  {
    {ipas_interfaces__msg__IPASMeasurement__FIELD_NAME__temperature, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {ipas_interfaces__msg__IPASMeasurement__FIELD_NAME__humidity, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {ipas_interfaces__msg__IPASMeasurement__FIELD_NAME__pressure, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {ipas_interfaces__msg__IPASMeasurement__FIELD_NAME__gas_resistance, 14, 14},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ipas_interfaces__msg__IPASMeasurement__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ipas_interfaces__msg__IPASMeasurement__TYPE_NAME, 35, 35},
      {ipas_interfaces__msg__IPASMeasurement__FIELDS, 4, 4},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "float32 temperature\n"
  "float32 humidity\n"
  "float32 pressure\n"
  "float32 gas_resistance";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
ipas_interfaces__msg__IPASMeasurement__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ipas_interfaces__msg__IPASMeasurement__TYPE_NAME, 35, 35},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 77, 77},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ipas_interfaces__msg__IPASMeasurement__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ipas_interfaces__msg__IPASMeasurement__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
