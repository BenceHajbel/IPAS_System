// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from ipas_interfaces:msg/IPASMeasurement.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ipas_interfaces/msg/ipas_measurement.hpp"


#ifndef IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__STRUCT_HPP_
#define IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_buffer/buffer.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__ipas_interfaces__msg__IPASMeasurement __attribute__((deprecated))
#else
# define DEPRECATED__ipas_interfaces__msg__IPASMeasurement __declspec(deprecated)
#endif

namespace ipas_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct IPASMeasurement_
{
  using Type = IPASMeasurement_<ContainerAllocator>;

  explicit IPASMeasurement_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->temperature = 0.0f;
      this->humidity = 0.0f;
      this->pressure = 0.0f;
      this->gas_resistance = 0.0f;
    }
  }

  explicit IPASMeasurement_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->temperature = 0.0f;
      this->humidity = 0.0f;
      this->pressure = 0.0f;
      this->gas_resistance = 0.0f;
    }
  }

  // field types and members
  using _temperature_type =
    float;
  _temperature_type temperature;
  using _humidity_type =
    float;
  _humidity_type humidity;
  using _pressure_type =
    float;
  _pressure_type pressure;
  using _gas_resistance_type =
    float;
  _gas_resistance_type gas_resistance;

  // setters for named parameter idiom
  Type & set__temperature(
    const float & _arg)
  {
    this->temperature = _arg;
    return *this;
  }
  Type & set__humidity(
    const float & _arg)
  {
    this->humidity = _arg;
    return *this;
  }
  Type & set__pressure(
    const float & _arg)
  {
    this->pressure = _arg;
    return *this;
  }
  Type & set__gas_resistance(
    const float & _arg)
  {
    this->gas_resistance = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator> *;
  using ConstRawPtr =
    const ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ipas_interfaces__msg__IPASMeasurement
    std::shared_ptr<ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ipas_interfaces__msg__IPASMeasurement
    std::shared_ptr<ipas_interfaces::msg::IPASMeasurement_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const IPASMeasurement_ & other) const
  {
    if (this->temperature != other.temperature) {
      return false;
    }
    if (this->humidity != other.humidity) {
      return false;
    }
    if (this->pressure != other.pressure) {
      return false;
    }
    if (this->gas_resistance != other.gas_resistance) {
      return false;
    }
    return true;
  }
  bool operator!=(const IPASMeasurement_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct IPASMeasurement_

// alias to use template instance with default allocator
using IPASMeasurement =
  ipas_interfaces::msg::IPASMeasurement_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace ipas_interfaces

#endif  // IPAS_INTERFACES__MSG__DETAIL__IPAS_MEASUREMENT__STRUCT_HPP_
