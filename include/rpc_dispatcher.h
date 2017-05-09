//
// Created by tiancai on 2017/5/9.
//

#ifndef HEPTAPOD_RPC_DISPATCHER_H
#define HEPTAPOD_RPC_DISPATCHER_H

#include "common.h"
#include "msgpack.hpp"
#include "detail/function_traits.h"
#include "detail/call_helper.h"

namespace hpt {

class RpcDispatcher
{
public:
    using Package = std::tuple<std::string, msgpack::object>;
    using Adaptor = std::function<void(const msgpack::object&)>;

    RpcDispatcher();
    ~RpcDispatcher();

    template <typename F> void Bind(const std::string& name, F func);
    void Dispatch(const msgpack::object& object);

private:
    std::unordered_map<std::string, Adaptor> _adaptors;
};


template <typename F>
void RpcDispatcher::Bind(const std::string& name, F func)
{
    Adaptor adaptor = [func](const msgpack::object& object) {
        typename detail::FunctionTraits<F>::DecayedArgs args;
        object.convert(args);
        detail::Call(func, args);
    };
    _adaptors.insert({name, adaptor});
}


} // namespace hpt

#endif //HEPTAPOD_RPC_DISPATCHER_H
