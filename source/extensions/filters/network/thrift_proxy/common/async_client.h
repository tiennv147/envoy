#pragma once

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace ThriftProxy {
namespace Common {

class AsyncRequest {
public:
  virtual ~AsyncRequest() = default;

  /**
   * Signals that the request should be cancelled. No further callbacks will be invoked.
   */
  virtual void cancel() PURE;
};

/**
 * Notifies the caller of async Thrift status.
 */
class AsyncCallbacks {
  virtual ~AsyncCallbacks() = default;
};

class AsyncClient {
public:
  virtual ~AyncClient() = default;
};

} // namespace Common
} // namespace ThriftProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
