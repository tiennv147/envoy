#include "common/http/http1/parser_factory.h"

#include <memory>

#include "common/http/http1/legacy_http_parser.h"

namespace Envoy {
namespace Http {
namespace Http1 {

bool ParserFactory::use_legacy_parser_ = true;

// TODO(dereka) add support for llhttp once it's added.
ParserPtr ParserFactory::create(MessageType type, void* data) {
  return std::make_unique<LegacyHttpParserImpl>(type, data);
}

bool ParserFactory::usesLegacyParser() { return use_legacy_parser_; }

void ParserFactory::useLegacy(bool use_legacy_parser) { use_legacy_parser_ = use_legacy_parser; }

} // namespace Http1
} // namespace Http
} // namespace Envoy
