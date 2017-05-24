//
// Created by tiancai on 2017/4/12.
//

#include <iostream>
#include "rpc_connection.h"


class EchoService : public hpt::RpcConnection<EchoService>
{
public:
    static void InitServiceMap()
    {
        Bind("Echo", &EchoService::Echo);
    }

    void Echo(std::string s)
    {
        std::cout << s << std::endl;
    }
};

//class EchoService
//{
//public:
//    void Echo(std::string) {}
//};


//template <typename F>
//void PrintFuncTypes(F f)
//{
//    using hello = hpt::detail::FunctionTraits<F>::DecayedArgs;
//    std::cout << typeid(hello).name() << std::endl;
//}


int main(int argc, char** argv)
{
    EchoService::InitServiceMap();
    auto s = std::make_shared<EchoService>();

    auto pkg = std::make_tuple("Echo", std::make_tuple("hello world"));
    msgpack::zone z;
    msgpack::object obj(pkg, z);
    s->Dispatch(obj);
    int i;
    std::cin >> i;
    return 0;
}
