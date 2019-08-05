#include "extensions/filters/network/memcached_proxy/config.h"

#include <memory>
#include <string>

#include "extensions/filters/network/memcached_proxy/filter.h"

#include "envoy/network/filter.h"
#include "envoy/registry/registry.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

Network::FilterFactoryCb MemcachedConfigFactory::createFilterFactoryFromProtoTyped(const envoy::config::filter::network::memcached_proxy::v1alpha1::MemcachedProxy& proto_config, Server::Configuration::FactoryContext& context) {
  ASSERT(!proto_config.stat_prefix().empty());

  const std::string stat_prefix = fmt::format("{}.memcached.", proto_config.stat_prefix());

  auto filter_config = std::make_shared<MemcachedFilterConfig>(stat_prefix, context.scope());
  return [filter_config](Network::FilterManager& filter_manager) -> void {

    // TODO maybe dynamically create a Binary-only or ASCII-only filter?
    filter_manager.addFilter(std::make_shared<MemcachedFilter>(filter_config)); // TODO this can probably be moved
  };
}

/**
 * Static registration for the Memcached proxy filter. @see RegisterFactory
 */
REGISTER_FACTORY(MemcachedConfigFactory, Server::Configuration::NamedNetworkFilterConfigFactory);

} // namespace MemcachedProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
