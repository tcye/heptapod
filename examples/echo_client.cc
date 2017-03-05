
#include <asio.hpp>
#include <memory>
#include <iostream>
#include <functional>
#include <thread>

class EchoClient
    : private asio::noncopyable,
      public std::enable_shared_from_this<EchoClient> {
 public:
  using SelfType = EchoClient;

  EchoClient(asio::io_service& service) : service_(service), socket_(service) {}

  asio::io_service& GetIoService() { return service_; }

  void TryConnect(asio::ip::tcp::endpoint endpoint) {
    socket_.async_connect(endpoint, std::bind(&SelfType::OnConnect, shared_from_this(), std::placeholders::_1));
  }

  void OnConnect(const asio::error_code& ec) {
    if (ec)
      std::cerr << "Connect Error: " << ec << std::endl;
      return;


  }

 private:
  asio::io_service& service_;
  asio::ip::tcp::socket socket_;

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
  auto client = std::make_shared<EchoClient>(service);
  std::thread t(RunClient, client);
  t.join();
  return 0;
}
