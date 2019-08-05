#include "extensions/filters/network/memcached_proxy/ascii_codec_impl.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

void AsciiDecoderImpl::decode(Buffer::Instance& data) {
  ENVOY_LOG(trace, "decoding {} bytes", data.length());
}

} // namespace Envoy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
