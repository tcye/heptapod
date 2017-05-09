//
// Created by tiancai on 2017/4/20.
//

#include <thread>
#include <csignal>
#include "common.h"
#include "io_service_pool.h"
#include "rpc_dispatcher.h"
#include "rpc_side.h"

using namespace std::chrono_literals;

namespace hpt {

static volatile bool s_quit = false;
static void SignalHandler(int /*sig*/)
{
    s_quit = true;
}

void RpcSide::WaitSignal()
{
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    while (!s_quit)
    {
        std::this_thread::sleep_for(1s);
    }
}

RpcSide::RpcSide(IoServicePool& io_service_pool) : _io_service_pool(io_service_pool)
{

}

RpcSide::~RpcSide()
{

}

void RpcSide::Dispatch(RpcStream& s, const msgpack::object& object)
{
    _rpc_dispatcher.Dispatch(object);
}

}
