#pragma once

#include <chrono>
#include <string>

#include "envoy/http/async_client.h"
#include "envoy/http/message.h"
#include "envoy/upstream/cluster_manager.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {

/**
 * Responsible for sending
 */
class HttpClient : public Http::AsyncClient::Callbacks {
public:
  HttpClient(Upstream::ClusterManager& cm);

  /**
   * Sends an HTTP POST to the identify provider to exchange
   * the client ID, secret, code, redirect URI, and state for
   * the user's access token.
   */
  void fetchAccessToken(const std::string& path, const std::string& host,
                        const std::string& client_id, const std::string& redirect_uri,
                        const std::string& scope, const std::string& state,
                        const std::chrono::milliseconds& timeout, const std::string& cluster);

  // Http::AsyncClient::Callbacks
  void onSuccess(Http::MessagePtr&& message) override;
  void onFailure(Http::AsyncClient::FailureReason reason) override;

private:
  Http::AsyncClient::Request* in_flight_request_;
  Upstream::ClusterManager& cm_;
};

} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy