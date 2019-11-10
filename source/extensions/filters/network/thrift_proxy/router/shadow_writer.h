#pragma once

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
class ShadowWriter {
public:
  virtual ~ShadowWriter() = default;

  /**
   * Shadow a request.
   * @param cluster supplies the cluster name to shadow to.
   * @param message supplies the complete request to shadow.
   * @param timeout supplies the shadowed request timeout.
   */
  virtual void shadow(const std::string& cluster, Http::MessagePtr&& request,
                      std::chrono::milliseconds timeout) PURE;
};

using ShadowWriterPtr = std::unique_ptr<ShadowWriter>;

} // namespace Router
} // namespace ThriftProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
