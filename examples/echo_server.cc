
#include <asio.hpp>
#include <memory>
#include <iostream>
#include <functional>
#include <thread>

class TcpConnection : private asio::noncopyable, public std::enable_shared_from_this<TcpConnection> {
 public:
  using SelfType = TcpConnection;


};


class EchoServer : private asio::noncopyable, public std::enable_shared_from_this<EchoServer> {
 public:
  using SelfType = EchoServer;

  EchoServer(asio::io_service& service) : service_(service), acceptor_(service) {
    unsigned short port = 8001;
    asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);

    acceptor_.open(endpoint.protocol());
    acceptor_.bind(endpoint);
    acceptor_.listen();
  }

  void WaitConnect(asio::ip::tcp::endpoint endpoint) {
    acceptor_.async_accept()
  }

 private:
  asio::io_service& service_;
  asio::ip::tcp::acceptor acceptor_;
};


void RunClient(std::shared_ptr<EchoClient> client) {
  auto addr = asio::ip::address::from_string("127.0.0.1");
  unsigned short port = 8001;
  asio::ip::tcp::endpoint endpoint(addr, port);
  client->TryConnect(endpoint);
  client->GetIoService().run();

}


int main(int argc, char* argv[]) {
  asio::io_service service;

  auto server = std::make_shared<EchoServer>(service);

  service.run();
  return 0;
}