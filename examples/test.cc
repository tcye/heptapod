//
// Created by tiancai on 2017/4/12.
//

#include "fmt/format.h"
#include <iostream>
#include "rpc_server.h"
#include "cxxabi.h"

int main(int argc, char** argv)
{
    int i;
    std::cout << abi::__cxa_demangle(typeid(hpt::RpcServer).name(), NULL, NULL, &i) << std::endl;
    std::cout << typeid(hpt::RpcServer).name() << std::endl;

    return 0;
}
