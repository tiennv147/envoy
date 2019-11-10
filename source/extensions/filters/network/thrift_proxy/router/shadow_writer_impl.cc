#include "extensions/filters/network/thrift_proxy/router/shadow_writer_impl.h"

#include <chrono>
#include <string>

#include "common/common/assert.h"
#include "common/http/headers.h"

#include "absl/strings/str_join.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace ThriftProxy {
namespace Router {

void ShadowWriterImpl::shadow(const std::string& cluster, /*Http::MessagePtr&& request, */
                              std::chrono::milliseconds timeout) {
  // It's possible that the cluster specified in the route configuration no longer exists due
  // to a CDS removal. Check that it still exists before shadowing.
  // TODO(mattklein123): Optimally we would have a stat but for now just fix the crashing issue.
  if (!cm_.get(cluster)) {
    ENVOY_LOG(debug, "shadow cluster '{}' does not exist", cluster);
    return;
  }

  // This is basically fire and forget. We don't handle cancelling.
  // TODO(dereka) figure out how this block works for Thrift.
  // cm_.httpAsyncClientForCluster(cluster).send(
  //     std::move(request), *this, Http::AsyncClient::RequestOptions().setTimeout(timeout));
}

} // namespace Router
} // namespace ThriftProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
