# generated from rosidl_generator_py/resource/_idl.py.em
# with input from ipas_interfaces:msg/IPASMeasurement.idl
# generated code does not contain a copyright notice

from __future__ import annotations

import collections.abc
import os
import typing

import rosidl_pycommon.interface_base_classes

if typing.TYPE_CHECKING:
    from ctypes import Structure

    class PyCapsule(Structure):
        pass  # don't need to define the full structure


# This is being done at the module level and not on the instance level to avoid looking
# for the same variable multiple times on each instance. This variable is not supposed to
# change during runtime so it makes sense to only look for it once.
ros_python_check_fields = os.getenv('ROS_PYTHON_CHECK_FIELDS', default='')


# Import statements for member types

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_IPASMeasurement(rosidl_pycommon.interface_base_classes.MessageTypeSupportMeta):
    """Metaclass of message 'IPASMeasurement'."""

    _CREATE_ROS_MESSAGE: typing.ClassVar[typing.Optional[PyCapsule]] = None
    _CONVERT_FROM_PY: typing.ClassVar[typing.Optional[PyCapsule]] = None
    _CONVERT_TO_PY: typing.ClassVar[typing.Optional[PyCapsule]] = None
    _DESTROY_ROS_MESSAGE: typing.ClassVar[typing.Optional[PyCapsule]] = None
    _TYPE_SUPPORT: typing.ClassVar[typing.Optional[PyCapsule]] = None

    class IPASMeasurementConstants(typing.TypedDict):
        pass

    __constants: IPASMeasurementConstants = {
    }

    @classmethod
    def __import_type_support__(cls) -> None:
        try:
            from rosidl_generator_py import import_type_support  # type: ignore[attr-defined]
            module = import_type_support('ipas_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'ipas_interfaces.msg.IPASMeasurement')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__ipas_measurement
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__ipas_measurement
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__ipas_measurement
            cls._TYPE_SUPPORT = module.type_support_msg__msg__ipas_measurement
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__ipas_measurement

    @classmethod
    def __prepare__(metacls, name: str, bases: tuple[type[typing.Any], ...], /, **kwds: typing.Any) -> collections.abc.MutableMapping[str, object]:
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class IPASMeasurement(rosidl_pycommon.interface_base_classes.BaseMessage, metaclass=Metaclass_IPASMeasurement):
    """Message class 'IPASMeasurement'."""

    __slots__ = [
        '_temperature',
        '_humidity',
        '_pressure',
        '_gas_resistance',
        '_check_fields',
    ]

    _fields_and_field_types: dict[str, str] = {
        'temperature': 'float',
        'humidity': 'float',
        'pressure': 'float',
        'gas_resistance': 'float',
    }

    # This attribute is used to store an rosidl_parser.definition variable
    # related to the data type of each of the components the message.
    SLOT_TYPES: tuple[rosidl_parser.definition.AbstractType, ...] = (
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, *,
                 temperature: typing.Optional[float] = None,  # noqa: E501
                 humidity: typing.Optional[float] = None,  # noqa: E501
                 pressure: typing.Optional[float] = None,  # noqa: E501
                 gas_resistance: typing.Optional[float] = None,  # noqa: E501
                 check_fields: typing.Optional[bool] = None) -> None:
        if check_fields is not None:
            self._check_fields = check_fields
        else:
            self._check_fields = ros_python_check_fields == '1'
        self.temperature = temperature if temperature is not None else float()
        self.humidity = humidity if humidity is not None else float()
        self.pressure = pressure if pressure is not None else float()
        self.gas_resistance = gas_resistance if gas_resistance is not None else float()

    def __repr__(self) -> str:
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args: list[str] = []
        for s, t in zip(self.get_fields_and_field_types().keys(), self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    from rosidl_buffer import Buffer as _RosidlBuffer
                    if not isinstance(field, _RosidlBuffer):
                        if self._check_fields:
                            assert fieldstr.startswith('array(')
                        prefix = "array('X', "
                        suffix = ')'
                        fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, IPASMeasurement):
            return False
        if self.temperature != other.temperature:
            return False
        if self.humidity != other.humidity:
            return False
        if self.pressure != other.pressure:
            return False
        if self.gas_resistance != other.gas_resistance:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls) -> dict[str, str]:
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def temperature(self) -> float:
        """Message field 'temperature'."""
        return self._temperature

    @temperature.setter
    def temperature(self, value: float) -> None:

        if self._check_fields:
            if False:  # Done for templating alignment
                pass
            else:
                assert \
                    isinstance(value, float), \
                    "The 'temperature' field must be of type 'float'"
                assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                    "The 'temperature' field must be a float in [-3.402823466e+38, 3.402823466e+38]"

        self._temperature = value

    @builtins.property
    def humidity(self) -> float:
        """Message field 'humidity'."""
        return self._humidity

    @humidity.setter
    def humidity(self, value: float) -> None:

        if self._check_fields:
            if False:  # Done for templating alignment
                pass
            else:
                assert \
                    isinstance(value, float), \
                    "The 'humidity' field must be of type 'float'"
                assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                    "The 'humidity' field must be a float in [-3.402823466e+38, 3.402823466e+38]"

        self._humidity = value

    @builtins.property
    def pressure(self) -> float:
        """Message field 'pressure'."""
        return self._pressure

    @pressure.setter
    def pressure(self, value: float) -> None:

        if self._check_fields:
            if False:  # Done for templating alignment
                pass
            else:
                assert \
                    isinstance(value, float), \
                    "The 'pressure' field must be of type 'float'"
                assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                    "The 'pressure' field must be a float in [-3.402823466e+38, 3.402823466e+38]"

        self._pressure = value

    @builtins.property
    def gas_resistance(self) -> float:
        """Message field 'gas_resistance'."""
        return self._gas_resistance

    @gas_resistance.setter
    def gas_resistance(self, value: float) -> None:

        if self._check_fields:
            if False:  # Done for templating alignment
                pass
            else:
                assert \
                    isinstance(value, float), \
                    "The 'gas_resistance' field must be of type 'float'"
                assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                    "The 'gas_resistance' field must be a float in [-3.402823466e+38, 3.402823466e+38]"

        self._gas_resistance = value
