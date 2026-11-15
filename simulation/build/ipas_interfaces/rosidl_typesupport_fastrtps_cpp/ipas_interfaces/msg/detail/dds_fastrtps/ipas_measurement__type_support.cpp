// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from ipas_interfaces:msg/IPASMeasurement.idl
// generated code does not contain a copyright notice
#include "ipas_interfaces/msg/detail/ipas_measurement__rosidl_typesupport_fastrtps_cpp.hpp"
#include "ipas_interfaces/msg/detail/ipas_measurement__functions.h"
#include "ipas_interfaces/msg/detail/ipas_measurement__struct.hpp"

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <string>
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_fastrtps_cpp/identifier.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_fastrtps_cpp/serialization_helpers.hpp"
#include "fastcdr/Cdr.h"


// forward declaration of message dependencies and their conversion functions

namespace ipas_interfaces
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{


bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ipas_interfaces
cdr_serialize(
  const ipas_interfaces::msg::IPASMeasurement & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: temperature
  cdr << ros_message.temperature;

  // Member: humidity
  cdr << ros_message.humidity;

  // Member: pressure
  cdr << ros_message.pressure;

  // Member: gas_resistance
  cdr << ros_message.gas_resistance;

  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ipas_interfaces
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  ipas_interfaces::msg::IPASMeasurement & ros_message)
{
  // Member: temperature
  cdr >> ros_message.temperature;

  // Member: humidity
  cdr >> ros_message.humidity;

  // Member: pressure
  cdr >> ros_message.pressure;

  // Member: gas_resistance
  cdr >> ros_message.gas_resistance;

  return true;
}  // NOLINT(readability/fn_size)


size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ipas_interfaces
get_serialized_size(
  const ipas_interfaces::msg::IPASMeasurement & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: temperature
  {
    size_t item_size = sizeof(ros_message.temperature);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: humidity
  {
    size_t item_size = sizeof(ros_message.humidity);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: pressure
  {
    size_t item_size = sizeof(ros_message.pressure);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: gas_resistance
  {
    size_t item_size = sizeof(ros_message.gas_resistance);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}


size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ipas_interfaces
max_serialized_size_IPASMeasurement(
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

  // Member: temperature
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // Member: humidity
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // Member: pressure
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // Member: gas_resistance
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
    using DataType = ipas_interfaces::msg::IPASMeasurement;
    is_plain =
      (
      offsetof(DataType, gas_resistance) +
      last_member_size
      ) == ret_val;
  }

  return ret_val;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ipas_interfaces
cdr_serialize_key(
  const ipas_interfaces::msg::IPASMeasurement & ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: temperature
  cdr << ros_message.temperature;

  // Member: humidity
  cdr << ros_message.humidity;

  // Member: pressure
  cdr << ros_message.pressure;

  // Member: gas_resistance
  cdr << ros_message.gas_resistance;

  return true;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ipas_interfaces
get_serialized_size_key(
  const ipas_interfaces::msg::IPASMeasurement & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: temperature
  {
    size_t item_size = sizeof(ros_message.temperature);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: humidity
  {
    size_t item_size = sizeof(ros_message.humidity);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: pressure
  {
    size_t item_size = sizeof(ros_message.pressure);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  // Member: gas_resistance
  {
    size_t item_size = sizeof(ros_message.gas_resistance);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_ipas_interfaces
max_serialized_size_key_IPASMeasurement(
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

  // Member: temperature
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: humidity
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: pressure
  {
    size_t array_size = 1;
    last_member_size = array_size * sizeof(uint32_t);
    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  // Member: gas_resistance
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
    using DataType = ipas_interfaces::msg::IPASMeasurement;
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
  auto typed_message =
    static_cast<const ipas_interfaces::msg::IPASMeasurement *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _IPASMeasurement__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<ipas_interfaces::msg::IPASMeasurement *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _IPASMeasurement__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const ipas_interfaces::msg::IPASMeasurement *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _IPASMeasurement__max_serialized_size(char & bounds_info)
{
  bool full_bounded;
  bool is_plain;
  size_t ret_val;

  ret_val = max_serialized_size_IPASMeasurement(full_bounded, is_plain, 0);

  bounds_info =
    is_plain ? ROSIDL_TYPESUPPORT_FASTRTPS_PLAIN_TYPE :
    full_bounded ? ROSIDL_TYPESUPPORT_FASTRTPS_BOUNDED_TYPE : ROSIDL_TYPESUPPORT_FASTRTPS_UNBOUNDED_TYPE;
  return ret_val;
}


static message_type_support_callbacks_t _IPASMeasurement__callbacks = {
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

static rosidl_message_type_support_t _IPASMeasurement__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_IPASMeasurement__callbacks,
  get_message_typesupport_handle_function,
  &ipas_interfaces__msg__IPASMeasurement__get_type_hash,
  &ipas_interfaces__msg__IPASMeasurement__get_type_description,
  &ipas_interfaces__msg__IPASMeasurement__get_type_description_sources,
};

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace ipas_interfaces

namespace rosidl_typesupport_fastrtps_cpp
{

template<>
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_EXPORT_ipas_interfaces
const rosidl_message_type_support_t *
get_message_type_support_handle<ipas_interfaces::msg::IPASMeasurement>()
{
  return &ipas_interfaces::msg::typesupport_fastrtps_cpp::_IPASMeasurement__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, ipas_interfaces, msg, IPASMeasurement)() {
  return &ipas_interfaces::msg::typesupport_fastrtps_cpp::_IPASMeasurement__handle;
}

#ifdef __cplusplus
}
#endif
