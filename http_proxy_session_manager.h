#ifndef HTTP_PROXY_SESSION_MANAGER_H
#define HTTP_PROXY_SESSION_MANAGER_H

#include <set>
#include <boost/noncopyable.hpp>
#include "http_proxy_session.h"
#include "request_dispatcher.h"


class HttpProxySessionManager : private boost::noncopyable {
public:
    HttpProxySessionManager();
    ~HttpProxySessionManager();

    void Start(HttpProxySessionPtr session);
    void Stop(HttpProxySessionPtr session);
    void StopAll();

    RequestDispatcher& dispatcher() { return dispatcher_; }

private:
    std::set<HttpProxySessionPtr> sessions_;
    RequestDispatcher dispatcher_;
};

#endif // HTTP_PROXY_SESSION_MANAGER_H
