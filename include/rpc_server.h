//
// Created by tiancai on 2017/3/8.
//

#pragma once
#ifndef HEPTAPOD_SERVER_H
#define HEPTAPOD_SERVER_H

#include <string>
#include <mutex>
#include "common.h"

namespace hpt {

class RpcServer
{
    HPT_CLASS(RpcServer)
public:

    RpcServer(IoServicePool& io_service_pool);
    ~RpcServer();

    bool Start(const std::string& address, uint16_t port);
    void Stop();
    void WaitSignal();

private:
    IoServicePool& _io_service_pool;
    RpcListenerPtr _listener;
    bool _is_running;
    std::mutex _start_stop_lock;

};

} //namespace hpt


#endif //HEPTAPOD_SERVER_H
