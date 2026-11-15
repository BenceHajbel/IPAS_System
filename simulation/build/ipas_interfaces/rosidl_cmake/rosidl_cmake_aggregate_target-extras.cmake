# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target ipas_interfaces::ipas_interfaces
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${ipas_interfaces_TARGETS}.
if(ipas_interfaces_TARGETS AND NOT TARGET ipas_interfaces::ipas_interfaces)
  add_library(ipas_interfaces::ipas_interfaces INTERFACE IMPORTED)
  set_target_properties(ipas_interfaces::ipas_interfaces PROPERTIES
    INTERFACE_LINK_LIBRARIES "${ipas_interfaces_TARGETS}")
endif()
