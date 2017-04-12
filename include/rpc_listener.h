//
// Created by tiancai on 2017/3/14.
//

#ifndef HEPTAPOD_RPC_LISTENER_H
#define HEPTAPOD_RPC_LISTENER_H

#include "common.h"
#include "io_service_pool.h"

namespace hpt {

class RpcListener : private noncopyable
{
public:
    static int LISTEN_MAX_CONNECTIONS = 4096;

    RpcListener(IoServicePool& io_service_pool, const Endpoint& endpoint);
    ~RpcListener();

    void Close();
    bool StartListen();

private:
    void AsyncAccept();

    IoServicePool& _io_service_pool;
    Acceptor _acceptor;
    Endpoint _endpoint;
};

} //namespace hpt


#endif //HEPTAPOD_RPC_LISTENER_H
