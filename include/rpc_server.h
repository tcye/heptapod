//
// Created by tiancai on 2017/3/8.
//

#ifndef HEPTAPOD_SERVER_H
#define HEPTAPOD_SERVER_H

#include "common.h"

namespace hpt {

class RpcServer : private noncopyable
{
public:
    RpcServer(const Endpoint& endpoint);
    ~RpcServer();
private:
    IoServicePtr _io_service;
};

} //namespace hpt


#endif //HEPTAPOD_SERVER_H
