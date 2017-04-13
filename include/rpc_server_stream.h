//
// Created by tiancai on 2017/4/13.
//

#ifndef HEPTAPOD_RPC_SERVER_STREAM_H
#define HEPTAPOD_RPC_SERVER_STREAM_H

#include "common.h"

namespace hpt {

class RpcServerStream
{
    HPT_CLASS(RpcServerStream)
public:
    RpcServerStream(IoService& io_service);
    ~RpcServerStream();

    Socket& socket();

private:
    Socket _socket;
};

}

#endif //HEPTAPOD_RPC_SERVER_STREAM_H
