//
// Created by tiancai on 2017/4/13.
//

#pragma once
#ifndef HEPTAPOD_RPC_SERVER_STREAM_H
#define HEPTAPOD_RPC_SERVER_STREAM_H

#include "common.h"

namespace hpt {

class RpcServerStream : std::enable_shared_from_this<RpcServerStream>
{
    HPT_CLASS(RpcServerStream)
public:
    RpcServerStream(IoService& io_service);
    ~RpcServerStream();

    void SetSocketConnected();

    Socket& socket() { return _socket; }

private:
    void TriggerReceive();
    void TriggerSend();

    Socket _socket;
};

}

#endif //HEPTAPOD_RPC_SERVER_STREAM_H
