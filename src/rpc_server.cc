//
// Created by tiancai on 2017/3/8.
//

#include <signal.h>
#include <thread>
#include <chrono>
#include <iostream>

#include "rpc_server.h"
#include "io_service_pool.h"
#include "rpc_listener.h"
#include "logging.h"

using namespace std::chrono_literals;

namespace hpt {

const int RpcServer::POOL_SIZE = 1;
const int RpcServer::POOL_THREAD_NUM = 1;

RpcServer::RpcServer() : _is_running(false)
{
    InitHptLogging();
}

RpcServer::~RpcServer()
{
    Stop();
    ShutdownHptLogging();
}

bool RpcServer::Start(const Endpoint& endpoint)
{
    std::lock_guard<std::mutex> guard(_start_stop_lock);

    if (_is_running)
        return true;

    _io_service_pool = std::make_shared<IoServicePool>(POOL_SIZE, POOL_THREAD_NUM);
    _listener = std::make_shared<RpcListener>(*_io_service_pool, endpoint);

    if (!_io_service_pool->Run())
    {
        LOG->error("Server start failed! io_service_pool run error.");
        return false;
    }

    if (!_listener->StartListen())
    {
        LOG->error("Server start failed! listener start listener error.");
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
    _io_service_pool->Stop();

    _listener.reset();
    _io_service_pool.reset();

    _is_running = false;
}

static volatile bool s_quit = false;
static void SignalHandler(int /*sig*/)
{
    s_quit = true;
}

void RpcServer::Run()
{
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    while (!s_quit)
    {
        std::this_thread::sleep_for(1s);
    }
}

} //namespace hpt
