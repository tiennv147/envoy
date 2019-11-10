#include "extensions/filters/network/thrift_proxy/router/shadow_policy_impl.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace ThriftProxy {
namespace Router {

ShadowPolicyImpl::ShadowPolicyImpl(const envoy::api::v2::route::ShadowPolicy& shadow_policy) : cluster_(shadow_policy.cluster()), runtime_key_(shadow_policy.runtime_fraction().runtime_key()), default_value_(shadow_policy.runtime_fraction().default_Value()) {}

} // namespace Router
} // namespace ThriftProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
