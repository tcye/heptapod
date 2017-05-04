//
// Created by tiancai on 2017/4/19.
//

#include "rpc_client.h"
#include "rpc_client_stream.h"
#include "io_service_pool.h"

namespace hpt {

RpcClient::RpcClient(IoServicePool& io_service_pool)
    : _io_service_pool(io_service_pool), _is_closed(false)
{

}

RpcClient::~RpcClient()
{
    Shutdown();
}

void RpcClient::Shutdown()
{
    if (!_is_closed.exchange(true))
    {
        _rpc_stream.reset();
    }
}

bool RpcClient::Connect(const std::string& address, uint16_t port)
{
    _rpc_stream = std::make_shared<RpcClientStream>(_io_service_pool.GetIoService());
    _rpc_stream->socket().async_connect(MakeEndpoint(address, port), MEM_FN_UNSAFE(OnConnect, _1));
    _rpc_stream->set_status(RpcClientStream::STATUS_CONNECTING);
}

void RpcClient::OnConnect(const asio::error_code& ec)
{
    if (ec)
    {
        logger()->error("OnConnect: Failed!");
        Shutdown();
        return;
    }

    logger()->info("OnConnect: Success!");
    _rpc_stream->SetSocketConnected();

    std::string s = "hello world";
    msgpack::sbuffer buf;
    msgpack::pack(buf, s);
    _rpc_stream->socket().write_some(asio::buffer(buf.data(), buf.size()));
}

} // namespace hpt

