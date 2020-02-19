#include <chrono>
#include <memory>
#include <string>
#include <utility>

#include "envoy/http/message.h"
#include "envoy/upstream/cluster_manager.h"

#include "common/http/header_map_impl.h"
#include "common/http/message_impl.h"

#include "extensions/filters/http/oauth/http_client.h"

#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {

HttpClient::HttpClient(Upstream::ClusterManager& cm) : cm_(cm) {}

void HttpClient::fetchAccessToken(const std::string& path, const std::string& host,
                                  const std::string& client_id, const std::string& redirect_uri,
                                  const std::string& scope, const std::string& state,
                                  const std::chrono::milliseconds& timeout,
                                  const std::string& cluster) {
  Http::HeaderMapPtr headers = Http::HeaderMapImpl::create({
      {Http::Headers::get().Method, Http::Headers::get().MethodValues.Post},
      {Http::Headers::get().Path, path},
      {Http::Headers::get().Accept, "application/json"},
      {Http::Headers::get().ContentType, "application/x-www-form-urlencoded"},
      {Http::Headers::get().Host, host},
  });

  const std::string request_body =
      absl::StrCat("response_type=code&client_id=", client_id, "&redirect_uri=", redirect_uri,
                   "&scope=", scope, "&state=", state);
  const std::string urlencoded_request_body = absl::StrReplaceAll(
      request_body, {{"\n", "%0A"}, {" ", "%20"}, {"+", "%2B"}, {"/", "%2F"}, {"=", "%3D"}});

  Buffer::InstancePtr body = std::make_unique<Buffer::OwnedImpl>();
  body->add(urlencoded_request_body);

  Http::MessagePtr message = std::make_unique<Http::RequestMessageImpl>(std::move(headers));
  message->body() = std::move(body);
  in_flight_request_ = cm_.httpAsyncClientForCluster(cluster).send(
      std::move(message), *this, Http::AsyncClient::RequestOptions().setTimeout(timeout));
}

void HttpClient::onSuccess(Http::MessagePtr&&) {
  // TODO
}

void HttpClient::onFailure(Http::AsyncClient::FailureReason) {
  // TODO
}

} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy