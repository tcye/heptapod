//
// Created by tiancai on 2017/5/24.
//

#ifndef HEPTAPOD_RPC_CONNECTION_H
#define HEPTAPOD_RPC_CONNECTION_H

#include "common.h"
#include "msgpack.hpp"
#include "detail/call_helper.h"

#include <functional>
#include <unordered_map>
#include <memory>

namespace hpt
{
    template <typename ServiceType>
    class RpcConnection : public std::enable_shared_from_this<RpcConnection<ServiceType>>
    {
        HPT_CLASS(RpcConnection)
    public:
        enum
        {
            STATUS_INIT = 0,
            STATUS_CONNECTING = 1,
            STATUS_CONNECTED = 2,
            STATUS_CLOSED = 3,
        };

        explicit RpcConnection(asio::io_service& io_service)
            : _socket(io_service), _write_strand(io_service), _status(STATUS_INIT), _bytes_written(0)
        {
        }

    protected:
        using PackageType = std::tuple<std::string, msgpack::object>;
        using AdaptorType = std::function<void(ServiceType*, const msgpack::object&)>;

        static std::unordered_map<std::string, AdaptorType> _adaptors;

        template <typename Ret, typename... Args>
        static void Bind(const std::string& name, Ret (ServiceType::*func)(Args ...))
        {
            AdaptorType adaptor = [func](ServiceType* s, const msgpack::object& obj) -> void
            {
                std::tuple<std::decay_t<Args>...> args;
                obj.convert(args);
                detail::Call(func, s, args);
            };
            _adaptors.insert({name, adaptor});
        }

    public:
        void Dispatch(const msgpack::object& obj)
        {
            PackageType pkg;
            obj.convert(pkg);

            auto&& name = std::get<0>(pkg);
            auto&& args = std::get<1>(pkg);

            auto result = _adaptors.find(name);
            if (result != _adaptors.end())
            {
                result->second(static_cast<ServiceType*>(this), args);
            }
        }

        template <typename... Args>
        void CallRemote(const std::string& name, Args ... args)
        {
            auto pkg = std::make_tuple(name, std::make_tuple(args...));
            auto buf = std::make_shared<msgpack::sbuffer>();
            msgpack::pack(*buf, pkg);
            _write_strand.dispatch(MEM_FN(DoCallRemote, buf));
        }

    private:
        asio::ip::tcp::socket _socket;
        asio::strand _write_strand;
        std::atomic_int _status;
        msgpack::unpacker _unpacker;
        std::deque<msgpack::sbuffer> _write_queue;
        size_t _bytes_written;
    };

    template <typename ServiceType>
    std::unordered_map<std::string, typename RpcConnection<ServiceType>::AdaptorType>
    RpcConnection<ServiceType>::_adaptors;
} // namespace hpt
#endif //HEPTAPOD_RPC_CONNECTION_H
