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


class TvlpProfileNetworkSeries(object):
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
        'length': 'int',
        'config': 'NetworkGeneratorConfig'
    }

    attribute_map = {
        'length': 'length',
        'config': 'config'
    }

    def __init__(self, length=None, config=None):  # noqa: E501
        """TvlpProfileNetworkSeries - a model defined in Swagger"""  # noqa: E501

        self._length = None
        self._config = None
        self.discriminator = None

        self.length = length
        self.config = config

    @property
    def length(self):
        """Gets the length of this TvlpProfileNetworkSeries.  # noqa: E501

        Length of the current operation in ns  # noqa: E501

        :return: The length of this TvlpProfileNetworkSeries.  # noqa: E501
        :rtype: int
        """
        return self._length

    @length.setter
    def length(self, length):
        """Sets the length of this TvlpProfileNetworkSeries.

        Length of the current operation in ns  # noqa: E501

        :param length: The length of this TvlpProfileNetworkSeries.  # noqa: E501
        :type: int
        """
        self._length = length

    @property
    def config(self):
        """Gets the config of this TvlpProfileNetworkSeries.  # noqa: E501


        :return: The config of this TvlpProfileNetworkSeries.  # noqa: E501
        :rtype: NetworkGeneratorConfig
        """
        return self._config

    @config.setter
    def config(self, config):
        """Sets the config of this TvlpProfileNetworkSeries.


        :param config: The config of this TvlpProfileNetworkSeries.  # noqa: E501
        :type: NetworkGeneratorConfig
        """
        self._config = config

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
        if issubclass(TvlpProfileNetworkSeries, dict):
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
        if not isinstance(other, TvlpProfileNetworkSeries):
            return False

        return self.__dict__ == other.__dict__

    def __ne__(self, other):
        """Returns true if both objects are not equal"""
        return not self == other
