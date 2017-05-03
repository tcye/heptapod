//
// Created by tiancai on 2017/5/3.
//

#ifndef HEPTAPOD_RPC_STREAM_H
#define HEPTAPOD_RPC_STREAM_H

#include "common.h"

namespace hpt {

class RpcStream : public std::enable_shared_from_this<RpcStream>
{
    HPT_CLASS(RpcStream)
public:
    RpcStream(IoService& io_service);

private:
    void TriggerReceive();
    void TriggerSend();
    void TryReadSome();
    void OnReadSome(const asio::error_code& error, std::size_t transferred_size);

    Socket _socket;
    msgpack::unpacker _unpacker;
};

} // namespace hpt


#endif //HEPTAPOD_RPC_STREAM_H
