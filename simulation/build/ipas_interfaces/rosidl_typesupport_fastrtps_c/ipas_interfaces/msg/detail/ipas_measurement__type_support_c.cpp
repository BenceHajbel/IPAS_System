// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from ipas_interfaces:msg/IPASMeasurement.idl
// generated code does not contain a copyright notice
#include "ipas_interfaces/msg/detail/ipas_measurement__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <cstddef>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/serialization_helpers.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "ipas_interfaces/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "ipas_interfaces/msg/detail/ipas_measurement__struct.h"
#include "ipas_interfaces/msg/detail/ipas_measurement__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif


// forward declare type support functions


using _IPASMeasurement__ros_msg_type = ipas_interfaces__msg__IPASMeasurement;


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
bool cdr_serialize_ipas_interfaces__msg__IPASMeasurement(
  const ipas_interfaces__msg__IPASMeasurement * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: temperature
  {
    cdr << ros_message->temperature;
  }

  // Field name: humidity
  {
    cdr << ros_message->humidity;
  }

  // Field name: pressure
  {
    cdr << ros_message->pressure;
  }

  // Field name: gas_resistance
  {
    cdr << ros_message->gas_resistance;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
bool cdr_deserialize_ipas_interfaces__msg__IPASMeasurement(
  eprosima::fastcdr::Cdr & cdr,
  ipas_interfaces__msg__IPASMeasurement * ros_message)
{
  // Field name: temperature
  {
    cdr >> ros_message->temperature;
  }

  // Field name: humidity
  {
    cdr >> ros_message->humidity;
  }

  // Field name: pressure
  {
    cdr >> ros_message->pressure;
  }

  // Field name: gas_resistance
  {
    cdr >> ros_message->gas_resistance;
  }

  return true;
}  // NOLINT(readability/fn_size)


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
size_t get_serialized_size_ipas_interfaces__msg__IPASMeasurement(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _IPASMeasurement__ros_msg_type * ros_message = static_cast<const _IPASMeasurement__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: temperature
  {
    size_t item_size = sizeof(ros_message->temperature);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: humidity
  {
    size_t item_size = sizeof(ros_message->humidity);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: pressure
  {
    size_t item_size = sizeof(ros_message->pressure);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: gas_resistance
  {
    size_t item_size = sizeof(ros_message->gas_resistance);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}


ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
size_t max_serialized_size_ipas_interfaces__msg__IPASMeasurement(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;

  // Field name: temperature
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: humidity
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: pressure
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: gas_resistance
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }


  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = ipas_interfaces__msg__IPASMeasurement;
    is_plain =
      (
      offsetof(DataType, gas_resistance) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
bool cdr_serialize_key_ipas_interfaces__msg__IPASMeasurement(
  const ipas_interfaces__msg__IPASMeasurement * ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Field name: temperature
  {
    cdr << ros_message->temperature;
  }

  // Field name: humidity
  {
    cdr << ros_message->humidity;
  }

  // Field name: pressure
  {
    cdr << ros_message->pressure;
  }

  // Field name: gas_resistance
  {
    cdr << ros_message->gas_resistance;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
size_t get_serialized_size_key_ipas_interfaces__msg__IPASMeasurement(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _IPASMeasurement__ros_msg_type * ros_message = static_cast<const _IPASMeasurement__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;

  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Field name: temperature
  {
    size_t item_size = sizeof(ros_message->temperature);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: humidity
  {
    size_t item_size = sizeof(ros_message->humidity);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: pressure
  {
    size_t item_size = sizeof(ros_message->pressure);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Field name: gas_resistance
  {
    size_t item_size = sizeof(ros_message->gas_resistance);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_ipas_interfaces
size_t max_serialized_size_key_ipas_interfaces__msg__IPASMeasurement(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  size_t last_member_size = 0;
  (void)last_member_size;
  (void)padding;
  (void)wchar_size;

  full_bounded = true;
  is_plain = true;
  // Field name: temperature
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: humidity
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: pressure
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Field name: gas_resistance
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  size_t ret_val = current_alignment - initial_alignment;
  if (is_plain) {
    // All members are plain, and type is not empty.
    // We still need to check that the in-memory alignment
    // is the same as the CDR mandated alignment.
    using DataType = ipas_interfaces__msg__IPASMeasurement;
    is_plain =
      (
      offsetof(DataType, gas_resistance) +
      last_member_size
      ) == ret_val;
  }
  return ret_val;
}


static bool _IPASMeasurement__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const ipas_interfaces__msg__IPASMeasurement * ros_message = static_cast<const ipas_interfaces__msg__IPASMeasurement *>(untyped_ros_message);
  (void)ros_message;
  return cdr_serialize_ipas_interfaces__msg__IPASMeasurement(ros_message, cdr);
}

static bool _IPASMeasurement__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  ipas_interfaces__msg__IPASMeasurement * ros_message = static_cast<ipas_interfaces__msg__IPASMeasurement *>(untyped_ros_message);
  (void)ros_message;
  return cdr_deserialize_ipas_interfaces__msg__IPASMeasurement(cdr, ros_message);
}

static uint32_t _IPASMeasurement__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_ipas_interfaces__msg__IPASMeasurement(
      untyped_ros_message, 0));
}

static size_t _IPASMeasurement__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_ipas_interfaces__msg__IPASMeasurement(
    full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t __callbacks_IPASMeasurement = {
  "ipas_interfaces::msg",
  "IPASMeasurement",
  _IPASMeasurement__cdr_serialize,
  _IPASMeasurement__cdr_deserialize,
  _IPASMeasurement__get_serialized_size,
  _IPASMeasurement__max_serialized_size,
  nullptr,
  false,
  nullptr,
  nullptr
};

static rosidl_message_type_support_t _IPASMeasurement__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_IPASMeasurement,
  get_message_typesupport_handle_function,
  &ipas_interfaces__msg__IPASMeasurement__get_type_hash,
  &ipas_interfaces__msg__IPASMeasurement__get_type_description,
  &ipas_interfaces__msg__IPASMeasurement__get_type_description_sources,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ipas_interfaces, msg, IPASMeasurement)() {
  return &_IPASMeasurement__type_support;
}

#if defined(__cplusplus)
}
#endif
