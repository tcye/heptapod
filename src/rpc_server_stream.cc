//
// Created by tiancai on 2017/4/13.
//

#include <iostream>
#include "rpc_server_stream.h"

namespace hpt {

const int RpcServerStream::READ_SIZE = 65536;

RpcServerStream::RpcServerStream(IoService& io_service) : _socket(io_service), _is_closed(true)
{

}

RpcServerStream::~RpcServerStream()
{

}

void RpcServerStream::SetSocketConnected()
{
    _is_closed = false;
    TriggerReceive();
    TriggerSend();
}

void RpcServerStream::Close()
{
    if (!_is_closed.exchange(true))
    {
        logger()->info("now shutdown stream");
        _socket.shutdown(asio::ip::tcp::socket::shutdown_both);
        _socket.close();
    }
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
    _socket.async_read_some(asio::buffer(_unpacker.buffer(), READ_SIZE),
                            MEM_FN(OnReadSome, _1, _2));
}

void RpcServerStream::OnReadSome(const asio::error_code& error, std::size_t transferred_size)
{
    if (error)
    {
        logger()->error("OnReadSome Failed! reason: {}", error.message());
        Close();
        return;
    }

    if (_is_closed)
    {
        logger()->error("OnReadSome: stream already closed!");
        return;
    }

    _unpacker.buffer_consumed(transferred_size);
    msgpack::unpacked result;
    while (_unpacker.next(result))
    {
        msgpack::object obj(result.get());
        std::cout << obj << std::endl;
    }

    if (!_is_closed)
    {
        TryReadSome();
    }
}

} // namespace hpt
