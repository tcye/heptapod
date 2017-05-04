//
// Created by tiancai on 2017/5/3.
//

#include "rpc_stream.h"

namespace hpt {

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
        logger()->info("close stream");
        _socket.shutdown(asio::ip::tcp::socket::shutdown_both);
        _socket.close();
    }
}

} // namespace hpt
