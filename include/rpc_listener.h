//
// Created by tiancai on 2017/3/14.
//

#ifndef HEPTAPOD_RPC_LISTENER_H
#define HEPTAPOD_RPC_LISTENER_H

#include "io_service_pool.h"

namespace hpt {


template <typename ServiceType>
class RpcListener : public std::enable_shared_from_this<RpcListener<ServiceType>>
{
    HPT_CLASS(RpcListener)
    static const int LISTEN_MAX_CONNECTIONS = 4096;
    enum
    {
        STATUS_INIT = 0,
        STATUS_LISTENING = 1,
        STATUS_CLOSED = 2,
    };

public:
    RpcListener(IoServicePool& io_service_pool, const asio::ip::tcp::endpoint& endpoint)
        : _io_service_pool(io_service_pool),
          _acceptor(io_service_pool.GetIoService()),
          _endpoint(endpoint),
          _status(STATUS_INIT)
    {
    }

    ~RpcListener()
    {
        Close();
    }

    void Close()
    {
        if (_status.exchange(STATUS_CLOSED) == STATUS_LISTENING)
        {
            _acceptor.cancel();
            _acceptor.close();
            logger()->info("Close: closed");
        }
    }

    bool StartListen()
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

private:
    void AsyncAccept()
    {
        auto service = std::make_shared<ServiceType>();
        service->Open(_io_service_pool.GetIoService());
        _acceptor.async_accept(service->socket(), MEM_FN(OnAccept, service, _1));
        service->set_status(ServiceType::STATUS_CONNECTING);
    }

    void OnAccept(std::shared_ptr<ServiceType> service, const asio::error_code& ec)
    {
        if (ec)
        {
            logger()->error("OnAccept: failed, reason: {}", ec.message());
            Close();
            return;
        }

        logger()->info("OnAccept: succeeded");
        service->SetSocketConnected();
        AsyncAccept();
    }

    IoServicePool& _io_service_pool;
    asio::ip::tcp::acceptor _acceptor;
    asio::ip::tcp::endpoint _endpoint;

    std::atomic_int _status;
};

} //namespace hpt


#endif //HEPTAPOD_RPC_LISTENER_H
