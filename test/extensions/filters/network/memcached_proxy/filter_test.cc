#include "common/buffer/buffer_impl.h"

#include <memory>

#include "extensions/filters/network/memcached_proxy/binary_codec_impl.h"
#include "extensions/filters/network/memcached_proxy/filter.h"

#include "test/mocks/network/mocks.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MemcachedProxy {

class MemcachedFilterTest : public testing::Test {
public:
  MemcachedFilterTest() = default;

  void initialize() {
    config_ = std::make_shared<MemcachedFilterConfig>(stat_prefix_, scope_);
    filter_ = std::make_shared<MemcachedFilter>(config_);
    filter_->initializeReadFilterCallbacks(callbacks_);
  }

  void fillJunk(Buffer::OwnedImpl& buffer) {
    buffer.writeBEInt<uint16_t>(0x00);
    buffer.writeBEInt<uint8_t>(0x00);
    buffer.writeBEInt<uint8_t>(0x00);
    buffer.writeBEInt<uint16_t>(0x00);
    buffer.writeBEInt<uint32_t>(0x00);
    buffer.writeBEInt<uint32_t>(0x00);
    buffer.writeBEInt<uint64_t>(0x00);
  }

  Buffer::OwnedImpl encodeGetRequest() {
    Buffer::OwnedImpl buffer;

    buffer.writeBEInt<uint8_t>(0x80);
    buffer.writeBEInt<uint8_t>(0x00);
    fillJunk(buffer);

    return buffer;
  }

  Buffer::OwnedImpl encodeSetRequest() {
    Buffer::OwnedImpl buffer;

    buffer.writeBEInt<uint8_t>(0x80);
    buffer.writeBEInt<uint8_t>(0x01);
    fillJunk(buffer);

    return buffer;
  }

  Buffer::OwnedImpl encodeAddRequest() {
    Buffer::OwnedImpl buffer;

    buffer.writeBEInt<uint8_t>(0x80);
    buffer.writeBEInt<uint8_t>(0x02);
    fillJunk(buffer);

    return buffer;
  }

  Buffer::OwnedImpl encodeReplaceRequest() {
    Buffer::OwnedImpl buffer;

    buffer.writeBEInt<uint8_t>(0x80);
    buffer.writeBEInt<uint8_t>(0x03);
    fillJunk(buffer);
    
    return buffer;
  }

  Buffer::OwnedImpl encodeDeleteRequest() {
    Buffer::OwnedImpl buffer;

    buffer.writeBEInt<uint8_t>(0x80);
    buffer.writeBEInt<uint8_t>(0x04);
    fillJunk(buffer);
    
    return buffer;
  }

  Buffer::OwnedImpl encodeIncrementRequest() {
    Buffer::OwnedImpl buffer;

    buffer.writeBEInt<uint8_t>(0x80);
    buffer.writeBEInt<uint8_t>(0x05);
    fillJunk(buffer);
    
    return buffer;
  }

  Buffer::OwnedImpl encodeDecrementRequest() {
    Buffer::OwnedImpl buffer;

    buffer.writeBEInt<uint8_t>(0x80);
    buffer.writeBEInt<uint8_t>(0x06);
    fillJunk(buffer);
    
    return buffer;
  }

  Stats::IsolatedStoreImpl scope_;
  std::string stat_prefix_{"test.memcached"};
  std::shared_ptr<MemcachedFilterConfig> config_;
  std::shared_ptr<MemcachedFilter> filter_;
  testing::NiceMock<Network::MockReadFilterCallbacks> callbacks_;
};

TEST_F(MemcachedFilterTest, Get) {
  initialize();

  Buffer::OwnedImpl data = encodeGetRequest();

  EXPECT_EQ(Envoy::Network::FilterStatus::Continue, filter_->onData(data, false));
  EXPECT_EQ(1UL, config_->stats_.downstream_rq_total_.value());
  EXPECT_EQ(1UL, config_->stats_.get_rq_.value());
}

TEST_F(MemcachedFilterTest, Set) {
  initialize();

  Buffer::OwnedImpl data = encodeSetRequest();

  EXPECT_EQ(Envoy::Network::FilterStatus::Continue, filter_->onData(data, false));
  EXPECT_EQ(1UL, config_->stats_.downstream_rq_total_.value());
  EXPECT_EQ(1UL, config_->stats_.set_rq_.value());
}

TEST_F(MemcachedFilterTest, Add) {
  initialize();

  Buffer::OwnedImpl data = encodeAddRequest();

  EXPECT_EQ(Envoy::Network::FilterStatus::Continue, filter_->onData(data, false));
  EXPECT_EQ(1UL, config_->stats_.downstream_rq_total_.value());
  EXPECT_EQ(1UL, config_->stats_.add_rq_.value());
}

TEST_F(MemcachedFilterTest, Replace) {
  initialize();

  Buffer::OwnedImpl data = encodeReplaceRequest();

  EXPECT_EQ(Envoy::Network::FilterStatus::Continue, filter_->onData(data, false));
  EXPECT_EQ(1UL, config_->stats_.downstream_rq_total_.value());
  EXPECT_EQ(1UL, config_->stats_.replace_rq_.value());
}

TEST_F(MemcachedFilterTest, Delete) {
  initialize();

  Buffer::OwnedImpl data = encodeDeleteRequest();

  EXPECT_EQ(Envoy::Network::FilterStatus::Continue, filter_->onData(data, false));
  EXPECT_EQ(1UL, config_->stats_.downstream_rq_total_.value());
  EXPECT_EQ(1UL, config_->stats_.delete_rq_.value());
}

TEST_F(MemcachedFilterTest, Increment) {
  initialize();

  Buffer::OwnedImpl data = encodeIncrementRequest();

  EXPECT_EQ(Envoy::Network::FilterStatus::Continue, filter_->onData(data, false));
  EXPECT_EQ(1UL, config_->stats_.downstream_rq_total_.value());
  EXPECT_EQ(1UL, config_->stats_.increment_rq_.value());
}

TEST_F(MemcachedFilterTest, Decrement) {
  initialize();

  Buffer::OwnedImpl data = encodeDecrementRequest();

  EXPECT_EQ(Envoy::Network::FilterStatus::Continue, filter_->onData(data, false));
  EXPECT_EQ(1UL, config_->stats_.downstream_rq_total_.value());
  EXPECT_EQ(1UL, config_->stats_.decrement_rq_.value());
}

} // namespace MemcachedProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
