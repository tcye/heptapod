//
// Created by tiancai on 2017/5/3.
//

#include <iostream>
#include "rpc_side.h"
#include "rpc_stream.h"
#include "rpc_stream_manager.h"

namespace hpt {

const int RpcStream::MAX_READ_SIZE_PER_TIME = 65536;

RpcStream::RpcStream(RpcSide& side)
    : _rpc_side(side),
      _socket(side.GetIoService()),
      _write_strand(_socket.get_io_service()),
      _status(STATUS_INIT)
{
    RpcStreamManager::Instance().Add(this);
}

RpcStream::~RpcStream()
{
    RpcStreamManager::Instance().Del(this);
    Close();
}

void RpcStream::Close()
{
    if (_status.exchange(STATUS_CLOSED) == STATUS_CONNECTED)
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
        _rpc_side.Dispatch(*this, obj);
    }

    if (_status == STATUS_CONNECTED)
    {
        TryReadSome();
    }
}

void RpcStream::DoCallRemote(std::shared_ptr<msgpack::sbuffer> buf)
{
    _socket.async_write_some(asio::buffer(buf->data(), buf->size()), _write_strand.wrap(MEM_FN(OnWriteSome, _1, _2)));
    _write_queue.push_back(std::move(*buf));
}

void RpcStream::OnWriteSome(const asio::error_code& ec, std::size_t transferred_size)
{
    if (ec)
    {
        return;
    }
    _bytes_written += transferred_size;

    if (!_write_queue.empty())
    {
        size_t front_buf_size = _write_queue.front().size();
        if (_bytes_written >= front_buf_size)
        {
            _write_queue.pop_front();
            _bytes_written -= front_buf_size;
        }
    }
}


} // namespace hpt
