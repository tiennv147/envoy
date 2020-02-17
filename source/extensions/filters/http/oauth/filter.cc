#include "extensions/filters/http/oauth/filter.h"

#include <memory>

#include "envoy/extensions/filters/http/oauth/v3/oauth.pb.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {

FilterConfig::FilterConfig(const envoy::extensions::filters::http::oauth::v3::Filter& proto_config) : client_id_(proto_config.client_id()), client_secret_(proto_config.client_secret()) {}

Filter::Filter(const std::shared_ptr<FilterConfig> filter_config) : config_(filter_config) {}

Http::FilterHeadersStatus Filter::decodeHeaders(Http::HeaderMap&, bool) {
  return Http::FilterHeadersStatus::Continue;
}

} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy