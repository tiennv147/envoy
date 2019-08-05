#pragma once

#include "extensions/filters/network/common/factory_base.h"
#include "extensions/filters/network/well_known_names.h"

#include "envoy/config/filter/network/memcached_proxy/v1alpha1/memcached_proxy.pb.h"
#include "envoy/config/filter/network/memcached_proxy/v1alpha1/memcached_proxy.pb.validate.h"

#include "envoy/network/filter.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

/**
 * Config registration for the Memcached filter.
 */
class MemcachedConfigFactory : public Common::FactoryBase<envoy::config::filter::network::memcached_proxy::v1alpha1::MemcachedProxy> {
public:
  MemcachedConfigFactory() : FactoryBase(NetworkFilterNames::get().MemcachedProxy) {}

private:
  Network::FilterFactoryCb createFilterFactoryFromProtoTyped(const envoy::config::filter::network::memcached_proxy::v1alpha1::MemcachedProxy& proto_config, Server::Configuration::FactoryContext& context) override;
};

} // namespace MemcachedProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
