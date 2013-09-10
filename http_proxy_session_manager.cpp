#include <boost/bind.hpp>
#include "http_proxy_session_manager.h"

HttpProxySessionManager::HttpProxySessionManager() {
}

HttpProxySessionManager::~HttpProxySessionManager() {
    // do nothing here
}

void HttpProxySessionManager::Start(HttpProxySessionPtr session) {
    sessions_.insert(session);
    session->Start();
}

void HttpProxySessionManager::Stop(HttpProxySessionPtr session) {
    sessions_.erase(session);
    session->Stop();
}

void HttpProxySessionManager::StopAll() {
    std::for_each(sessions_.begin(), sessions_.end(),
                  boost::bind(&HttpProxySession::Stop, _1));
    sessions_.clear();
}