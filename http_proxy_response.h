#ifndef HTTP_PROXY_RESPONSE_H
#define HTTP_PROXY_RESPONSE_H

#include <string>
#include <vector>
#include <boost/array.hpp>


class HttpProxyResponse {
public:
    HttpProxyResponse();
    ~HttpProxyResponse();

    std::string& status_line() { return status_line_; }
    boost::array<char, 4096>& body() { return body_; }

    void AddHeader(const std::string& name, const std::string& value) {
        Header h;
        h.name = name;
        h.value = value;
        headers_.push_back(h);
    }

    void SetBodyLength(std::size_t length) {
        body_length_ = length;
    }

    std::size_t GetBodyLength() { return body_length_; }

    std::string& headers() {
        header_string_ = "";
        for(std::vector<Header>::iterator it = headers_.begin(); it != headers_.end(); ++it) {
            header_string_ += it->name;
            header_string_ += ": ";
            header_string_ += it->value;
            header_string_ += "\r\n";
        }
        header_string_ += "\r\n";
        return header_string_;
    }

private:
    struct Header {
        std::string name;
        std::string value;
    };

    std::string status_line_;
    std::vector<Header> headers_;
    std::string header_string_;
    boost::array<char, 4096> body_;
    std::size_t body_length_;
};

#endif // HTTP_PROXY_RESPONSE_H