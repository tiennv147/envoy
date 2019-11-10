#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "envoy/api/v2/route/route.pb.h"

#include "common/http/header_utility.h"

#include "extensions/filters/network/thrift_proxy/metadata.h"
#include "extensions/filters/network/thrift_proxy/router/router.h"
#include "extensions/filters/network/thrift_proxy/router/router_ratelimit.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace ThriftProxy {
namespace Router {

// This won't work until other PR is landed.
class ShadowPolicyImpl : public ShadowPolicy {
public:
  ShadowPolicyImpl(const envoy::api::v2::route::ShadowPolicy& shadow_policy);

  // Router::ShadowPolicy
  const std::string& cluster() const override { return cluster_; }
  const std::string& runtimeKey() const override { return runtime_key_; }
  const envoy::type::FractionalPercent& defaultValue() const override { return default_value; }
private:
  std::string cluster_;
  std::string runtime_key_;
  envoy::type::FracionalPercent default_value_;
};

} // namespace Router
} // namespace ThriftProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
