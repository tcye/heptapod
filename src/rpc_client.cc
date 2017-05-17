//
// Created by tiancai on 2017/4/19.
//

#include <tuple>
#include "rpc_client.h"
#include "rpc_client_stream.h"
#include "io_service_pool.h"

namespace hpt {

RpcClient::RpcClient(IoServicePool& io_service_pool)
    : RpcSide(io_service_pool), _is_closed(false)
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
    _rpc_stream = std::make_shared<RpcClientStream>(*this);
    _rpc_stream->socket().async_connect(MakeEndpoint(address, port), MEM_FN_UNSAFE(OnConnect, _1));
    _rpc_stream->set_status(RpcClientStream::STATUS_CONNECTING);
	return true;
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

    _rpc_stream->CallRemote("echo", "helloworld");
}

} // namespace hpt

