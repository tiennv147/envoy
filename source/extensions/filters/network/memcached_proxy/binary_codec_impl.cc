#include "extensions/filters/network/memcached_proxy/binary_codec_impl.h"

#include <iostream>
#include <memory>
#include <utility>

#include "common/common/logger.h"

#include "envoy/buffer/buffer.h"

#include "extensions/filters/network/memcached_proxy/utility.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

static constexpr uint8_t HEADER_LENGTH = 24;

BinaryMessageImpl::BinaryMessageImpl(uint8_t data_type, uint8_t vbucket_id_or_status, uint32_t opaque, uint64_t cas, const std::string& key, const std::string& extras, const std::string& body) : data_type_(data_type), vbucket_id_or_status_(vbucket_id_or_status), opaque_(opaque), cas_(cas), extras_(extras), body_(body), key_(key) {}

void BinaryDecoderImpl::decode(Buffer::Instance& data) {
  ENVOY_LOG(trace, "decoding {} bytes", data.length());
  if (data.length() < HEADER_LENGTH) {
    // TODO(dereka) implement me; see what Redis does for a protocol error
  }

  // Determine whether request or response, peek 1 byte. They are actually *very* similar in
  // structure except request has vbucket_id and response has status...
  const auto magic_byte = data.drainBEInt<uint8_t>();
  switch (static_cast<MagicByte>(magic_byte)) {
  case MagicByte::REQUEST:
    callbacks_.onRequest();
    break;
  case MagicByte::RESPONSE:
    callbacks_.onResponse();
    break;
  default:
    throw ProtocolError(fmt::format("unknown magic byte value: {}", magic_byte));
  }

  const auto opcode = data.drainBEInt<uint8_t>();
  switch (static_cast<CommandOpCodes>(opcode)) {
  case CommandOpCodes::GET:
    callbacks_.onGet();
    break;
  case CommandOpCodes::SET:
    callbacks_.onSet();
    break;
  case CommandOpCodes::ADD:
    callbacks_.onAdd();
    break;
  case CommandOpCodes::REPLACE:
    callbacks_.onReplace();
    break;
  case CommandOpCodes::DELETE:
    callbacks_.onDelete();
    break;
  case CommandOpCodes::INCREMENT:
    callbacks_.onIncrement();
    break;
  case CommandOpCodes::DECREMENT:
    callbacks_.onDecrement();
    break;
  case CommandOpCodes::QUIT:
    break;
  case CommandOpCodes::FLUSH:
    break;
  case CommandOpCodes::GET_Q:
    break;
  case CommandOpCodes::NO_OP:
    break;
  case CommandOpCodes::VERSION:
    break;
  case CommandOpCodes::GET_K:
    break;
  case CommandOpCodes::GET_K_Q:
    break;
  case CommandOpCodes::APPEND:
    break;
  case CommandOpCodes::PREPEND:
    break;
  case CommandOpCodes::STAT:
    break;
  case CommandOpCodes::SET_Q:
    break;
  case CommandOpCodes::ADD_Q:
    break;
  case CommandOpCodes::REPLACE_Q:
    break;
  case CommandOpCodes::DELETE_Q:
    break;
  case CommandOpCodes::INCREMENT_Q:
    break;
  case CommandOpCodes::DECREMENT_Q:
    break;
  case CommandOpCodes::QUIT_Q:
    break;
  case CommandOpCodes::FLUSH_Q:
    break;
  case CommandOpCodes::APPEND_Q:
    break;
  case CommandOpCodes::PREPEND_Q:
    break;
  case CommandOpCodes::SASL_LIST_MECHS:
    break;
  case CommandOpCodes::SASL_AUTH:
    break;
  case CommandOpCodes::SASL_STEP:
    break;
  case CommandOpCodes::R_GET:
    break;
  case CommandOpCodes::R_SET:
    break;
  case CommandOpCodes::R_SET:
    break;
  case CommandOpCodes::R_SET_Q:
    break;
  case CommandOpCodes::R_APPEND:
    break;
  case CommandOpCodes::R_APPEND_Q:
    break;
  case CommandOpCodes::R_PREPEND:
    break;
  case CommandOpCodes::R_PREPEND_Q:
    break;
  case CommandOpCodes::R_DELETE:
    break;
  case CommandOpCodes::R_DELETE_Q:
    break;
  case CommandOpCodes::R_INCR:
    break;
  case CommandOpCodes::R_INCR_Q:
    break;
  case CommandOpCodes::R_DECR:
    break;
  case CommandOpCodes::R_DECR_Q:
    break;
  default:
    throw ProtocolError(fmt::format("unknown opcode value: {}", opcode));
    break;
  }

  const auto key_length = data.drainBEInt<uint16_t>();
  const auto extras_length = data.drainBEInt<uint8_t>();
  const auto data_type = data.drainBEInt<uint8_t>();
  const auto vbucket_id_or_status = data.drainBEInt<uint16_t>();
  auto body_length = data.drainBEInt<uint32_t>();
  const auto opaque = data.drainBEInt<uint32_t>();
  const auto cas = data.drainBEInt<uint64_t>();

  body_length -= (key_length + extras_length);

  const std::string extras = BufferHelper::drainString(data, extras_length);
  const std::string key = BufferHelper::drainString(data, key_length);
  const std::string body = BufferHelper::drainString(data, body_length);

  auto message = std::make_unique<BinaryMessageImpl>(data_type, vbucket_id_or_status, opaque, cas, key, extras, body);

  std::cout << magic_byte << std::endl;

  ENVOY_LOG(info, "decoded {} {} key={}, cas={}, body={}",
    //
    //
    message->key().empty() ? "N/A" : message->key(),
    message->cas(),
    message->body().empty() ? "N/A" : message->body());

  // callbacks_.decodeMessage(std::move(message));
  ENVOY_LOG(trace, "{} bytes remaining after decoding", data.length());
}

} // namespace MemcachedProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
