#include <memory>
#include <string>

#include "envoy/extensions/filters/http/oauth/v3/oauth.pb.h"
#include "envoy/registry/registry.h"

#include "extensions/filters/http/oauth/config.h"
#include "extensions/filters/http/oauth/filter.h"
#include "extensions/filters/http/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {

Factory::Factory() : FactoryBase(HttpFilterNames::get().OAuth) {}

Http::FilterFactoryCb Factory::createFilterFactoryFromProtoTyped(
    const envoy::extensions::filters::http::oauth::v3::Filter& proto_config, const std::string&,
    Server::Configuration::FactoryContext& context) {
  const auto filter_config = std::make_shared<FilterConfig>(proto_config, context);
  return [filter_config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    callbacks.addStreamDecoderFilter(std::make_shared<Filter>(filter_config));
  };
}

/**
 * Static registration for the Lua filter. @see RegisterFactory.
 */
REGISTER_FACTORY(Factory, Server::Configuration::NamedHttpFilterConfigFactory);

} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy