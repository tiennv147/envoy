#include "extensions/filters/network/memcached_proxy/binary_codec_impl.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

class TestDecoderCallbacks : public DecoderCallbacks {
public:
  void decodeMessage(MessagePtr&& message) override { decodeMessage_(message); }

  MOCK_METHOD1(decodeMessage_, void(MessagePtr& message));
}

class BinaryCodecImplTest : public testing::Test {
public:
  Buffer::OwendImpl output_;
  testing::NiceMock<TestDecoderCallbacks> callbacks_;
  BinaryDecoderImpl decoder_{callbacks_};
};

} // namespace MemcachedProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy