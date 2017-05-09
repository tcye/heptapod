//
// Created by tiancai on 2017/4/20.
//

#ifndef HEPTAPOD_WAIT_SIGNAL_H
#define HEPTAPOD_WAIT_SIGNAL_H

#include "rpc_dispatcher.h"
#include "io_service_pool.h"

namespace hpt {

class IoServicePool;
class RpcDispatcher;
class RpcStream;

class RpcSide
{
public:
    RpcSide(IoServicePool& io_service_pool);
    virtual ~RpcSide();

    void WaitSignal();

    void Dispatch(RpcStream& s, const msgpack::object& object);
    template <typename F> void Bind(const std::string& name, F func);

    asio::io_service& GetIoService() { return _io_service_pool.GetIoService(); }
    IoServicePool& io_service_pool() { return _io_service_pool; }

protected:
    IoServicePool& _io_service_pool;
    RpcDispatcher _rpc_dispatcher;
};

template <typename F>
void RpcSide::Bind(const std::string& name, F func)
{
    _rpc_dispatcher.Bind(name, func);
}

} // namespace hpt

#endif //HEPTAPOD_WAIT_SIGNAL_H
