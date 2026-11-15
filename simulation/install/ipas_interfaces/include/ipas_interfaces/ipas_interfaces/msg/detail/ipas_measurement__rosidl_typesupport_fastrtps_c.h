// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from ipas_interfaces:msg/IPASMeasurement.idl
// generated code does not contain a copyright notice
#ifndef IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "ipas_interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "ipas_interfaces/msg/detail/ipas_measurement__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
bool cdr_serialize_ipas_interfaces__msg__IPASMeasurement(
  const ipas_interfaces__msg__IPASMeasurement * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
bool cdr_deserialize_ipas_interfaces__msg__IPASMeasurement(
  eprosima::fastcdr::Cdr &,
  ipas_interfaces__msg__IPASMeasurement * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
size_t get_serialized_size_ipas_interfaces__msg__IPASMeasurement(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
size_t max_serialized_size_ipas_interfaces__msg__IPASMeasurement(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
bool cdr_serialize_key_ipas_interfaces__msg__IPASMeasurement(
  const ipas_interfaces__msg__IPASMeasurement * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
size_t get_serialized_size_key_ipas_interfaces__msg__IPASMeasurement(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
size_t max_serialized_size_key_ipas_interfaces__msg__IPASMeasurement(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ipas_interfaces, msg, IPASMeasurement)();

#ifdef __cplusplus
}
#endif

#endif  // IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
