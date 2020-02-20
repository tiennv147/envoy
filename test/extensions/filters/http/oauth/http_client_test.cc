#include <memory>
#include <string>

#include "envoy/http/message.h"

#include "common/http/header_map_impl.h"
#include "common/http/message_impl.h"

#include "extensions/filters/http/oauth/http_client.h"

#include "test/mocks/upstream/mocks.h"
#include "test/test_common/utility.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {
namespace {

MATCHER_P(BodyMatcher, expected_body, "") {
  return arg->bodyAsString() == expected_body;
}

class HttpClientTest : public testing::Test {
public:
  HttpClientTest() { client_ = std::make_unique<HttpClient>(cm_); }

  static Http::RequestMessagePtr buildRequest(Http::RequestHeaderMapPtr&& headers,
                                              const std::string& body) {
    Buffer::InstancePtr body_buffer = std::make_unique<Buffer::OwnedImpl>();
    body_buffer->add(body);

    Http::RequestMessagePtr message =
        std::make_unique<Http::RequestMessageImpl>(std::move(headers));
    message->body() = std::move(body_buffer);
    return message;
  }

  std::unique_ptr<HttpClient> client_;
  NiceMock<Upstream::MockClusterManager> cm_;
};

TEST_F(HttpClientTest, BasicRequest) {
  const std::string host = "envoyproxy.io";
  const std::string path = "/token";
  Http::RequestHeaderMapPtr expected_request_headers =
      Http::makeHeaderMap<Http::TestRequestHeaderMapImpl>({
          {":method", "POST"},
          {":path", path},
          {"accept", "application/json"},
          {"content-type", "application/x-www-form-urlencoded"},
          {":host", host},
      });
  const std::string expected_body =
      "response_type=code&client_id=client_id&redirect_uri=%2Fcallback&scope=email&state=nonce";
  const auto request = buildRequest(std::move(expected_request_headers), expected_body);
  EXPECT_CALL(cm_, httpAsyncClientForCluster("auth_cluster"))
      .WillOnce(ReturnRef(cm_.async_client_));
  // TODO(derekargueta) fix HeaderMatching
  EXPECT_CALL(cm_.async_client_, send_(BodyMatcher(expected_body), _, _))
      .WillOnce(
          Invoke([&](Http::RequestMessagePtr&, Http::AsyncClient::Callbacks& callbacks,
                     const Http::AsyncClient::RequestOptions&) -> Http::AsyncClient::Request* {
            callbacks.onSuccess(
                Http::ResponseMessagePtr{new Http::ResponseMessageImpl(Http::ResponseHeaderMapPtr{
                    new Http::TestResponseHeaderMapImpl{{":status", "200"}}})});
            return nullptr;
          }));
  client_->fetchAccessToken(path, host, "client_id", "/callback", "email", "nonce",
                            std::chrono::milliseconds(200), "auth_cluster");
}

} // namespace
} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
