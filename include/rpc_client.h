//
// Created by tiancai on 2017/4/19.
//

#ifndef HEPTAPOD_RPC_CLIENT_H
#define HEPTAPOD_RPC_CLIENT_H

#include "common.h"
#include "rpc_side.h"

namespace hpt {

template <typename ServiceType>
class RpcClient : public RpcSide<ServiceType>
{
    HPT_CLASS(RpcClient)
public:
    RpcClient() : _is_closed(true)
    {
    }

    ~RpcClient()
    {
        Shutdown();
    }

    bool Connect(const std::string& address, uint16_t port)
    {
        _rpc_service = std::make_shared<ServiceType>();
        _rpc_service->Open(GetIoService());
        _rpc_service->socket().async_connect(MakeEndpoint(address, port), MEM_FN_UNSAFE(OnConnect, _1));
        _rpc_service->set_status(ServiceType::STATUS_CONNECTING);
        _is_closed = false;
        return true;
    }

    void Shutdown()
    {
        if (!_is_closed.exchange(true))
        {
            _rpc_service.reset();
        }
    }

private:
    void OnConnect(const asio::error_code& ec)
    {
        if (ec)
        {
            logger()->error("OnConnect: Failed!");
            Shutdown();
            return;
        }

        logger()->info("OnConnect: Success!");
        _rpc_service->SetSocketConnected();
    }

    std::atomic_bool _is_closed;
    std::shared_ptr<ServiceType> _rpc_service;
};

} // namespace hpt


#endif //HEPTAPOD_RPC_CLIENT_H
