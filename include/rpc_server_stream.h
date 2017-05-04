//
// Created by tiancai on 2017/4/13.
//

#ifndef HEPTAPOD_RPC_SERVER_STREAM_H
#define HEPTAPOD_RPC_SERVER_STREAM_H

#include <atomic>
#include "common.h"
#include "msgpack.hpp"

namespace hpt {

class RpcServerStream : public std::enable_shared_from_this<RpcServerStream>
{
    HPT_CLASS(RpcServerStream)
    static const int READ_SIZE;

public:
    RpcServerStream(asio::io_service& io_service);
    ~RpcServerStream();

    void SetSocketConnected();
    void Close();

    asio::ip::tcp::socket& socket() { return _socket; }

private:
    void TriggerReceive();
    void TriggerSend();

    void TryReadSome();

    void OnReadSome(const asio::error_code& error, std::size_t transferred_size);

    asio::ip::tcp::socket _socket;
    msgpack::unpacker _unpacker;
    std::atomic_bool _is_closed;
};

}

#endif // HEPTAPOD_RPC_SERVER_STREAM_H
