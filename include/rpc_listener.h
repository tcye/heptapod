//
// Created by tiancai on 2017/3/14.
//

#ifndef HEPTAPOD_RPC_LISTENER_H
#define HEPTAPOD_RPC_LISTENER_H

#include "type_alias.h"
#include "io_service_pool.h"

namespace hpt {

class RpcListener : private noncopyable
{
public:
    RpcListener(IoServicePool& io_service_pool);
    ~RpcListener();

private:
    IoServicePool& _io_service_pool;
    Acceptor _acceptor;
    Endpoint _endpoint;
};

} //namespace hpt


#endif //HEPTAPOD_RPC_LISTENER_H
