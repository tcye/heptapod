//
// Created by tiancai on 2017/3/8.
//

#pragma once
#ifndef HEPTAPOD_SERVER_H
#define HEPTAPOD_SERVER_H

#include <string>
#include <mutex>
#include "common.h"
#include "wait_signal_func.h"

namespace hpt {

class RpcServer : public WaitSignalFunc
{
    HPT_CLASS(RpcServer)
public:

    RpcServer(IoServicePool& io_service_pool);
    ~RpcServer();

    bool Start(const std::string& address, uint16_t port);
    void Stop();

private:
    IoServicePool& _io_service_pool;
    RpcListenerPtr _listener;
    bool _is_running;
    std::mutex _start_stop_lock;

};

} //namespace hpt


#endif //HEPTAPOD_SERVER_H
