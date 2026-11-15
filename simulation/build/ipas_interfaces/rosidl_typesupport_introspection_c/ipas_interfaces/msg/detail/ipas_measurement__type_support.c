// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from ipas_interfaces:msg/IPASMeasurement.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "ipas_interfaces/msg/detail/ipas_measurement__rosidl_typesupport_introspection_c.h"
#include "ipas_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "ipas_interfaces/msg/detail/ipas_measurement__functions.h"
#include "ipas_interfaces/msg/detail/ipas_measurement__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ipas_interfaces__msg__IPASMeasurement__init(message_memory);
}

void ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_fini_function(void * message_memory)
{
  ipas_interfaces__msg__IPASMeasurement__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_message_member_array[4] = {
  {
    "temperature",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ipas_interfaces__msg__IPASMeasurement, temperature),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL,  // resize(index) function pointer
    false  // is_rosidl_buffer
  },
  {
    "humidity",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ipas_interfaces__msg__IPASMeasurement, humidity),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL,  // resize(index) function pointer
    false  // is_rosidl_buffer
  },
  {
    "pressure",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ipas_interfaces__msg__IPASMeasurement, pressure),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL,  // resize(index) function pointer
    false  // is_rosidl_buffer
  },
  {
    "gas_resistance",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ipas_interfaces__msg__IPASMeasurement, gas_resistance),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL,  // resize(index) function pointer
    false  // is_rosidl_buffer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_message_members = {
  "ipas_interfaces__msg",  // message namespace
  "IPASMeasurement",  // message name
  4,  // number of fields
  sizeof(ipas_interfaces__msg__IPASMeasurement),
  false,  // has_any_key_member_
  ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_message_member_array,  // message members
  ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_init_function,  // function to initialize message memory (memory has to be allocated)
  ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_message_type_support_handle = {
  0,
  &ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_message_members,
  get_message_typesupport_handle_function,
  &ipas_interfaces__msg__IPASMeasurement__get_type_hash,
  &ipas_interfaces__msg__IPASMeasurement__get_type_description,
  &ipas_interfaces__msg__IPASMeasurement__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ipas_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ipas_interfaces, msg, IPASMeasurement)() {
  if (!ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_message_type_support_handle.typesupport_identifier) {
    ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ipas_interfaces__msg__IPASMeasurement__rosidl_typesupport_introspection_c__IPASMeasurement_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
