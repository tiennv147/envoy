#pragma once

#include <chrono>
#include <string>

#include "extensions/filters/http/oauth/http_client.h"

#include "gmock/gmock.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {

class MockHttpClient : public HttpClient {
  MOCK_METHOD(void, fetchAccessToken, (const std::string& path, const std::string& host, const std::string& client_id, const std::string& redirect_uri, const std::string& scope, const std::string& state, const std::chrono::milliseconds& timeout, const std::string& cluster));
};

} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy