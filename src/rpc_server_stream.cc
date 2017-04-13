//
// Created by tiancai on 2017/4/13.
//

#include "rpc_server_stream.h"

namespace hpt {

RpcServerStream::RpcServerStream(IoService& io_service) : _socket(io_service)
{

}

RpcServerStream::~RpcServerStream()
{

}

Socket& RpcServerStream::socket()
{
    return _socket;
}


}
