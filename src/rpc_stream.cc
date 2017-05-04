//
// Created by tiancai on 2017/5/3.
//

#include <iostream>
#include "rpc_stream.h"

namespace hpt {

const int RpcStream::MAX_READ_SIZE_PER_TIME = 65536;

RpcStream::RpcStream(asio::io_service& io_service) : _socket(io_service), _status(STATUS_INIT)
{

}

RpcStream::~RpcStream()
{
    Close();
}

void RpcStream::Close()
{
    if (_status.exchange(STATUS_CLOSED) != STATUS_CLOSED)
    {
        _socket.shutdown(asio::ip::tcp::socket::shutdown_both);
        _socket.close();
        logger()->info("Close: closed");
    }
    else
    {
        logger()->info("CLose: duplicated close operation");
    }
}

void RpcStream::SetSocketConnected()
{
    int expected_status = STATUS_CONNECTING;
    if (_status.compare_exchange_strong(expected_status, STATUS_CONNECTED))
    {
        logger()->info("SetSocketConnected: succeeded");
        TriggerReceive();
        TriggerSend();
    }
    else
    {
        logger()->error("SetSocketConnected: last status is not expected! {}", expected_status);
        return;
    }
}

void RpcStream::TriggerReceive()
{
    logger()->info("TriggerReceive");
    TryReadSome();
}

void RpcStream::TriggerSend()
{
    logger()->info("TriggerSend");
}

void RpcStream::TryReadSome()
{
    _unpacker.reserve_buffer(MAX_READ_SIZE_PER_TIME);
    _socket.async_read_some(asio::buffer(_unpacker.buffer(), MAX_READ_SIZE_PER_TIME),
                            MEM_FN(OnReadSome, _1, _2));
}

void RpcStream::OnReadSome(const asio::error_code& ec, std::size_t transferred_size)
{
    if (ec)
    {
        logger()->error("OnReadSome: Failed! reason: {}", ec.message());
        Close();
        return;
    }

    _unpacker.buffer_consumed(transferred_size);
    msgpack::unpacked result;
    while (_unpacker.next(result))
    {
        msgpack::object obj(result.get());
        std::cout << obj << std::endl;
    }

    if (_status != STATUS_CLOSED)
    {
        TryReadSome();
    }
}

} // namespace hpt
