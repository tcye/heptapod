//
// Created by tiancai on 2017/5/3.
//

#ifndef HEPTAPOD_RPC_CLIENT_STREAM_H
#define HEPTAPOD_RPC_CLIENT_STREAM_H

#include "common.h"

namespace hpt {

class RpcClientStream : std::enable_shared_from_this<RpcClientStream>
{
    HPT_CLASS(RpcClientStream)
public:
    RpcClientStream();
    ~RpcClientStream();

};

} // namespace hpt

#endif //HEPTAPOD_RPC_CLIENT_STREAM_H
