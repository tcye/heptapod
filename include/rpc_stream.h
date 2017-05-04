//
// Created by tiancai on 2017/5/3.
//

#ifndef HEPTAPOD_RPC_STREAM_H
#define HEPTAPOD_RPC_STREAM_H

#include "common.h"
#include "msgpack.hpp"

namespace hpt {

class RpcStream : public std::enable_shared_from_this<RpcStream>
{
    HPT_CLASS(RpcStream)
    static const int MAX_READ_SIZE_PER_TIME;
public:
    enum
    {
        STATUS_INIT = 0,
        STATUS_CONNECTING = 1,
        STATUS_CONNECTED = 2,
        STATUS_CLOSED = 3,
    };

    RpcStream(asio::io_service& io_service);
    ~RpcStream();

    void SetSocketConnected();
    void Close();

    asio::ip::tcp::socket& socket() { return _socket; }
    int status() { return _status; }
    void set_status(int status) { _status = status; }

private:
    void TriggerReceive();
    void TriggerSend();
    void TryReadSome();
    void OnReadSome(const asio::error_code& error, std::size_t transferred_size);

    asio::ip::tcp::socket _socket;
    msgpack::unpacker _unpacker;
    std::atomic_int _status;
};

} // namespace hpt


#endif //HEPTAPOD_RPC_STREAM_H
