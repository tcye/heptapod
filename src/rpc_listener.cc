//
// Created by tiancai on 2017/3/14.
//
#include "rpc_listener.h"

namespace hpt {

RpcListener::RpcListener(IoServicePool& io_service_pool)
        : _io_service_pool(io_service_pool)
{

}

RpcListener::~RpcListener()
{

}

}
