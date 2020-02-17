#pragma once

#include <memory>

#include "extensions/filters/http/common/pass_through_filter.h"

#include "envoy/extensions/filters/http/oauth/v3/oauth.pb.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {

class FilterConfig {
public:
  FilterConfig(const envoy::extensions::filters::http::oauth::v3::Filter& proto_config);
private:
  const std::string client_id_;
  const std::string client_secret_;
};

class Filter : public Http::PassThroughDecoderFilter {
public:
  Filter(const std::shared_ptr<FilterConfig> filter_config);

  // Http::StreamDecoderFilter
  Http::FilterHeadersStatus decodeHeaders(Http::HeaderMap&, bool) override;
private:
  std::shared_ptr<FilterConfig> config_;  // can this just be a const ref?
};

} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy