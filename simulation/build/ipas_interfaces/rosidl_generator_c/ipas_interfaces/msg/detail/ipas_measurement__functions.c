// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from ipas_interfaces:msg/IPASMeasurement.idl
// generated code does not contain a copyright notice
#include "ipas_interfaces/msg/detail/ipas_measurement__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
ipas_interfaces__msg__IPASMeasurement__init(ipas_interfaces__msg__IPASMeasurement * msg)
{
  if (!msg) {
    return false;
  }
  // temperature
  // humidity
  // pressure
  // gas_resistance
  return true;
}

void
ipas_interfaces__msg__IPASMeasurement__fini(ipas_interfaces__msg__IPASMeasurement * msg)
{
  if (!msg) {
    return;
  }
  // temperature
  // humidity
  // pressure
  // gas_resistance
}

bool
ipas_interfaces__msg__IPASMeasurement__are_equal(const ipas_interfaces__msg__IPASMeasurement * lhs, const ipas_interfaces__msg__IPASMeasurement * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // temperature
  if (lhs->temperature != rhs->temperature) {
    return false;
  }
  // humidity
  if (lhs->humidity != rhs->humidity) {
    return false;
  }
  // pressure
  if (lhs->pressure != rhs->pressure) {
    return false;
  }
  // gas_resistance
  if (lhs->gas_resistance != rhs->gas_resistance) {
    return false;
  }
  return true;
}

bool
ipas_interfaces__msg__IPASMeasurement__copy(
  const ipas_interfaces__msg__IPASMeasurement * input,
  ipas_interfaces__msg__IPASMeasurement * output)
{
  if (!input || !output) {
    return false;
  }
  // temperature
  output->temperature = input->temperature;
  // humidity
  output->humidity = input->humidity;
  // pressure
  output->pressure = input->pressure;
  // gas_resistance
  output->gas_resistance = input->gas_resistance;
  return true;
}

ipas_interfaces__msg__IPASMeasurement *
ipas_interfaces__msg__IPASMeasurement__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ipas_interfaces__msg__IPASMeasurement * msg = (ipas_interfaces__msg__IPASMeasurement *)allocator.allocate(sizeof(ipas_interfaces__msg__IPASMeasurement), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(ipas_interfaces__msg__IPASMeasurement));
  bool success = ipas_interfaces__msg__IPASMeasurement__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
ipas_interfaces__msg__IPASMeasurement__destroy(ipas_interfaces__msg__IPASMeasurement * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    ipas_interfaces__msg__IPASMeasurement__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
ipas_interfaces__msg__IPASMeasurement__Sequence__init(ipas_interfaces__msg__IPASMeasurement__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ipas_interfaces__msg__IPASMeasurement * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(ipas_interfaces__msg__IPASMeasurement)) {
      return false;
    }
    data = (ipas_interfaces__msg__IPASMeasurement *)allocator.zero_allocate(size, sizeof(ipas_interfaces__msg__IPASMeasurement), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = ipas_interfaces__msg__IPASMeasurement__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        ipas_interfaces__msg__IPASMeasurement__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
ipas_interfaces__msg__IPASMeasurement__Sequence__fini(ipas_interfaces__msg__IPASMeasurement__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      ipas_interfaces__msg__IPASMeasurement__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

ipas_interfaces__msg__IPASMeasurement__Sequence *
ipas_interfaces__msg__IPASMeasurement__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  ipas_interfaces__msg__IPASMeasurement__Sequence * array = (ipas_interfaces__msg__IPASMeasurement__Sequence *)allocator.allocate(sizeof(ipas_interfaces__msg__IPASMeasurement__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = ipas_interfaces__msg__IPASMeasurement__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
ipas_interfaces__msg__IPASMeasurement__Sequence__destroy(ipas_interfaces__msg__IPASMeasurement__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    ipas_interfaces__msg__IPASMeasurement__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
ipas_interfaces__msg__IPASMeasurement__Sequence__are_equal(const ipas_interfaces__msg__IPASMeasurement__Sequence * lhs, const ipas_interfaces__msg__IPASMeasurement__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!ipas_interfaces__msg__IPASMeasurement__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
ipas_interfaces__msg__IPASMeasurement__Sequence__copy(
  const ipas_interfaces__msg__IPASMeasurement__Sequence * input,
  ipas_interfaces__msg__IPASMeasurement__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(ipas_interfaces__msg__IPASMeasurement)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(ipas_interfaces__msg__IPASMeasurement);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    ipas_interfaces__msg__IPASMeasurement * data =
      (ipas_interfaces__msg__IPASMeasurement *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!ipas_interfaces__msg__IPASMeasurement__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          ipas_interfaces__msg__IPASMeasurement__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!ipas_interfaces__msg__IPASMeasurement__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
