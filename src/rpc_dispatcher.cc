//
// Created by tiancai on 2017/5/9.
//

#include "rpc_dispatcher.h"

namespace hpt {

RpcDispatcher::RpcDispatcher()
{

}

RpcDispatcher::~RpcDispatcher()
{

}

void RpcDispatcher::Dispatch(const msgpack::object& object)
{
    Package pkg;
    object.convert(pkg);

    auto&& name = std::get<0>(pkg);
    auto&& args = std::get<1>(pkg);

    auto result = _adaptors.find(name);
    if (result != _adaptors.end())
    {
        result->second(args);
    }
}

} // namespace hpt
