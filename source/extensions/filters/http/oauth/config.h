#pragma once

#include "envoy/extensions/filters/http/oauth/v3/oauth.pb.h"
#include "envoy/extensions/filters/http/oauth/v3/oauth.pb.validate.h"

#include "extensions/filters/http/common/factory_base.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {

class Factory : public Common::FactoryBase<envoy::extensions::filters::http::oauth::v3::Filter> {
public:
  Factory();

private:
  Http::FilterFactoryCb createFilterFactoryFromProtoTyped(
      const envoy::extensions::filters::http::oauth::v3::Filter& proto_config, const std::string&,
      Server::Configuration::FactoryContext& context) override;
};

} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
