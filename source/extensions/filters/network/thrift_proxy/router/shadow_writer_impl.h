#pragma once

#include "extensions/filters/network/thrift_proxy/router/shadow_writer.h"

#include <chrono>
#include <memory>
#include <string>

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace ThriftProxy {
namespace Router {

/**
 * Interface used to shadow complete requests to an alternate upstream cluster in a "fire and
 * forget" fashion.
 */
class ShadowWriterImpl : Logger::Loggable<Logger::Id::thrift>, public ShadowWriter {
public:
  ShadowWriterImpl(Upstream::ClusterManager& cm) : cm_(cm) {}

  virtual void shadow(const std::string& cluster, Http::MessagePtr&& request,
                      std::chrono::milliseconds timeout) override;

  // Callbacks
private:
  Upstream::ClusterManager& cm_;
};

using ShadowWriterPtr = std::unique_ptr<ShadowWriter>;

} // namespace Router
} // namespace ThriftProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
