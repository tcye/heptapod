//
// Created by tiancai on 2017/5/3.
//

#ifndef HEPTAPOD_RPC_CLIENT_STREAM_H
#define HEPTAPOD_RPC_CLIENT_STREAM_H

#include "rpc_stream.h"

namespace hpt {

class RpcClientStream : public RpcStream
{
    HPT_CLASS(RpcClientStream)
public:
    using RpcStream::RpcStream;
    ~RpcClientStream();
};

} // namespace hpt

#endif //HEPTAPOD_RPC_CLIENT_STREAM_H
