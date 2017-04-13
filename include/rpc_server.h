//
// Created by tiancai on 2017/3/8.
//

#ifndef HEPTAPOD_SERVER_H
#define HEPTAPOD_SERVER_H

#include <mutex>
#include "common.h"

namespace hpt {

class RpcServer
{
    HPT_CLASS(RpcServer)
public:
    static const int POOL_SIZE;
    static const int POOL_THREAD_NUM;

    RpcServer();
    ~RpcServer();

    bool Start(const Endpoint& endpoint);
    void Run();
    void Stop();

private:
    IoServicePoolPtr _io_service_pool;
    RpcListenerPtr _listener;
    bool _is_running;
    std::mutex _start_stop_lock;

};

} //namespace hpt


#endif //HEPTAPOD_SERVER_H
