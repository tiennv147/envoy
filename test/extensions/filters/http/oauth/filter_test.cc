#include <memory>
#include <string>

#include "extensions/filters/http/oauth/filter.h"

#include "test/mocks/http/mocks.h"
#include "test/mocks/server/mocks.h"
#include "test/mocks/upstream/mocks.h"

#include "test/test_common/utility.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {
namespace {

class FilterTest : public testing::Test {
public:
  FilterTest() {
    const std::string default_config = R"EOF(
    client_id: foo
    client_secret: bar
    signout_path: /signout
    )EOF";

    ON_CALL(context_, clusterManager()).WillByDefault(testing::ReturnRef(cm_));
    EXPECT_CALL(cm_, get(_)).WillRepeatedly(testing::Return(&thread_local_cluster_));

    setConfig(default_config);

    filter_->setDecoderFilterCallbacks(decoder_callbacks_);
  }

  void setConfig(const std::string& config) {
    envoy::extensions::filters::http::oauth::v3::Filter proto_config;
    TestUtility::loadFromYaml(config, proto_config);
    config_ = std::make_shared<FilterConfig>(proto_config, context_);
    filter_ = std::make_unique<Filter>(config_);
  }
  
  std::shared_ptr<FilterConfig> config_;
  std::unique_ptr<Filter> filter_;
  Http::MockStreamDecoderFilterCallbacks decoder_callbacks_;
  NiceMock<Server::Configuration::MockFactoryContext> context_;
  NiceMock<Upstream::MockClusterManager> cm_;
  Upstream::MockThreadLocalCluster thread_local_cluster_;
};

TEST_F(FilterTest, SignOutPath) {
  Http::TestHeaderMapImpl request_headers{
    {":method", "GET"},
    {":path", "/signout"},
    {":authority", "envoyproxy.io"},
  };

  Http::TestHeaderMapImpl response_headers{
      {":status", "302"},
      {"location", "/"},
      {"set-cookie", "OauthAccessToken=deleted; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT"},
  };

  EXPECT_CALL(decoder_callbacks_, encodeHeaders_(HeaderMapEqualRef(&response_headers), true));
  EXPECT_EQ(Http::FilterHeadersStatus::StopIteration, filter_->decodeHeaders(request_headers, true));
}

TEST_F(FilterTest, SignOutPathWithQueryParameters) {
  Http::TestHeaderMapImpl request_headers{
    {":method", "GET"},
    {":path", "/signout?foo=bar&biz=baz"},
    {":authority", "envoyproxy.io"},
  };

  Http::TestHeaderMapImpl response_headers{
      {":status", "302"},
      {"location", "/"},
      {"set-cookie", "OauthAccessToken=deleted; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT"},
  };

  EXPECT_CALL(decoder_callbacks_, encodeHeaders_(HeaderMapEqualRef(&response_headers), true));
  EXPECT_EQ(Http::FilterHeadersStatus::StopIteration, filter_->decodeHeaders(request_headers, true));
}

} // namespace
} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy