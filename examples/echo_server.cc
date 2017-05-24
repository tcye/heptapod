
#include <iostream>
#include "rpc_connection.h"
#include "rpc_server.h"


class EchoService : public hpt::RpcConnection<EchoService>
{
public:
    static void InitServiceMap()
    {
        Bind("Echo", &EchoService::Echo);
    }

    void OnConnected() override
    {
    }
    
    void Echo(const std::string& s)
    {
        std::cout << s << std::endl;
        CallRemote("Echo", "Hello from server");
    }
};

int main(int argc, char* argv[])
{
    hpt::RpcServer<EchoService> server;
    server.Start("127.0.0.1", 8001);
    server.WaitSignal();
    server.Stop();
    return 0;
}