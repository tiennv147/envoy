#include "extensions/filters/network/memcached_proxy/utility.h"

#include <string>

#include "envoy/buffer/buffer.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

std::string BufferHelper::drainString(Buffer::Instance& data, uint32_t length) {
  if (length > 0) {
    char* start = reinterpret_cast<char*>(data.linearize(length));
    std::string ret(start, length);
    data.drain(length);
    return ret;
  }

  return "";
}

} // namespace MemcachedProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
