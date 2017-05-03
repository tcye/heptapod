//
// Created by tiancai on 2017/4/19.
//

#include "rpc_client.h"
#include "io_service_pool.h"

namespace hpt {

RpcClient::RpcClient(IoServicePool& io_service_pool) : _io_service_pool(io_service_pool)
{

}

RpcClient::~RpcClient()
{

}

} // namespace hpt

