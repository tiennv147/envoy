#pragma once

#include <memory>

#include "envoy/common/pure.h"

namespace Envoy {
namespace Http {
namespace Http1 {

enum class ParserType { Legacy, LlHttp };

enum class MessageType { Request, Response };

/**
 * Callbacks base class. Any users of the HTTP1 parser who want to use the parsed data needs to
 * implement this interface and pass the object to the `data` parameter of ParserFactory::create.
 */
class ParserCallbacks {
public:
  virtual ~ParserCallbacks() = default;
  /**
   * Called when a request/response is beginning.
   */
  virtual int onMessageBegin() PURE;

  /**
   * Called when URL data is received.
   * @param data supplies the start address.
   * @param length supplies the length.
   */
  virtual int onUrl(const char* data, size_t length) PURE;

  /**
   * Called when an HTTP status is parsed from a response.
   */
  virtual int onStatus() PURE;

  /**
   * Called when header field data is received.
   * @param data supplies the start address.
   * @param length supplies the length.
   */
  virtual int onHeaderField(const char* data, size_t length) PURE;

  /**
   * Called when header value data is received.
   * @param data supplies the start address.
   * @param length supplies the length.
   */
  virtual int onHeaderValue(const char* data, size_t length) PURE;

  /**
   * Called when headers are complete.
   * @return 0 if no error, 1 if there should be no body.
   */
  virtual int onHeadersComplete() PURE;

  /**
   * Called when body data is received.
   * @param data supplies the start address.
   * @param length supplies the length.
   */
  virtual int onBody(const char* data, size_t length) PURE;

  /**
   * Called when the request/response is complete.
   */
  virtual int onMessageComplete() PURE;
  virtual int onChunkHeader() PURE;   // shrug
  virtual int onChunkComplete() PURE; // shrug
};

/**
 * Parser interface. Wrappers around HTTP/1.x parsing libraries should implement this interface.
 */
class Parser {
public:
  virtual ~Parser() = default;

  /**
   * Excutes the parser on `len` characters of `slice` data.
   */
  virtual int execute(const char* slice, int len) PURE;

  /**
   * Continues the parser if paused, otherwise does nothing.
   */
  virtual void resume() PURE;

  /**
   * Pauses the parser if running, otherwise does nothing.
   */
  virtual int pause() PURE;

  /**
   * Returns an error code if an error was encountered while parsing HTTP.
   */
  virtual int getErrno() PURE;
  virtual int statusCode() const PURE;
  virtual int httpMajor() const PURE;
  virtual int httpMinor() const PURE;
  virtual uint64_t contentLength() const PURE;
  virtual int flags() const PURE;
  virtual uint16_t method() const PURE;
  virtual const char* methodName() const PURE;
  virtual const char* errnoName() PURE;
  virtual bool usesOldImpl() const PURE;
};

enum class Flags {
  Chunked = 1,
};

enum class ParserStatus {
  Ok = 0,
  Paused = 31,
};

enum class Method {
  Head = 2,
  Connect = 5,
  Options = 6,
};

using ParserPtr = std::unique_ptr<Parser>;

} // namespace Http1
} // namespace Http
} // namespace Envoy
