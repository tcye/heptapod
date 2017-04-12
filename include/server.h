//
// Created by tiancai on 2017/3/8.
//

#ifndef HEPTAPOD_SERVER_H
#define HEPTAPOD_SERVER_H

#include "common.h"

namespace hpt {

class Server : private noncopyable
{
public:
    Server(const Endpoint& endpoint);
    ~Server();
private:
    IoServicePtr _io_service;
};

} //namespace hpt


#endif //HEPTAPOD_SERVER_H
