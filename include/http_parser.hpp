#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include "common.h"
#include "libs/http-parser/http_parser.h"

class HttpParser {
public:
    template<class Buffer>
    std::size_t consume(const Buffer& buffer) {
        std::size_t consumed =
            ::http_parser_execute(&parser_, &settings_,
                                  buffer.data(), buffer.size());

        if (consumed != buffer.size()) {
            if (HTTP_PARSER_ERRNO(&parser_) != HPE_OK) {
                XERROR << "Error occurred during message parsing, error code: "
                       << parser_.http_errno << ", message: "
                       << ::http_errno_description(static_cast<http_errno>(parser_.http_errno));
                return 0;
            } else {
                // TODO will this happen? a message is parsed, but there is still data?
                XWARN << "Weird: parser does not consume all data, but there is no error.";
                return consumed;
            }
        }

        return consumed;
    }

public:
    HttpParser(http_parser_type type)
        : header_completed_(false),
          message_completed_(false),
          chunked_(false) {
        ::http_parser_init(&parser_, type);
        parser_.data = this;

        InitMessage();
    }

    virtual ~HttpParser() = default;

    static int OnMessageBegin(http_parser *parser);
    static int OnUrl(http_parser *parser, const char *at, std::size_t length);
    static int OnStatus(http_parser *parser, const char *at, std::size_t length);
    static int OnHeaderField(http_parser *parser, const char *at, std::size_t length);
    static int OnHeaderValue(http_parser *parser, const char *at, std::size_t length);
    static int OnHeadersComplete(http_parser *parser);
    static int OnBody(http_parser *parser, const char *at, std::size_t length);
    static int OnMessageComplete(http_parser *parser);

private:
    void InitMessage(http_parser_type type) {
        switch (type) {
        case HTTP_REQUEST:
            message_.reset(new HttpRequest);
            break;
        case HTTP_RESPONSE:
            message_.reset(new HttpResponse);
            break;
        case HTTP_BOTH:
        default:
            ; // TODO ignore here?
        }
    }

private:
    http_parser parser_;

    bool header_completed_;
    bool message_completed_;
    bool chunked_;
    std::string current_header_field_;
    std::string current_header_value_;

    std::unique_ptr<HttpMessage> message_;

private:
    static http_parser_settings settings_;

    DISABLE_COPY_AND_ASSIGNMENT(HttpParser);
};

#endif // HTTP_PARSER_HPP