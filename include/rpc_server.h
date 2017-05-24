//
// Created by tiancai on 2017/3/8.
//

#ifndef HEPTAPOD_SERVER_H
#define HEPTAPOD_SERVER_H

#include <mutex>
#include "rpc_side.h"
#include "rpc_listener.h"

namespace hpt {


template <typename ServiceType>
class RpcServer : public RpcSide<ServiceType>
{
    HPT_CLASS(RpcServer)
public:

    RpcServer() : _is_running(false)
    {
    }

    ~RpcServer()
    {
        Stop();
    }

    bool Start(const std::string& address, uint16_t port)
    {
        std::lock_guard<std::mutex> guard(_start_stop_lock);

        if (_is_running)
            return true;

        _listener = std::make_shared<RpcListener<ServiceType>>(_io_service_pool, hpt::MakeEndpoint(address, port));

        if (!_listener->StartListen())
        {
            logger()->error("Server start failed! listener start listener error.");
            return false;
        }

        _is_running = true;
        return true;
    }
    void Stop()
    {
        std::lock_guard<std::mutex> guard(_start_stop_lock);

        if (!_is_running)
            return;

        _listener->Close();
        _listener.reset();
        _is_running = false;
    }

private:
    std::shared_ptr<RpcListener<ServiceType>> _listener;
    bool _is_running;
    std::mutex _start_stop_lock;
};

} //namespace hpt


#endif //HEPTAPOD_SERVER_H
