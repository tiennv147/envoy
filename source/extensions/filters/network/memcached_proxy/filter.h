#pragma once

#include <memory>
#include <string>

#include "extensions/filters/network/memcached_proxy/codec.h"

#include "envoy/buffer/buffer.h"
#include "envoy/network/filter.h"
#include "envoy/stats/scope.h"
#include "envoy/stats/stats_macros.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

/**
 * All Memcached proxy stats. @see stats_macros.h
 */
// clang-format off
#define ALL_MEMCACHED_PROXY_STATS(COUNTER, GAUGE)                      \
  COUNTER(downstream_rq_total)                                        \
  GAUGE(downstream_rq_active, Accumulate)                             \
  COUNTER(get_rq)                                                     \
  COUNTER(set_rq)                                                     \
  COUNTER(add_rq)                                                     \
  COUNTER(replace_rq)                                                \
  COUNTER(delete_rq)                                                  \
  COUNTER(increment_rq)                                               \
  COUNTER(decrement_rq)
// clang-format on

/**
 * Struct definition for all Memcached proxy stats. @see stats_macros.h
 */
struct MemcachedProxyStats {
  ALL_MEMCACHED_PROXY_STATS(GENERATE_COUNTER_STRUCT, GENERATE_GAUGE_STRUCT);
};

/**
 * Configuration for the Memcached proxy filter.
 */
class MemcachedFilterConfig {
public:
  MemcachedFilterConfig(const std::string& stat_prefix, Stats::Scope& scope);

  Stats::Scope& scope_;
  MemcachedProxyStats stats_;

private:
  MemcachedProxyStats generateStats(const std::string& prefix, Stats::Scope& scope) {
    return MemcachedProxyStats{ALL_MEMCACHED_PROXY_STATS(POOL_COUNTER_PREFIX(scope, prefix), POOL_GAUGE_PREFIX(scope, prefix))};
  }

  const std::string stat_prefix_;
};

/**
 * Implementation of Memcached proxy filter.
 */
class MemcachedFilter : public Network::Filter,
                        DecoderCallbacks,
                        Logger::Loggable<Logger::Id::memcached> {
public:
  explicit MemcachedFilter(std::shared_ptr<MemcachedFilterConfig> config);

  // Network::ReadFilter
  Network::FilterStatus onData(Buffer::Instance& data, bool end_stream) override;
  Network::FilterStatus onNewConnection() override;
  void initializeReadFilterCallbacks(Network::ReadFilterCallbacks& callbacks) override;

  // Network::WriteFilter
  Network::FilterStatus onWrite(Buffer::Instance& data, bool end_stream) override;

  // MemcachedProxy::DecoderCallback
  void onRequest() override;
  void onResponse() override;
  void onGet() override;
  void onSet() override;
  void onAdd() override;
  void onReplace() override;
  void onDelete() override;
  void onIncrement() override;
  void onDecrement() override;
  void onQuit() override;
  void onFlush() override;
  void onGetQuiet() override;
  void onNoOp() override;
  void onVersion() override;
  void onGetK() override;
  void onGetKQuiet() override;
  void onAppend() override;
  void onPrepend() override;
  void onStat() override;
  void onSetQuiet() override;
  void onAddQuiet() override;
  void onReplaceQuiet() override;
  void onDeleteQuiet() override;
  void onIncrementQuiet() override;
  void onDecrementQuiet() override;
  void onQuitQuiet() override;
  void onFlushQuiet() override;
  void onAppendQuiet() override;
  void onPrependQuiet() override;
  void onSaslListMechs() override;
  void onSaslAuth() override;
  void onRGet() override;
  void onRSet() override;
  void onRSetQuiet() override;
  void onRAppend() override;
  void onRAppendQuiet() override;
  void onRPrepend() override;
  void onRPrependQuiet() override;
  void onRDelete() override;
  void onRDeleteQuiet() override;
  void onRIncrement() override;
  void onRIncrementQuiet() override;
  void onRDecrement() override;
  void onRDecrementQuiet() override;

  DecoderPtr createBinaryDecoder(DecoderCallbacks& callbacks);
  DecoderPtr createAsciiDecoder(DecoderCallbacks& callbacks);

private:
  Network::ReadFilterCallbacks* callbacks_{};
  std::shared_ptr<MemcachedFilterConfig> config_;
  std::unique_ptr<Decoder> ascii_decoder_;
  std::unique_ptr<Decoder> binary_decoder_;
};

} // namespace MemcachedProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
