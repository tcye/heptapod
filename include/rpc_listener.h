//
// Created by tiancai on 2017/3/14.
//

#ifndef HEPTAPOD_RPC_LISTENER_H
#define HEPTAPOD_RPC_LISTENER_H

#include "common.h"

namespace hpt {

class IoServicePool;
class RpcServerStream;

class RpcListener : public std::enable_shared_from_this<RpcListener>
{
    HPT_CLASS(RpcListener)

    static const int LISTEN_MAX_CONNECTIONS;
public:

    RpcListener(IoServicePool& io_service_pool, const asio::ip::tcp::endpoint& endpoint);
    ~RpcListener();

    void Close();
    bool StartListen();

    bool is_closed() { return _is_closed; }

private:
    void AsyncAccept();
    void OnAccept(std::shared_ptr<RpcServerStream> stream, const asio::error_code& error);

    IoServicePool& _io_service_pool;
    asio::ip::tcp::acceptor _acceptor;
    asio::ip::tcp::endpoint _endpoint;

    bool _is_closed;
};

} //namespace hpt


#endif //HEPTAPOD_RPC_LISTENER_H
