//
// Created by tiancai on 2017/4/19.
//

#ifndef HEPTAPOD_RPC_CLIENT_H
#define HEPTAPOD_RPC_CLIENT_H

#include <string>
#include "common.h"
#include "wait_signal_func.h"

namespace hpt {

class RpcClient : public WaitSignalFunc
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
    Socket _socket;
};

} // namespace hpt


#endif //HEPTAPOD_RPC_CLIENT_H
