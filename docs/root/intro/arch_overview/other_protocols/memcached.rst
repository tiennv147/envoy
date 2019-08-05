.. _arch_overview_memcached:

Memcached
=======

Envoy can act as a Memcached proxy for both the binary and ASCII versions of the protocol.

The Redis project offers a thorough reference on partitioning as it relates to Redis. See
"`Partitioning: how to split data among multiple Redis instances
<https://redis.io/topics/partitioning>`_".

**Features of Envoy Memcached**:

* TODO

**Planned future enhancements**:

* Prefix-based routing.
* Circuit breaking.
* Health checking.
* Replicated pools.
* Broadcast operations.
* Cold cache warm-up.
* Multi-level caches.
* Built-in retry.
* Tracing.

.. _arch_overview_redis_configuration:

Configuration
-------------

For filter configuration details, see the Memcached proxy filter
:ref:`configuration reference <config_network_filters_memcached_proxy>`.

The corresponding cluster definition should be configured with
:ref:`ring hash load balancing <envoy_api_field_Cluster.lb_policy>`.

Supported Protcols
------------------

Envoy Memcached supports the entirety of the ASCII protocol, and all the frozen opcodes of the binary protocol.

Failure modes
-------------

If Redis throws an error, we pass that error along as the response to the command. Envoy treats a
response from Redis with the error datatype as a normal response and passes it through to the
caller.

Envoy can also generate its own errors in response to the client.

.. csv-table::
  :header: Error, Meaning
  :widths: 1, 1

  no upstream host, "The ring hash load balancer did not have a healthy host available at the
  ring position chosen for the key."
  upstream failure, "The backend did not respond within the timeout period or closed
  the connection."
  invalid request, "Command was rejected by the first stage of the command splitter due to
  datatype or length."
  unsupported command, "The command was not recognized by Envoy and therefore cannot be serviced
  because it cannot be hashed to a backend server."
  finished with n errors, "Fragmented commands which sum the response (e.g. DEL) will return the
  total number of errors received if any were received."
  upstream protocol error, "A fragmented command received an unexpected datatype or a backend
  responded with a response that not conform to the Redis protocol."
  wrong number of arguments for command, "Certain commands check in Envoy that the number of
  arguments is correct."
  "NOAUTH Authentication required.", "The command was rejected because a downstream authentication
  password has been set and the client has not successfully authenticated."
  ERR invalid password, "The authentication command failed due to an invalid password."
  "ERR Client sent AUTH, but no password is set", "An authentication command was received, but no
  downstream authentication password has been configured."


In the case of MGET, each individual key that cannot be fetched will generate an error response.
For example, if we fetch five keys and two of the keys' backends time out, we would get an error
response for each in place of the value.

.. code-block:: none

  $ redis-cli MGET a b c d e
  1) "alpha"
  2) "bravo"
  3) (error) upstream failure
  4) (error) upstream failure
  5) "echo"
