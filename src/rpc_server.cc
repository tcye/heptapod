//
// Created by tiancai on 2017/3/8.
//

#include <thread>
#include <iostream>

#include "rpc_server.h"
#include "rpc_listener.h"
#include "io_service_pool.h"
#include "rpc_stream.h"
#include "rpc_stream_manager.h"

using namespace std::chrono_literals;

namespace hpt {

RpcServer::RpcServer(IoServicePool& io_service_pool)
    : RpcSide(io_service_pool), _is_running(false)
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

    _listener = std::make_shared<RpcListener>(*this, hpt::MakeEndpoint(address, port));

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

    for (auto stream : RpcStreamManager::Instance())
    {
        stream->Close();
    }
    RpcStreamManager::Instance().Clear();

    _is_running = false;
}

} //namespace hpt
