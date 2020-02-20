#pragma once

#include <memory>

#include "envoy/extensions/filters/http/oauth/v3/oauth.pb.h"
#include "envoy/server/filter_config.h"

#include "extensions/filters/http/common/pass_through_filter.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {

class FilterConfig {
public:
  FilterConfig(const envoy::extensions::filters::http::oauth::v3::Filter& proto_config,
               Server::Configuration::FactoryContext& context);

  const std::string& clientId() const { return client_id_; }
  const std::string& clientSecret() const { return client_secret_; }
  const std::string& callbackPath() const { return callback_path_; }
  const std::string& signoutPath() const { return signout_path_; }

private:
  const std::string client_id_;
  const std::string client_secret_;
  const std::string idp_cluster_name_;
  const std::string callback_path_;
  const std::string signout_path_;
};

class Filter : public Http::PassThroughDecoderFilter {
public:
  explicit Filter(const std::shared_ptr<FilterConfig> filter_config);

  // Http::StreamDecoderFilter
  Http::FilterHeadersStatus decodeHeaders(Http::RequestHeaderMap&, bool) override;

private:
  std::shared_ptr<FilterConfig> config_; // can this just be a const ref?

  /**
   * Creates a redirect response to "/" and sets it in the decoder_callbacks_.
   */
  void enqueueSignoutRedirect();

  void enqueueForbiddenResponse();
};

} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
