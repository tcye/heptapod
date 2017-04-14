
#include "rpc_server.h"


int main(int argc, char* argv[])
{
    auto address = hpt::Address::from_string("127.0.0.1");
    uint16_t port = 8001;
    hpt::Endpoint endpoint(address, port);

    auto server = std::make_shared<hpt::RpcServer>();
    server->Start(endpoint);
    server->Run();
    server->Stop();

    return 0;
}