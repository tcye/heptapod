//
// Created by tiancai on 2017/4/19.
//

#ifndef HEPTAPOD_RPC_CLIENT_H
#define HEPTAPOD_RPC_CLIENT_H

#include "common.h"
#include "rpc_side.h"

namespace hpt {

class IoServicePool;
class RpcClientStream;

class RpcClient : public RpcSide
{
    HPT_CLASS(RpcClient)
public:
    RpcClient(IoServicePool& io_service_pool);
    ~RpcClient();

    bool Connect(const std::string& address, uint16_t port);
    void Shutdown();

private:
    void OnConnect(const asio::error_code& error);

private:
    IoServicePool& _io_service_pool;
    std::atomic_bool _is_closed;
    std::shared_ptr<RpcClientStream> _rpc_stream;
};

} // namespace hpt


#endif //HEPTAPOD_RPC_CLIENT_H
