//
// Created by tiancai on 2017/4/19.
//

#include "rpc_client.h"
#include "io_service_pool.h"

namespace hpt {

RpcClient::RpcClient(IoServicePool& io_service_pool)
    : _io_service_pool(io_service_pool), _socket(io_service_pool.GetIoService()), _is_closed(false)
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
        _socket.shutdown(asio::ip::tcp::socket::shutdown_both);
        _socket.close();
    }
}

bool RpcClient::Connect(const std::string& address, uint16_t port)
{
    _socket.async_connect(MakeEndpoint(address, port), MEM_FN_UNSAFE(OnConnect, _1));
}

void RpcClient::OnConnect(const asio::error_code& error)
{
    if (error)
    {
        logger()->error("Connect Failed!");
        Shutdown();
        return;
    }

    logger()->info("Connect Success!");
    Shutdown();
}

} // namespace hpt

