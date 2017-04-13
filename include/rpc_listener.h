//
// Created by tiancai on 2017/3/14.
//

#ifndef HEPTAPOD_RPC_LISTENER_H
#define HEPTAPOD_RPC_LISTENER_H

#include "common.h"
#include "io_service_pool.h"

namespace hpt {

class RpcListener : private noncopyable, public std::enable_shared_from_this<RpcListener>
{
public:
    using SelfType = RpcListener;
    static const int LISTEN_MAX_CONNECTIONS = 4096;

    RpcListener(IoServicePool& io_service_pool, const Endpoint& endpoint);
    ~RpcListener();

    void Close();
    bool StartListen();

    bool is_closed() { return _is_closed; }

private:
    void AsyncAccept();
    void OnAccept(RpcServerStreamPtr stream, const asio::error_code& error);

    IoServicePool& _io_service_pool;
    Acceptor _acceptor;
    Endpoint _endpoint;

    bool _is_closed;
};

} //namespace hpt


#endif //HEPTAPOD_RPC_LISTENER_H
