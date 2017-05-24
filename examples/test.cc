//
// Created by tiancai on 2017/4/12.
//

#include <iostream>
#include "rpc_connection.h"


class EchoService : public hpt::RpcConnection<EchoService>
{
public:
    using hpt::RpcConnection<EchoService>::RpcConnection;
    static void InitServiceMap()
    {
        Bind("Echo", &EchoService::Echo);
    }

    void Echo(std::string s)
    {
        CallRemote("Echo", s);
        std::cout << s << std::endl;
    }
};

//template <typename F>
//void PrintFuncTypes(F f)
//{
//    using hello = hpt::detail::FunctionTraits<F>::DecayedArgs;
//    std::cout << typeid(hello).name() << std::endl;
//}


int main(int argc, char** argv)
{
    EchoService::InitServiceMap();
    asio::io_service io_service;
    auto s = std::make_shared<EchoService>();
    s->CallRemote("Echo", "world");

    auto pkg = std::make_tuple("Echo", std::make_tuple("hello world"));
    msgpack::zone z;
    msgpack::object obj(pkg, z);
    s->Dispatch(obj);
    int i;
    std::cin >> i;
    return 0;
}
