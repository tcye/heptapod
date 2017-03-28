//
// Created by tiancai on 2017/3/14.
//
#include "rpc_listener.h"

namespace hpt {

RpcListener::RpcListener(IoServicePool& io_service_pool,
                         const Endpoint& endpoint)
        : _io_service_pool(io_service_pool),
          _endpoint(endpoint),
          _acceptor(io_service_pool.GetIoService())
{

}

RpcListener::~RpcListener()
{
    Close();
}

void RpcListener::Close()
{

}

bool RpcListener::StartListen()
{
    asio::error_code ec;
    _acceptor.open(_endpoint.protocol(), ec);
    if (ec)
    {
        std::cerr << "StartListen, open failed! " << ec.message() << std::endl;
        return false;
    }
    _acceptor.set_option(Acceptor::reuse_address(true), ec);
    if (ec)
    {
        std::cerr << "StartListen, set option failed! " << ec.message() << std::endl;
        return false;
    }
    _acceptor.bind(_endpoint, ec);
    if (ec)
    {
        std::cerr << "StartListen, bind failed! " << ec.message() << std::endl;
        return false;
    }
    _acceptor.listen(LISTEN_MAX_CONNECTIONS, ec);
    if (ec)
    {
        std::cerr << "StartListen, listen failed! " << ec.message() << std::endl;
        return false;
    }

    AsyncAccept();
    return true;
}

void RpcListener::AsyncAccept()
{
    _acceptor.async_accept()
}

}
