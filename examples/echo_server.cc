
#include "io_service_pool.h"
#include "rpc_server.h"
#include <iostream>


int main(int argc, char* argv[])
{
    hpt::IoServicePool io_service_pool(4, 2);
    hpt::RpcServer server(io_service_pool);

    server.Bind("echo", [](const std::string& s){ std::cout << "this is echo:" << s << std::endl; });

    io_service_pool.Run();

    server.Start("127.0.0.1", 8001);
    server.WaitSignal();
    server.Stop();

    io_service_pool.Stop();

    return 0;
}