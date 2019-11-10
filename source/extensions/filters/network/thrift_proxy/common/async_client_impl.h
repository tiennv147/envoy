#pragma once

#include "source/extensions/filters/network/thrift_proxy/common/async_client.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace ThriftProxy {
namespace Common {

class AsyncClientImpl : public AsyncClient {
public:
  AsyncClientImpl(Upstream::ClusterManager& cluster_manager);

private:
  Upstream::ClusterManager& cluster_manager_;
};

class AsyncRequestImpl : public AsyncRequest, AsyncCallbacks {
public:
private:

};

} // namespace Common
} // namespace ThriftProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
