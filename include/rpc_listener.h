//
// Created by tiancai on 2017/3/14.
//

#ifndef HEPTAPOD_RPC_LISTENER_H
#define HEPTAPOD_RPC_LISTENER_H

#include "common.h"

namespace hpt {

class RpcServer;
class IoServicePool;
class RpcServerStream;

class RpcListener : public std::enable_shared_from_this<RpcListener>
{
    HPT_CLASS(RpcListener)

    static const int LISTEN_MAX_CONNECTIONS;
public:

    RpcListener(RpcServer& server, const asio::ip::tcp::endpoint& endpoint);
    ~RpcListener();

    void Close();
    bool StartListen();

private:
    void AsyncAccept();
    void OnAccept(std::shared_ptr<RpcServerStream> stream, const asio::error_code& error);

    RpcServer& _server;
    asio::ip::tcp::acceptor _acceptor;
    asio::ip::tcp::endpoint _endpoint;

    enum
    {
        STATUS_INIT = 0,
        STATUS_LISTENING = 1,
        STATUS_CLOSED = 2,
    };

    std::atomic_int _status;
};

} //namespace hpt


#endif //HEPTAPOD_RPC_LISTENER_H
