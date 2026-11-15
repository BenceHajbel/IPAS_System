// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ipas_interfaces:msg/IPASMeasurement.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ipas_interfaces/msg/ipas_measurement.hpp"


#ifndef IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__BUILDER_HPP_
#define IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ipas_interfaces/msg/detail/ipas_measurement__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ipas_interfaces
{

namespace msg
{

namespace builder
{

class Init_IPASMeasurement_gas_resistance
{
public:
  explicit Init_IPASMeasurement_gas_resistance(::ipas_interfaces::msg::IPASMeasurement & msg)
  : msg_(msg)
  {}
  ::ipas_interfaces::msg::IPASMeasurement gas_resistance(::ipas_interfaces::msg::IPASMeasurement::_gas_resistance_type arg)
  {
    msg_.gas_resistance = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ipas_interfaces::msg::IPASMeasurement msg_;
};

class Init_IPASMeasurement_pressure
{
public:
  explicit Init_IPASMeasurement_pressure(::ipas_interfaces::msg::IPASMeasurement & msg)
  : msg_(msg)
  {}
  Init_IPASMeasurement_gas_resistance pressure(::ipas_interfaces::msg::IPASMeasurement::_pressure_type arg)
  {
    msg_.pressure = std::move(arg);
    return Init_IPASMeasurement_gas_resistance(msg_);
  }

private:
  ::ipas_interfaces::msg::IPASMeasurement msg_;
};

class Init_IPASMeasurement_humidity
{
public:
  explicit Init_IPASMeasurement_humidity(::ipas_interfaces::msg::IPASMeasurement & msg)
  : msg_(msg)
  {}
  Init_IPASMeasurement_pressure humidity(::ipas_interfaces::msg::IPASMeasurement::_humidity_type arg)
  {
    msg_.humidity = std::move(arg);
    return Init_IPASMeasurement_pressure(msg_);
  }

private:
  ::ipas_interfaces::msg::IPASMeasurement msg_;
};

class Init_IPASMeasurement_temperature
{
public:
  Init_IPASMeasurement_temperature()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_IPASMeasurement_humidity temperature(::ipas_interfaces::msg::IPASMeasurement::_temperature_type arg)
  {
    msg_.temperature = std::move(arg);
    return Init_IPASMeasurement_humidity(msg_);
  }

private:
  ::ipas_interfaces::msg::IPASMeasurement msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::ipas_interfaces::msg::IPASMeasurement>()
{
  return ipas_interfaces::msg::builder::Init_IPASMeasurement_temperature();
}

}  // namespace ipas_interfaces

#endif  // IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__BUILDER_HPP_
