//
// Created by tiancai on 2017/4/12.
//

#include <iostream>
#include "rpc_dispatcher.h"

void hello(std::string test)
{

}

int main(int argc, char** argv)
{
    hpt::RpcDispatcher d;

//    d.Bind("hello", [](std::string text){ });
    d.Bind("hello", hello);

    return 0;
}
