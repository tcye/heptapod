//
// Created by tiancai on 2017/3/8.
//

#ifndef HEPTAPOD_SERVER_H
#define HEPTAPOD_SERVER_H

#include <string>
#include <mutex>
#include "common.h"
#include "rpc_side.h"

namespace hpt {

class IoServicePool;
class RpcListener;

class RpcServer : public RpcSide
{
    HPT_CLASS(RpcServer)
public:

    RpcServer(IoServicePool& io_service_pool);
    ~RpcServer();

    bool Start(const std::string& address, uint16_t port);
    void Stop();

private:
    std::shared_ptr<RpcListener> _listener;
    bool _is_running;
    std::mutex _start_stop_lock;

};

} //namespace hpt


#endif //HEPTAPOD_SERVER_H
