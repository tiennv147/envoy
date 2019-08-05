#pragma once

#include "common/common/logger.h"

#include "extensions/filters/network/memcached_proxy/codec.h"

#include "envoy/buffer/buffer.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

class AsciiDecoderImpl : public Decoder, Logger::Loggable<Logger::Id::memcached> {
public:
  // explicit AsciiDecoderImpl(DecoderCallbacks& callbacks) : callbacks_(callbacks) {}

  // MemcachedProxy::Decoder
  void decode(Buffer::Instance& data) override;
private:
  // DecoderCallbacks& callbacks_;
};

} // namespace Envoy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
