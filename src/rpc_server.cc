//
// Created by tiancai on 2017/3/8.
//

#include <thread>
#include <iostream>

#include "rpc_server.h"
#include "rpc_listener.h"
#include "io_service_pool.h"

using namespace std::chrono_literals;

namespace hpt {

RpcServer::RpcServer(IoServicePool& io_service_pool)
    : _is_running(false), _io_service_pool(io_service_pool)
{

}

RpcServer::~RpcServer()
{
    Stop();
}

bool RpcServer::Start(const std::string& address, uint16_t port)
{
    std::lock_guard<std::mutex> guard(_start_stop_lock);

    if (_is_running)
        return true;

    _listener = std::make_shared<RpcListener>(_io_service_pool, hpt::MakeEndpoint(address, port));

    if (!_listener->StartListen())
    {
        logger()->error("Server start failed! listener start listener error.");
        return false;
    }

    _is_running = true;
    return true;
}

void RpcServer::Stop()
{
    std::lock_guard<std::mutex> guard(_start_stop_lock);

    if (!_is_running)
        return;

    _listener->Close();
    _listener.reset();
    _is_running = false;
}

static volatile bool s_quit = false;
static void SignalHandler(int /*sig*/)
{
    s_quit = true;
}

void RpcServer::WaitSignal()
{
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    while (!s_quit)
    {
        std::this_thread::sleep_for(1s);
    }
}

} //namespace hpt
