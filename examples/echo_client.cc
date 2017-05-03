
#include "common.h"
#include "msgpack.hpp"
#include <iostream>


void RunIoService(asio::io_service& io_service)
{
    io_service.run();
}

int main(int argc, char* argv[])
{
    asio::io_service io_service;
    asio::io_service::work* work = new asio::io_service::work(io_service);
    std::thread t(&RunIoService, std::ref(io_service));

    asio::ip::tcp::socket sock(io_service);

    auto addr = hpt::Address::from_string("127.0.0.1");
    uint16_t port = 8001;
    hpt::Endpoint endpoint(addr, port);

    sock.connect(endpoint);

    std::string s;
    while (std::cin >> s)
    {
        msgpack::sbuffer buf;
        msgpack::pack(buf, s);
        sock.write_some(asio::buffer(buf.data(), buf.size()));
    }

    delete work;
    t.join();
    return 0;
}
