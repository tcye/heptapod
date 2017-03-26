
#include <iostream>
#include <thread>

#include "type_alias.h"

class EchoServer;

class TcpConnection
        : private hpt::noncopyable,
          public std::enable_shared_from_this<TcpConnection>
{
public:
    using SelfType = TcpConnection;

    TcpConnection(hpt::IoService& service) : _sock(service) {}

    hpt::Socket& GetSocket() { return _sock; }

    void OnAccept()
    {
        std::cout << "Connection Established! " << std::endl;
    }

private:
    hpt::Socket _sock;
};

class EchoServer
        : private asio::noncopyable,
          public std::enable_shared_from_this<EchoServer>
{
public:
    using SelfType = EchoServer;

    EchoServer()
    {
        _service = std::make_shared<hpt::IoService>();
        _acceptor = std::make_shared<hpt::Acceptor>(*_service);
    }

    void StartAccept(hpt::Endpoint endpoint)
    {
        _acceptor->open(endpoint.protocol());
        _acceptor->bind(endpoint);
        _acceptor->listen();
        AcceptOne();
    }

    void AcceptOne()
    {
        auto conn = std::make_shared<TcpConnection>(*this->_service);
        _acceptor->async_accept(conn->GetSocket(),
                                MEM_FN(HandleAccept, conn, _1));
    }

    void HandleAccept(std::shared_ptr<TcpConnection> connection,
                      const asio::error_code& ec)
    {
        if (ec)
        {
            std::cerr << "Accept Error, " << ec << std::endl;
            return;
        }
        connection->OnAccept();
        AcceptOne();
    }

    void Run()
    {
        hpt::Endpoint endpoint(hpt::Address::from_string("127.0.0.1"), 8001);
        StartAccept(endpoint);
        _service->run();
    }

    hpt::IoService& GetIoService() { return *_service; }

private:
    hpt::IoServicePtr _service;
    hpt::AcceptorPtr _acceptor;
};

int main(int argc, char* argv[])
{
    auto server = std::make_shared<EchoServer>();
    server->Run();
    return 0;
}