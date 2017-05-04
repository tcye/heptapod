//
// Created by tiancai on 2017/4/13.
//

#ifndef HEPTAPOD_RPC_SERVER_STREAM_H
#define HEPTAPOD_RPC_SERVER_STREAM_H

#include "rpc_stream.h"

namespace hpt {

class RpcServerStream : public RpcStream
{
    HPT_CLASS(RpcServerStream)
public:
    using RpcStream::RpcStream;
    ~RpcServerStream();
};

}

#endif // HEPTAPOD_RPC_SERVER_STREAM_H
