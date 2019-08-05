#include "extensions/filters/network/memcached_proxy/filter.h"

#include <memory>
#include <string>

#include "extensions/filters/network/memcached_proxy/ascii_codec_impl.h"
#include "extensions/filters/network/memcached_proxy/binary_codec_impl.h"

#include "envoy/buffer/buffer.h"
#include "envoy/network/connection.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

MemcachedFilterConfig::MemcachedFilterConfig(const std::string& stat_prefix, Stats::Scope& scope) : scope_(scope), stats_(generateStats(stat_prefix, scope)), stat_prefix_(stat_prefix) {}

MemcachedFilter::MemcachedFilter(std::shared_ptr<MemcachedFilterConfig> config) : config_(std::move(config)) {}

void MemcachedFilter::initializeReadFilterCallbacks(Network::ReadFilterCallbacks& callbacks) {
  callbacks_ = &callbacks;
}

Network::FilterStatus MemcachedFilter::onData(Buffer::Instance& data, bool) {
  try {
    // TODO - peek first byte to determine whether binary or ASCII.
    // Thought: maybe just have a single decoder that can handle ASCII or Binary....
    if (!binary_decoder_) {
      binary_decoder_ = createBinaryDecoder(*this);
    }

    binary_decoder_->decode(data);
    return Network::FilterStatus::Continue;
  } catch (ProtocolError&) {
    // TODO(dereka) create error message
    // encode error message
    // write error message to socket
    // (see Redis filter)
    callbacks_->connection().close(Network::ConnectionCloseType::NoFlush);
    return Network::FilterStatus::StopIteration;
  }
}

void MemcachedFilter::onRequest() {
  config_->stats_.downstream_rq_total_.inc();
}

void MemcachedFilter::onResponse() {

}

void MemcachedFilter::onGet() {
  config_->stats_.get_rq_.inc();
}

void MemcachedFilter::onSet() {
  config_->stats_.set_rq_.inc();
}

void MemcachedFilter::onAdd() {
  config_->stats_.add_rq_.inc();
}

void MemcachedFilter::onReplace() {
  config_->stats_.replace_rq_.inc();
}

void MemcachedFilter::onDelete() {
  config_->stats_.delete_rq_.inc();
}

void MemcachedFilter::onIncrement() {
  config_->stats_.increment_rq_.inc();
}

void MemcachedFilter::onDecrement() {
  config_->stats_.decrement_rq_.inc();
}

Network::FilterStatus MemcachedFilter::onWrite(Buffer::Instance&, bool) {
  return Network::FilterStatus::Continue;
}

Network::FilterStatus MemcachedFilter::onNewConnection() { return Network::FilterStatus::Continue; }

DecoderPtr MemcachedFilter::createBinaryDecoder(DecoderCallbacks& callbacks) {
  return std::make_unique<BinaryDecoderImpl>(callbacks);
}

DecoderPtr MemcachedFilter::createAsciiDecoder(DecoderCallbacks&) {
  // TODO bring callbacks back
  return std::make_unique<AsciiDecoderImpl>();
}

} // namespace MemcachedProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
