# coding: utf-8

"""
    OpenPerf API

    REST API interface for OpenPerf  # noqa: E501

    OpenAPI spec version: 1
    Contact: support@spirent.com
    Generated by: https://github.com/swagger-api/swagger-codegen.git
"""


import pprint
import re  # noqa: F401

import six


class PacketCaptureConfig(object):
    """NOTE: This class is auto generated by the swagger code generator program.

    Do not edit the class manually.
    """

    """
    Attributes:
      swagger_types (dict): The key is attribute name
                            and the value is attribute type.
      attribute_map (dict): The key is attribute name
                            and the value is json key in definition.
    """
    swagger_types = {
        'mode': 'str',
        'buffer_wrap': 'bool',
        'buffer_size': 'int',
        'packet_size': 'int',
        'filter': 'str',
        'start_trigger': 'str',
        'stop_trigger': 'str',
        'duration': 'int'
    }

    attribute_map = {
        'mode': 'mode',
        'buffer_wrap': 'buffer_wrap',
        'buffer_size': 'buffer_size',
        'packet_size': 'packet_size',
        'filter': 'filter',
        'start_trigger': 'start_trigger',
        'stop_trigger': 'stop_trigger',
        'duration': 'duration'
    }

    def __init__(self, mode='buffer', buffer_wrap=False, buffer_size=16777216, packet_size=None, filter=None, start_trigger=None, stop_trigger=None, duration=None):  # noqa: E501
        """PacketCaptureConfig - a model defined in Swagger"""  # noqa: E501

        self._mode = None
        self._buffer_wrap = None
        self._buffer_size = None
        self._packet_size = None
        self._filter = None
        self._start_trigger = None
        self._stop_trigger = None
        self._duration = None
        self.discriminator = None

        self.mode = mode
        if buffer_wrap is not None:
            self.buffer_wrap = buffer_wrap
        self.buffer_size = buffer_size
        if packet_size is not None:
            self.packet_size = packet_size
        if filter is not None:
            self.filter = filter
        if start_trigger is not None:
            self.start_trigger = start_trigger
        if stop_trigger is not None:
            self.stop_trigger = stop_trigger
        if duration is not None:
            self.duration = duration

    @property
    def mode(self):
        """Gets the mode of this PacketCaptureConfig.  # noqa: E501

        Capture mode  # noqa: E501

        :return: The mode of this PacketCaptureConfig.  # noqa: E501
        :rtype: str
        """
        return self._mode

    @mode.setter
    def mode(self, mode):
        """Sets the mode of this PacketCaptureConfig.

        Capture mode  # noqa: E501

        :param mode: The mode of this PacketCaptureConfig.  # noqa: E501
        :type: str
        """
        self._mode = mode

    @property
    def buffer_wrap(self):
        """Gets the buffer_wrap of this PacketCaptureConfig.  # noqa: E501

        Indicates whether capture wraps when it reaches the end of the buffer.  When buffer wrap is enabled capture will continue until capture is stopped with the stop command or a stop trigger.   # noqa: E501

        :return: The buffer_wrap of this PacketCaptureConfig.  # noqa: E501
        :rtype: bool
        """
        return self._buffer_wrap

    @buffer_wrap.setter
    def buffer_wrap(self, buffer_wrap):
        """Sets the buffer_wrap of this PacketCaptureConfig.

        Indicates whether capture wraps when it reaches the end of the buffer.  When buffer wrap is enabled capture will continue until capture is stopped with the stop command or a stop trigger.   # noqa: E501

        :param buffer_wrap: The buffer_wrap of this PacketCaptureConfig.  # noqa: E501
        :type: bool
        """
        self._buffer_wrap = buffer_wrap

    @property
    def buffer_size(self):
        """Gets the buffer_size of this PacketCaptureConfig.  # noqa: E501

        Capture buffer size in bytes.  # noqa: E501

        :return: The buffer_size of this PacketCaptureConfig.  # noqa: E501
        :rtype: int
        """
        return self._buffer_size

    @buffer_size.setter
    def buffer_size(self, buffer_size):
        """Sets the buffer_size of this PacketCaptureConfig.

        Capture buffer size in bytes.  # noqa: E501

        :param buffer_size: The buffer_size of this PacketCaptureConfig.  # noqa: E501
        :type: int
        """
        self._buffer_size = buffer_size

    @property
    def packet_size(self):
        """Gets the packet_size of this PacketCaptureConfig.  # noqa: E501

        Maximum length of packet to capture. If the packet is larger than the packet size, the packet is truncated.  # noqa: E501

        :return: The packet_size of this PacketCaptureConfig.  # noqa: E501
        :rtype: int
        """
        return self._packet_size

    @packet_size.setter
    def packet_size(self, packet_size):
        """Sets the packet_size of this PacketCaptureConfig.

        Maximum length of packet to capture. If the packet is larger than the packet size, the packet is truncated.  # noqa: E501

        :param packet_size: The packet_size of this PacketCaptureConfig.  # noqa: E501
        :type: int
        """
        self._packet_size = packet_size

    @property
    def filter(self):
        """Gets the filter of this PacketCaptureConfig.  # noqa: E501

        Berkley Packet Filter (BPF) rules that matches packets to capture.  An empty rule, the default, matches all frames.   # noqa: E501

        :return: The filter of this PacketCaptureConfig.  # noqa: E501
        :rtype: str
        """
        return self._filter

    @filter.setter
    def filter(self, filter):
        """Sets the filter of this PacketCaptureConfig.

        Berkley Packet Filter (BPF) rules that matches packets to capture.  An empty rule, the default, matches all frames.   # noqa: E501

        :param filter: The filter of this PacketCaptureConfig.  # noqa: E501
        :type: str
        """
        self._filter = filter

    @property
    def start_trigger(self):
        """Gets the start_trigger of this PacketCaptureConfig.  # noqa: E501

        Berkley Packet Filter (BPF) rules used to trigger the start of packet capture.  When a trigger condition is specified, the capture start command puts capture into an armed state and capture will only begin when the trigger condition occurs.   # noqa: E501

        :return: The start_trigger of this PacketCaptureConfig.  # noqa: E501
        :rtype: str
        """
        return self._start_trigger

    @start_trigger.setter
    def start_trigger(self, start_trigger):
        """Sets the start_trigger of this PacketCaptureConfig.

        Berkley Packet Filter (BPF) rules used to trigger the start of packet capture.  When a trigger condition is specified, the capture start command puts capture into an armed state and capture will only begin when the trigger condition occurs.   # noqa: E501

        :param start_trigger: The start_trigger of this PacketCaptureConfig.  # noqa: E501
        :type: str
        """
        self._start_trigger = start_trigger

    @property
    def stop_trigger(self):
        """Gets the stop_trigger of this PacketCaptureConfig.  # noqa: E501

        Berkley Packet Filter (BPF) rules used to trigger the stop of packet capture.   # noqa: E501

        :return: The stop_trigger of this PacketCaptureConfig.  # noqa: E501
        :rtype: str
        """
        return self._stop_trigger

    @stop_trigger.setter
    def stop_trigger(self, stop_trigger):
        """Sets the stop_trigger of this PacketCaptureConfig.

        Berkley Packet Filter (BPF) rules used to trigger the stop of packet capture.   # noqa: E501

        :param stop_trigger: The stop_trigger of this PacketCaptureConfig.  # noqa: E501
        :type: str
        """
        self._stop_trigger = stop_trigger

    @property
    def duration(self):
        """Gets the duration of this PacketCaptureConfig.  # noqa: E501

        Maximum time duration for the capture in msec.   # noqa: E501

        :return: The duration of this PacketCaptureConfig.  # noqa: E501
        :rtype: int
        """
        return self._duration

    @duration.setter
    def duration(self, duration):
        """Sets the duration of this PacketCaptureConfig.

        Maximum time duration for the capture in msec.   # noqa: E501

        :param duration: The duration of this PacketCaptureConfig.  # noqa: E501
        :type: int
        """
        self._duration = duration

    def to_dict(self):
        """Returns the model properties as a dict"""
        result = {}

        for attr, _ in six.iteritems(self.swagger_types):
            value = getattr(self, attr)
            if isinstance(value, list):
                result[attr] = list(map(
                    lambda x: x.to_dict() if hasattr(x, "to_dict") else x,
                    value
                ))
            elif hasattr(value, "to_dict"):
                result[attr] = value.to_dict()
            elif isinstance(value, dict):
                result[attr] = dict(map(
                    lambda item: (item[0], item[1].to_dict())
                    if hasattr(item[1], "to_dict") else item,
                    value.items()
                ))
            else:
                result[attr] = value
        if issubclass(PacketCaptureConfig, dict):
            for key, value in self.items():
                result[key] = value

        return result

    def to_str(self):
        """Returns the string representation of the model"""
        return pprint.pformat(self.to_dict())

    def __repr__(self):
        """For `print` and `pprint`"""
        return self.to_str()

    def __eq__(self, other):
        """Returns true if both objects are equal"""
        if not isinstance(other, PacketCaptureConfig):
            return False

        return self.__dict__ == other.__dict__

    def __ne__(self, other):
        """Returns true if both objects are not equal"""
        return not self == other
