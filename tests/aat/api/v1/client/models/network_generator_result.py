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


class NetworkGeneratorResult(object):
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
        'id': 'str',
        'generator_id': 'str',
        'active': 'bool',
        'timestamp_first': 'datetime',
        'timestamp_last': 'datetime',
        'read': 'NetworkGeneratorStats',
        'write': 'NetworkGeneratorStats',
        'dynamic_results': 'DynamicResults'
    }

    attribute_map = {
        'id': 'id',
        'generator_id': 'generator_id',
        'active': 'active',
        'timestamp_first': 'timestamp_first',
        'timestamp_last': 'timestamp_last',
        'read': 'read',
        'write': 'write',
        'dynamic_results': 'dynamic_results'
    }

    def __init__(self, id=None, generator_id=None, active=None, timestamp_first=None, timestamp_last=None, read=None, write=None, dynamic_results=None):  # noqa: E501
        """NetworkGeneratorResult - a model defined in Swagger"""  # noqa: E501

        self._id = None
        self._generator_id = None
        self._active = None
        self._timestamp_first = None
        self._timestamp_last = None
        self._read = None
        self._write = None
        self._dynamic_results = None
        self.discriminator = None

        self.id = id
        if generator_id is not None:
            self.generator_id = generator_id
        self.active = active
        self.timestamp_first = timestamp_first
        self.timestamp_last = timestamp_last
        self.read = read
        self.write = write
        if dynamic_results is not None:
            self.dynamic_results = dynamic_results

    @property
    def id(self):
        """Gets the id of this NetworkGeneratorResult.  # noqa: E501

        Unique generator result identifier  # noqa: E501

        :return: The id of this NetworkGeneratorResult.  # noqa: E501
        :rtype: str
        """
        return self._id

    @id.setter
    def id(self, id):
        """Sets the id of this NetworkGeneratorResult.

        Unique generator result identifier  # noqa: E501

        :param id: The id of this NetworkGeneratorResult.  # noqa: E501
        :type: str
        """
        self._id = id

    @property
    def generator_id(self):
        """Gets the generator_id of this NetworkGeneratorResult.  # noqa: E501

        Network generator identifier that generated this result  # noqa: E501

        :return: The generator_id of this NetworkGeneratorResult.  # noqa: E501
        :rtype: str
        """
        return self._generator_id

    @generator_id.setter
    def generator_id(self, generator_id):
        """Sets the generator_id of this NetworkGeneratorResult.

        Network generator identifier that generated this result  # noqa: E501

        :param generator_id: The generator_id of this NetworkGeneratorResult.  # noqa: E501
        :type: str
        """
        self._generator_id = generator_id

    @property
    def active(self):
        """Gets the active of this NetworkGeneratorResult.  # noqa: E501

        Indicates whether the result is currently being updated  # noqa: E501

        :return: The active of this NetworkGeneratorResult.  # noqa: E501
        :rtype: bool
        """
        return self._active

    @active.setter
    def active(self, active):
        """Sets the active of this NetworkGeneratorResult.

        Indicates whether the result is currently being updated  # noqa: E501

        :param active: The active of this NetworkGeneratorResult.  # noqa: E501
        :type: bool
        """
        self._active = active

    @property
    def timestamp_first(self):
        """Gets the timestamp_first of this NetworkGeneratorResult.  # noqa: E501

        The ISO8601-formatted start time of the generator  # noqa: E501

        :return: The timestamp_first of this NetworkGeneratorResult.  # noqa: E501
        :rtype: datetime
        """
        return self._timestamp_first

    @timestamp_first.setter
    def timestamp_first(self, timestamp_first):
        """Sets the timestamp_first of this NetworkGeneratorResult.

        The ISO8601-formatted start time of the generator  # noqa: E501

        :param timestamp_first: The timestamp_first of this NetworkGeneratorResult.  # noqa: E501
        :type: datetime
        """
        self._timestamp_first = timestamp_first

    @property
    def timestamp_last(self):
        """Gets the timestamp_last of this NetworkGeneratorResult.  # noqa: E501

        The ISO8601-formatted date of the last result update  # noqa: E501

        :return: The timestamp_last of this NetworkGeneratorResult.  # noqa: E501
        :rtype: datetime
        """
        return self._timestamp_last

    @timestamp_last.setter
    def timestamp_last(self, timestamp_last):
        """Sets the timestamp_last of this NetworkGeneratorResult.

        The ISO8601-formatted date of the last result update  # noqa: E501

        :param timestamp_last: The timestamp_last of this NetworkGeneratorResult.  # noqa: E501
        :type: datetime
        """
        self._timestamp_last = timestamp_last

    @property
    def read(self):
        """Gets the read of this NetworkGeneratorResult.  # noqa: E501


        :return: The read of this NetworkGeneratorResult.  # noqa: E501
        :rtype: NetworkGeneratorStats
        """
        return self._read

    @read.setter
    def read(self, read):
        """Sets the read of this NetworkGeneratorResult.


        :param read: The read of this NetworkGeneratorResult.  # noqa: E501
        :type: NetworkGeneratorStats
        """
        self._read = read

    @property
    def write(self):
        """Gets the write of this NetworkGeneratorResult.  # noqa: E501


        :return: The write of this NetworkGeneratorResult.  # noqa: E501
        :rtype: NetworkGeneratorStats
        """
        return self._write

    @write.setter
    def write(self, write):
        """Sets the write of this NetworkGeneratorResult.


        :param write: The write of this NetworkGeneratorResult.  # noqa: E501
        :type: NetworkGeneratorStats
        """
        self._write = write

    @property
    def dynamic_results(self):
        """Gets the dynamic_results of this NetworkGeneratorResult.  # noqa: E501


        :return: The dynamic_results of this NetworkGeneratorResult.  # noqa: E501
        :rtype: DynamicResults
        """
        return self._dynamic_results

    @dynamic_results.setter
    def dynamic_results(self, dynamic_results):
        """Sets the dynamic_results of this NetworkGeneratorResult.


        :param dynamic_results: The dynamic_results of this NetworkGeneratorResult.  # noqa: E501
        :type: DynamicResults
        """
        self._dynamic_results = dynamic_results

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
        if issubclass(NetworkGeneratorResult, dict):
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
        if not isinstance(other, NetworkGeneratorResult):
            return False

        return self.__dict__ == other.__dict__

    def __ne__(self, other):
        """Returns true if both objects are not equal"""
        return not self == other
