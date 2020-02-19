#include "envoy/extensions/filters/http/oauth/v3/oauth.pb.h"
#include "envoy/extensions/filters/http/oauth/v3/oauth.pb.validate.h"

#include "extensions/filters/http/oauth/config.h"

#include "test/mocks/server/mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace OAuth {
namespace {

TEST(OAuthFilterConfigTest, ValidateFail) {
  NiceMock<Server::Configuration::MockFactoryContext> context;
  EXPECT_THROW(Factory().createFilterFactoryFromProto(
                   envoy::extensions::filters::http::oauth::v3::Filter(), "stats", context),
               ProtoValidationException);
}

} // namespace
} // namespace OAuth
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy