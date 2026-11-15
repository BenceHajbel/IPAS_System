// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from ipas_interfaces:msg/IPASMeasurement.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ipas_interfaces/msg/ipas_measurement.hpp"


#ifndef IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__TRAITS_HPP_
#define IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__TRAITS_HPP_

#include <stdint.h>

#include <array>
#include <cstddef>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "ipas_interfaces/msg/detail/ipas_measurement__struct.hpp"
#include "rosidl_runtime_cpp/buffer__traits.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace ipas_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const IPASMeasurement & msg,
  std::ostream & out)
{
  out << "{";
  // member: temperature
  {
    out << "temperature: ";
    rosidl_generator_traits::value_to_yaml(msg.temperature, out);
    out << ", ";
  }

  // member: humidity
  {
    out << "humidity: ";
    rosidl_generator_traits::value_to_yaml(msg.humidity, out);
    out << ", ";
  }

  // member: pressure
  {
    out << "pressure: ";
    rosidl_generator_traits::value_to_yaml(msg.pressure, out);
    out << ", ";
  }

  // member: gas_resistance
  {
    out << "gas_resistance: ";
    rosidl_generator_traits::value_to_yaml(msg.gas_resistance, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const IPASMeasurement & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: temperature
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "temperature: ";
    rosidl_generator_traits::value_to_yaml(msg.temperature, out);
    out << "\n";
  }

  // member: humidity
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "humidity: ";
    rosidl_generator_traits::value_to_yaml(msg.humidity, out);
    out << "\n";
  }

  // member: pressure
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "pressure: ";
    rosidl_generator_traits::value_to_yaml(msg.pressure, out);
    out << "\n";
  }

  // member: gas_resistance
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "gas_resistance: ";
    rosidl_generator_traits::value_to_yaml(msg.gas_resistance, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const IPASMeasurement & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

template<typename T, std::enable_if_t<std::is_same_v<std::decay_t<T>, ipas_interfaces::msg::IPASMeasurement>, int> = 0>
constexpr auto as_tuple_ref(T && msg)
{
  return std::forward_as_tuple(
    std::forward<T>(msg).temperature,
    std::forward<T>(msg).humidity,
    std::forward<T>(msg).pressure,
    std::forward<T>(msg).gas_resistance);
}

}  // namespace msg

}  // namespace ipas_interfaces

namespace rosidl_generator_traits
{

template<>
constexpr const char * data_type<ipas_interfaces::msg::IPASMeasurement>()
{
  return "ipas_interfaces::msg::IPASMeasurement";
}

template<>
constexpr const char * name<ipas_interfaces::msg::IPASMeasurement>()
{
  return "ipas_interfaces/msg/IPASMeasurement";
}

template<>
struct has_fixed_size<ipas_interfaces::msg::IPASMeasurement>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<ipas_interfaces::msg::IPASMeasurement>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<ipas_interfaces::msg::IPASMeasurement>
  : std::true_type {};

template<>
struct MessageTraits<ipas_interfaces::msg::IPASMeasurement>
{
  static constexpr std::size_t member_count = 4;
  static constexpr std::array<std::string_view, member_count> member_names = {
    "temperature",
    "humidity",
    "pressure",
    "gas_resistance",
  };
};

}  // namespace rosidl_generator_traits

#endif  // IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__TRAITS_HPP_
