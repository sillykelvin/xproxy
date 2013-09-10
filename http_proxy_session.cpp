#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "http_proxy_session.h"
#include "http_proxy_session_manager.h"

HttpProxySession::HttpProxySession(boost::asio::io_service& local_service,
                                   HttpProxySessionManager& manager)
    : remote_service_(), local_socket_(local_service),
      remote_socket_(remote_service_), manager_(manager) {
}

HttpProxySession::~HttpProxySession() {
    // do nothing here
}

boost::asio::ip::tcp::socket& HttpProxySession::LocalSocket() {
    return local_socket_;
}

boost::asio::ip::tcp::socket& HttpProxySession::RemoteSocket() {
    return remote_socket_;
}

void HttpProxySession::Start() {
    local_socket_.async_read_some(
                boost::asio::buffer(buffer_),
                boost::bind(&HttpProxySession::HandleRead,
                            shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void HttpProxySession::Stop() {
    local_socket_.close();
    remote_socket_.close();
}

void HttpProxySession::HandleRead(const boost::system::error_code &e,
                                  std::size_t size) {
    std::cout << buffer_.data() << std::endl;
    local_socket_.async_send(boost::asio::buffer(std::string("HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello Proxy!")),
                             boost::bind(&HttpProxySession::HandleWrite,
                                         shared_from_this(),
                                         boost::asio::placeholders::error));
    // TODO implement me
}

void HttpProxySession::HandleWrite(const boost::system::error_code& e) {
    if(!e) {
        boost::system::error_code ec;
        local_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    }

    if(e != boost::asio::error::operation_aborted) {
        manager_.Stop(shared_from_this());
    }
}