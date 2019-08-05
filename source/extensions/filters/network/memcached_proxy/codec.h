#pragma once

#include <memory>

#include "envoy/buffer/buffer.h"
#include "envoy/common/exception.h"
#include "envoy/common/pure.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

enum class Command {
  GET,
  SET
};

class DecoderCallbacks {
public:
  virtual ~DecoderCallbacks() = default;

  virtual void onRequest() PURE;
  virtual void onResponse() PURE;
  virtual void onGet() PURE;
  virtual void onSet() PURE;
  virtual void onAdd() PURE;
  virtual void onReplace() PURE;
  virtual void onDelete() PURE;
  virtual void onIncrement() PURE;
  virtual void onDecrement() PURE;
  virtual void onQuit() PURE;
  virtual void onFlush() PURE;
  virtual void onGetQuiet() PURE;
  virtual void onNoOp() PURE;
  virtual void onVersion() PURE;
  virtual void onGetK() PURE;
  virtual void onGetKQuiet() PURE;
  virtual void onAppend() PURE;
  virtual void onPrepend() PURE;
  virtual void onStat() PURE;
  virtual void onSetQuiet() PURE;
  virtual void onAddQuiet() PURE;
  virtual void onReplaceQuiet() PURE;
  virtual void onDeleteQuiet() PURE;
  virtual void onIncrementQuiet() PURE;
  virtual void onDecrementQuiet() PURE;
  virtual void onQuitQuiet() PURE;
  virtual void onFlushQuiet() PURE;
  virtual void onAppendQuiet() PURE;
  virtual void onPrependQuiet() PURE;
  virtual void onSaslListMechs() PURE;
  virtual void onSaslAuth() PURE;
  virtual void onRGet() PURE;
  virtual void onRSet() PURE;
  virtual void onRSetQuiet() PURE;
  virtual void onRAppend() PURE;
  virtual void onRAppendQuiet() PURE;
  virtual void onRPrepend() PURE;
  virtual void onRPrependQuiet() PURE;
  virtual void onRDelete() PURE;
  virtual void onRDeleteQuiet() PURE;
  virtual void onRIncrement() PURE;
  virtual void onRIncrementQuiet() PURE;
  virtual void onRDecrement() PURE;
  virtual void onRDecrementQuiet() PURE;
};

class Decoder {
public:
  virtual ~Decoder() = default;

  virtual void decode(Buffer::Instance& data) PURE;
};

using DecoderPtr = std::unique_ptr<Decoder>;

class Encoder {
public:
  virtual ~Encoder() = default;

  virtual void encodeMessage(const Message& message, Buffer::Instance& out) PURE;
};

/**
 * A Memcached protocol error.
 */
class ProtocolError : public EnvoyException {
public:
  ProtocolError(const std::string& message) : EnvoyException(message) {}
};

} // namespace MemcachedProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
