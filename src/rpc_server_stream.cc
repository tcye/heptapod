//
// Created by tiancai on 2017/4/13.
//

#include "rpc_server_stream.h"

namespace hpt {

const int RpcServerStream::READ_SIZE = 65536;

RpcServerStream::RpcServerStream(IoService& io_service) : _socket(io_service)
{

}

RpcServerStream::~RpcServerStream()
{

}

void RpcServerStream::SetSocketConnected()
{
    TriggerReceive();
    TriggerSend();
}

void RpcServerStream::TriggerReceive()
{
    TryReadSome();
}

void RpcServerStream::TriggerSend()
{

}

void RpcServerStream::TryReadSome()
{
    _unpacker.reserve_buffer(READ_SIZE);
    _socket.async_read_some(
        asio::buffer(_unpacker.buffer(), READ_SIZE),
        MEM_FN(OnReadSome, _1, _2)
    );
}

void RpcServerStream::OnReadSome(const asio::error_code& error, std::size_t transferred_size)
{

}

}
