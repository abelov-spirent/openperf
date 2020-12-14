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


class NetworkServer(object):
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
        'port': 'int',
        'protocol': 'str',
        'interface': 'str',
        'address_family': 'str',
        'stats': 'NetworkServerStats'
    }

    attribute_map = {
        'id': 'id',
        'port': 'port',
        'protocol': 'protocol',
        'interface': 'interface',
        'address_family': 'address_family',
        'stats': 'stats'
    }

    def __init__(self, id=None, port=None, protocol=None, interface=None, address_family=None, stats=None):  # noqa: E501
        """NetworkServer - a model defined in Swagger"""  # noqa: E501

        self._id = None
        self._port = None
        self._protocol = None
        self._interface = None
        self._address_family = None
        self._stats = None
        self.discriminator = None

        self.id = id
        self.port = port
        self.protocol = protocol
        if interface is not None:
            self.interface = interface
        if address_family is not None:
            self.address_family = address_family
        self.stats = stats

    @property
    def id(self):
        """Gets the id of this NetworkServer.  # noqa: E501

        Unique network server identifier  # noqa: E501

        :return: The id of this NetworkServer.  # noqa: E501
        :rtype: str
        """
        return self._id

    @id.setter
    def id(self, id):
        """Sets the id of this NetworkServer.

        Unique network server identifier  # noqa: E501

        :param id: The id of this NetworkServer.  # noqa: E501
        :type: str
        """
        self._id = id

    @property
    def port(self):
        """Gets the port of this NetworkServer.  # noqa: E501

        UDP/TCP port on which to listen for client connections  # noqa: E501

        :return: The port of this NetworkServer.  # noqa: E501
        :rtype: int
        """
        return self._port

    @port.setter
    def port(self, port):
        """Sets the port of this NetworkServer.

        UDP/TCP port on which to listen for client connections  # noqa: E501

        :param port: The port of this NetworkServer.  # noqa: E501
        :type: int
        """
        self._port = port

    @property
    def protocol(self):
        """Gets the protocol of this NetworkServer.  # noqa: E501

        Protocol which is used to establish client connections  # noqa: E501

        :return: The protocol of this NetworkServer.  # noqa: E501
        :rtype: str
        """
        return self._protocol

    @protocol.setter
    def protocol(self, protocol):
        """Sets the protocol of this NetworkServer.

        Protocol which is used to establish client connections  # noqa: E501

        :param protocol: The protocol of this NetworkServer.  # noqa: E501
        :type: str
        """
        self._protocol = protocol

    @property
    def interface(self):
        """Gets the interface of this NetworkServer.  # noqa: E501

        Bind server socket to a particular device, specified as interface name (required for dpdk driver)  # noqa: E501

        :return: The interface of this NetworkServer.  # noqa: E501
        :rtype: str
        """
        return self._interface

    @interface.setter
    def interface(self, interface):
        """Sets the interface of this NetworkServer.

        Bind server socket to a particular device, specified as interface name (required for dpdk driver)  # noqa: E501

        :param interface: The interface of this NetworkServer.  # noqa: E501
        :type: str
        """
        self._interface = interface

    @property
    def address_family(self):
        """Gets the address_family of this NetworkServer.  # noqa: E501

        Specifies the addressing scheme that an instance of the Socket class can use. (required for dpdk driver) * inet - IPv4 support * inet6 - IPv6 support   # noqa: E501

        :return: The address_family of this NetworkServer.  # noqa: E501
        :rtype: str
        """
        return self._address_family

    @address_family.setter
    def address_family(self, address_family):
        """Sets the address_family of this NetworkServer.

        Specifies the addressing scheme that an instance of the Socket class can use. (required for dpdk driver) * inet - IPv4 support * inet6 - IPv6 support   # noqa: E501

        :param address_family: The address_family of this NetworkServer.  # noqa: E501
        :type: str
        """
        self._address_family = address_family

    @property
    def stats(self):
        """Gets the stats of this NetworkServer.  # noqa: E501


        :return: The stats of this NetworkServer.  # noqa: E501
        :rtype: NetworkServerStats
        """
        return self._stats

    @stats.setter
    def stats(self, stats):
        """Sets the stats of this NetworkServer.


        :param stats: The stats of this NetworkServer.  # noqa: E501
        :type: NetworkServerStats
        """
        self._stats = stats

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
        if issubclass(NetworkServer, dict):
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
        if not isinstance(other, NetworkServer):
            return False

        return self.__dict__ == other.__dict__

    def __ne__(self, other):
        """Returns true if both objects are not equal"""
        return not self == other
