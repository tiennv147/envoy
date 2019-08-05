.. _config_network_filters_redis_proxy:

Memcached proxy
===========

* Memcached :ref:`architecture overview <arch_overview_memcached>`
* :ref:`v2 API reference <envoy_api_msg_config.filter.network.memcached_proxy.v1alpha1.MemcachedProxy>`
* This filter should be configured with the name *envoy.memcached_proxy*.

.. _config_network_filters_memcached_proxy_stats:

Statistics
----------

Every configured Memcached proxy filter has statistics rooted at *memcached.<stat_prefix>.* with the
following statistics:

.. csv-table::
  :header: Name, Type, Description
  :widths: 1, 1, 2

  downstream_cx_active, Gauge, Total active connections
  downstream_cx_protocol_error, Counter, Total protocol errors
  downstream_cx_rx_bytes_buffered, Gauge, Total received bytes currently buffered
  downstream_cx_rx_bytes_total, Counter, Total bytes received
  downstream_cx_total, Counter, Total connections
  downstream_cx_tx_bytes_buffered, Gauge, Total sent bytes currently buffered
  downstream_cx_tx_bytes_total, Counter, Total bytes sent
  downstream_cx_drain_close, Counter, Number of connections closed due to draining
  downstream_rq_active, Gauge, Total active requests
  downstream_rq_total, Counter, Total requests


Per command statistics
----------------------

The Memcached filter will gather statistics for commands in the
*mecached.<stat_prefix>.command.<command>.* namespace. By default latency stats are in milliseconds and can be
changed to microseconds by setting the configuration parameter :ref:`latency_in_micros <envoy_api_field_config.filter.network.memcached_proxy.v2.MemcachedProxy.latency_in_micros>` to true.

.. csv-table::
  :header: Name, Type, Description
  :widths: 1, 1, 2

  total, Counter, Number of commands
  success, Counter, Number of commands that were successful
  error, Counter, Number of commands that returned a partial or complete error response
  latency, Histogram, Command execution time in milliseconds

.. _config_network_filters_redis_proxy_per_command_stats:
