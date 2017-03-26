
#include <iostream>
#include <thread>

#include "type_alias.h"

class EchoClient
        : private hpt::noncopyable,
          public std::enable_shared_from_this<EchoClient>
{
public:
    using SelfType = EchoClient;

    EchoClient()
    {
        _service = std::make_shared<hpt::IoService>();
        _socket = std::make_shared<hpt::Socket>(*_service);
    }

    hpt::IoService& GetIoService() { return *_service; }

    void TryConnect(hpt::Endpoint endpoint)
    {
        _socket->async_connect(endpoint, MEM_FN(OnConnect, _1));
    }

    void OnConnect(const asio::error_code& ec)
    {
        if (ec)
        {
            std::cerr << "Connect Error: " << ec << std::endl;
            return;
        }
    }

    void Run()
    {
        _service->run();
    }

private:
    hpt::IoServicePtr _service;
    hpt::SocketPtr _socket;
};

using EchoClientPtr = std::shared_ptr<EchoClient>;

void RunClient(EchoClientPtr client)
{
    auto addr = hpt::Address::from_string("127.0.0.1");
    uint16_t port = 8001;
    hpt::Endpoint endpoint(addr, port);
    client->TryConnect(endpoint);
    client->Run();
}

int main(int argc, char* argv[])
{
    auto client = std::make_shared<EchoClient>();
    std::thread t(RunClient, client);
    t.join();
    return 0;
}
