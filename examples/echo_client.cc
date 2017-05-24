
#include <iostream>
#include "rpc_connection.h"
#include "rpc_client.h"


class EchoService : public hpt::RpcConnection<EchoService>
{
public:
    static void InitServiceMap()
    {
        Bind("Echo", &EchoService::Echo);
    }

    void OnConnected() override
    {
        CallRemote("Echo", "Client connected");
    }
    
    void Echo(const std::string& s)
    {
        std::cout << s << std::endl;
        CallRemote("Echo", "Hello from client");
    }
};


int main(int argc, char* argv[])
{
    hpt::RpcClient<EchoService> client;
    client.Connect("127.0.0.1", 8001);
    client.WaitSignal();
    client.Shutdown();
    return 0;
}
