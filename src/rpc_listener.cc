//
// Created by tiancai on 2017/3/14.
//
#include "rpc_listener.h"
#include "io_service_pool.h"
#include "rpc_server_stream.h"

namespace hpt {

const int RpcListener::LISTEN_MAX_CONNECTIONS = 4096;

RpcListener::RpcListener(IoServicePool& io_service_pool,
                         const asio::ip::tcp::endpoint& endpoint)
        : _io_service_pool(io_service_pool),
          _endpoint(endpoint),
          _acceptor(io_service_pool.GetIoService()),
          _status(STATUS_INIT)
{

}

RpcListener::~RpcListener()
{
    Close();
}

void RpcListener::Close()
{
    if (_status.exchange(STATUS_CLOSED) == STATUS_LISTENING)
    {
        _acceptor.cancel();
        _acceptor.close();
        logger()->info("Close: closed");
    }
}

bool RpcListener::StartListen()
{
    int expected_status = STATUS_INIT;
    if (_status.compare_exchange_strong(expected_status, STATUS_LISTENING))
    {
        asio::error_code ec;
        _acceptor.open(_endpoint.protocol(), ec);
        if (ec)
        {
            logger()->error("StartListen: open failed, reason: {}", ec.message());
            return false;
        }
        _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true), ec);
        if (ec)
        {
            logger()->error("StartListen: set_option failed, reason: {}", ec.message());
            return false;
        }
        _acceptor.bind(_endpoint, ec);
        if (ec)
        {
            logger()->error("StartListen: bind failed, reason: {}", ec.message());
            return false;
        }
        _acceptor.listen(LISTEN_MAX_CONNECTIONS, ec);
        if (ec)
        {
            logger()->error("StartListen: listen failed, reason: {}", ec.message());
            return false;
        }

        AsyncAccept();
        logger()->info("StartListen: succeeded");
        return true;
    }
    else
    {
        logger()->error("StartListen: check status failed! last status is {}", expected_status);
        return false;
    }
}

void RpcListener::AsyncAccept()
{
    auto stream = std::make_shared<RpcServerStream>(_io_service_pool.GetIoService());
    _acceptor.async_accept(stream->socket(), MEM_FN(OnAccept, stream, _1));
    stream->set_status(RpcServerStream::STATUS_CONNECTING);
}

void RpcListener::OnAccept(std::shared_ptr<RpcServerStream> stream, const asio::error_code& ec)
{
    if (ec)
    {
        logger()->error("OnAccept: failed, reason: {}", ec.message());
        Close();
        return;
    }

    logger()->info("OnAccept: succeeded");
    stream->SetSocketConnected();
    AsyncAccept();
}

} // namespace hpt
