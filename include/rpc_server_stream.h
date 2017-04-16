//
// Created by tiancai on 2017/4/13.
//

#pragma once
#ifndef HEPTAPOD_RPC_SERVER_STREAM_H
#define HEPTAPOD_RPC_SERVER_STREAM_H

#include "common.h"
#include "msgpack.hpp"

namespace hpt {

class RpcServerStream : public std::enable_shared_from_this<RpcServerStream>
{
    HPT_CLASS(RpcServerStream)
    static const int READ_SIZE;

public:
    RpcServerStream(IoService& io_service);
    ~RpcServerStream();

    void SetSocketConnected();

    Socket& socket() { return _socket; }

private:
    void TriggerReceive();
    void TriggerSend();

    void TryReadSome();

    void OnReadSome(const asio::error_code& error, std::size_t transferred_size);

    Socket _socket;
    msgpack::unpacker _unpacker;
};

}

#endif //HEPTAPOD_RPC_SERVER_STREAM_H
