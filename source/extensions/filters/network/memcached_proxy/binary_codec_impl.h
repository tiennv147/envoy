#pragma once

#include "common/common/logger.h"

#include "extensions/filters/network/memcached_proxy/codec.h"

#include "envoy/buffer/buffer.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

// https://github.com/memcached/memcached/wiki/BinaryProtocolRevamped

// TODO(dereka) maybe move these to source file
enum class MagicByte {
  REQUEST = 0x80,
  RESPONSE = 0x81
};

enum class ResponseStatus {
  NO_ERROR = 0x0000,
  KEY_NOT_FOUND = 0X0001,
  KEY_EXISTS = 0x0002,
  VALUE_TOO_LARGE = 0x0003,
  INVALID_ARGUMENTS = 0X0004,
  ITEM_NOT_STORED = 0X0005,
  CHANGE_ON_NON_NUMERIC_VALUE = 0X0006,
  VBUCKET_BELONGS_TO_ANOTHER_SERVER = 0x0007,
  AUTHENTICATION_ERROR = 0X0008,
  AUTHENTICATION_CONTINUE = 0X0009,
  UNKNOWN_COMMAND = 0X0081,
  OUT_OF_MEMORY = 0X0082,
  NOT_SUPPORTED = 0X0083,
  INTERNAL_ERROR = 0X0084,
  BUSY = 0X0085,
  TEMPORARY_FAILURE = 0x0086
};

enum class CommandOpCodes {
  GET = 0x00,
  SET = 0X01,
  ADD = 0X02,
  REPLACE = 0X03,
  DELETE = 0X04,
  INCREMENT = 0X05,
  DECREMENT = 0X06,
  QUIT = 0X07,
  FLUSH = 0X08,
  GET_Q = 0X09,
  NO_OP = 0X0A,
  VERSION = 0X0B,
  GET_K = 0X0C,
  GET_K_Q = 0X0D,
  APPEND = 0X0E,
  PREPEND = 0X0F,
  STAT = 0X10,
  SET_Q = 0X11,
  ADD_Q = 0X12,
  REPLACE_Q = 0X13,
  DELETE_Q = 0X14,
  INCREMENT_Q = 0X15,
  DECREMENT_Q = 0X16,
  QUIT_Q = 0X17,
  FLUSH_Q = 0X18,
  APPEND_Q = 0X19,
  PREPEND_Q = 0X1A,
  /* The following have not been finalized into the Memcached binary protocol.
  VERBOSITY = 0X1B,
  TOUHC = 0X1C,
  GAT = 0X1D,
  GAT_Q = 0X1E,
  */
  SASL_LIST_MECHS = 0X20,
  SASL_AUTH = 0X21,
  SASL_STEP = 0X22,
  R_GET = 0X30,
  R_SET = 0X31,
  R_SET_Q = 0X32,
  R_APPEND = 0X33,
  R_APPEND_Q = 0X34,
  R_PREPEND = 0X35,
  R_PREPEND_Q = 0X36,
  R_DELETE = 0X37,
  R_DELETE_Q = 0X38,
  R_INCR = 0X39,
  R_INCR_Q = 0X3A,
  R_DECR = 0X3B,
  R_DECR_Q = 0X3C
  /* The following have not been finalized into the Memcached binary protocol.
  SET_VBUCKET = 0X3D,
  GET_VBUCKET = 0X3E,
  DEL_VBUCKET = 0X3F,
  TAP_CONNECT = 0X40,
  TAP_MUTATION = 0X41,
  TAP_DELETE = 0X42,
  TAP_FLUSH = 0X43,
  TAP_OPAQUE = 0X44,
  TAP_VBUCKET_SET = 0X45,
  TAP_CHECKPOINT_START = 0X46,
  TAP_CHECKPOINT_END = 0X47
  */
};

enum class DataTypes {
  RAW_BYTES = 0x00
};

/**
 * Implementation of the abstract Memcached Message interface.
 */
class BinaryMessageImpl : Message {
public:
  BinaryMessageImpl(uint8_t data_type, uint8_t vbucket_id_or_status, uint32_t opaque, uint64_t cas, const std::string& key, const std::string& extras, const std::string& body);
  ~BinaryMessageImpl() = default;

  // MemcachedProxy::Message
  uint8_t dataType() const override { return data_type_; }
  uint8_t vbucketIdOrStatus() const override { return vbucket_id_or_status_; }
  uint32_t opaque() const override { return opaque_; }
  uint64_t cas() const override { return cas_; }
  const std::string& extras() const override { return extras_; }
  const std::string& body() const override { return body_; }
  const std::string& key() const override { return key_; }

private:
  uint8_t data_type_;
  uint8_t vbucket_id_or_status_;
  uint32_t opaque_;
  uint64_t cas_;
  std::string extras_;
  std::string body_;
  std::string key_;
};

class BinaryDecoderImpl : public Decoder, Logger::Loggable<Logger::Id::memcached> {
public:
  BinaryDecoderImpl() = default;
  explicit BinaryDecoderImpl(DecoderCallbacks& callbacks) : callbacks_(callbacks) {}

  // MemcachedProxy::Decoder
  void decode(Buffer::Instance& data) override;

private:

  DecoderCallbacks& callbacks_;
};

} // nanespace MemcachedProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
