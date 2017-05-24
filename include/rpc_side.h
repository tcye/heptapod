//
// Created by tiancai on 2017/4/20.
//

#ifndef HEPTAPOD_WAIT_SIGNAL_H
#define HEPTAPOD_WAIT_SIGNAL_H

#include "signal.h"
#include <thread>
#include "io_service_pool.h"

using namespace std::chrono_literals;

namespace hpt {

template <typename ServiceType>
class RpcSide
{
public:
    RpcSide()
    {
        ServiceType::InitServiceMap();
        _io_service_pool.Run();
    }
    virtual ~RpcSide() { _io_service_pool.Stop(); }

    static void WaitSignal()
    {
        signal(SIGINT, SignalHandler);
        signal(SIGTERM, SignalHandler);
        while (!s_quit)
        {
            std::this_thread::sleep_for(1s);
        }
    }

    asio::io_service& GetIoService() { return _io_service_pool.GetIoService(); }
    IoServicePool& io_service_pool() { return _io_service_pool; }

protected:
    IoServicePool _io_service_pool;

private:
    static void SignalHandler(int /*sig*/)
    {
        s_quit = true;
    }
    static volatile bool s_quit;
};

template <typename ServiceType>
bool volatile RpcSide<ServiceType>::s_quit = false;

} // namespace hpt

#endif //HEPTAPOD_WAIT_SIGNAL_H
