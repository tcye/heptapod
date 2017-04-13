//
// Created by tiancai on 2017/3/14.
//
#include "rpc_listener.h"
#include "rpc_server_stream.h"
#include "logging.h"

namespace hpt {

RpcListener::RpcListener(IoServicePool& io_service_pool,
                         const Endpoint& endpoint)
        : _io_service_pool(io_service_pool),
          _endpoint(endpoint),
          _acceptor(io_service_pool.GetIoService()),
          _is_closed(true)
{

}

RpcListener::~RpcListener()
{
    Close();
}

void RpcListener::Close()
{
    if (_is_closed)
        return;

    _acceptor.cancel();
    _acceptor.close();
}

bool RpcListener::StartListen()
{
    asio::error_code ec;
    _acceptor.open(_endpoint.protocol(), ec);
    if (ec)
    {
        LOG->error("StartListen, open failed! reason: {}", ec.message());
        return false;
    }
    _acceptor.set_option(Acceptor::reuse_address(true), ec);
    if (ec)
    {
        LOG->error("StartListen, set reuse address failed! reason: {}", ec.message());
        return false;
    }
    _acceptor.bind(_endpoint, ec);
    if (ec)
    {
        LOG->error("StartListen, bind failed! reason: {}", ec.message());
        return false;
    }
    _acceptor.listen(LISTEN_MAX_CONNECTIONS, ec);
    if (ec)
    {
        LOG->error("StartListen, listen failed! reason: {}", ec.message());
        return false;
    }

    AsyncAccept();
    _is_closed = false;
    return true;
}

void RpcListener::AsyncAccept()
{
    RpcServerStreamPtr stream = std::make_shared<RpcServerStream>(_io_service_pool.GetIoService());
    _acceptor.async_accept(stream->socket(), MEM_FN(OnAccept, stream, _1));
}

void RpcListener::OnAccept(RpcServerStreamPtr stream, const asio::error_code& error)
{
    if (error)
    {
        LOG->error("OnAccept failed! reason: {}", error.message());
        return;
    }

    if (_is_closed)
    {
        LOG->info("OnAccept listener already closed.");
        return;
    }

    AsyncAccept();
}

} // namespace hpt
