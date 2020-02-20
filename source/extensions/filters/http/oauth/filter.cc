#include <memory>
#include <string>
#include <utility>

#include "envoy/extensions/filters/http/oauth/v3/oauth.pb.h"
#include "envoy/server/filter_config.h"

#include "common/common/enum_to_int.h"
#include "common/http/header_map_impl.h"
#include "common/http/headers.h"
#include "common/http/utility.h"

#include "extensions/filters/http/oauth/filter.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {

FilterConfig::FilterConfig(const envoy::extensions::filters::http::oauth::v3::Filter& proto_config,
                           Server::Configuration::FactoryContext& context)
    : client_id_(proto_config.client_id()), client_secret_(proto_config.client_secret()),
      signout_path_(proto_config.signout_path()) {
  auto& cluster_manager = context.clusterManager();

  // TODO(derekargueta) should this be a hard constraint? The dynamic nature of CDS
  // may imply that instead of forcing this, we just increment a metric instead?
  if (cluster_manager.get(idp_cluster_name_) == nullptr) {
    throw EnvoyException("Cluster does not exist");
  }
}

Filter::Filter(const std::shared_ptr<FilterConfig> filter_config) : config_(filter_config) {}

Http::FilterHeadersStatus Filter::decodeHeaders(Http::RequestHeaderMap& headers, bool) {
  const auto path = headers.Path()->value().getStringView();
  Http::Utility::QueryParams query_parameters = Http::Utility::parseQueryString(path);
  const auto no_query_param_path = path.substr(0, path.find('?'));
  if (no_query_param_path == config_->signoutPath()) {
    // Set cookie deletion
    enqueueSignoutRedirect();
    return Http::FilterHeadersStatus::StopIteration;
  } else if (no_query_param_path == config_->callbackPath()) {
    const auto* cookies = headers.get(Http::Headers::get().Cookie);
    if (cookies == nullptr) {
      // TODO error out b/c we can't compare states and validate no CSRF violation.
    }
    const std::string cookie_state = Http::Utility::parseCookieValue(headers, "OAuthState");
    // Exchange code for access token.
    const auto code_iter = query_parameters.find("code");
    const auto state_iter = query_parameters.find("state");
    if (code_iter == query_parameters.end() || state_iter == query_parameters.end()) {
      // Return 403 unauthorized
      return Http::FilterHeadersStatus::StopIteration;
    }

    if (state_iter->second != cookie_state) {
      // Return 403 unauthorized - possible CSRF attack.
      return Http::FilterHeadersStatus::StopIteration;
    }
    // send fetchAccessToken()

    // Return an HTTP 302 to the originally requested resource?
    // And set a set-cookie to clear the state cookie?
  } else {
    // validate access token?
  }

  return Http::FilterHeadersStatus::Continue;
}

void Filter::enqueueSignoutRedirect() {
  Http::ResponseHeaderMapPtr response_headers = Http::createHeaderMap<Http::ResponseHeaderMapImpl>({
      {Http::Headers::get().Status, std::to_string(enumToInt(Http::Code::Found))},
      // The location header can be relative or absolute. We choose relative here to avoid
      // needing to get the host.
      {Http::Headers::get().Location, "/"},
      {Http::Headers::get().SetCookie,
       "OauthAccessToken=deleted; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT"},
  });

  decoder_callbacks_->encodeHeaders(std::move(response_headers), true);
}

void Filter::enqueueForbiddenResponse() {
  Http::ResponseHeaderMapPtr response_headers = Http::createHeaderMap<Http::ResponseHeaderMapImpl>({
      {Http::Headers::get().Status, std::to_string(enumToInt(Http::Code::Forbidden))},
  });

  decoder_callbacks_->encodeHeaders(std::move(response_headers), true);
}

} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
