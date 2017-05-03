//
// Created by tiancai on 2017/3/8.
//

#pragma once
#ifndef HEPTAPOD_COMMON_H
#define HEPTAPOD_COMMON_H

#include "asio.hpp"
#include "spdlog/spdlog.h"

#define HPT_DISABLE_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete; \
    void operator=(const TypeName&) = delete;

#define HPT_ENABLE_CLS_LEVEL_LOGGER(TypeName) \
    static std::shared_ptr<spdlog::logger>& logger() \
    { \
        static std::shared_ptr<spdlog::logger> _logger = spdlog::stdout_color_mt(#TypeName); \
        return _logger; \
    }

#define HPT_CLASS(TypeName) \
    HPT_DISABLE_COPY_AND_ASSIGN(TypeName) \
    HPT_ENABLE_CLS_LEVEL_LOGGER(TypeName) \
    using SelfType = TypeName;

using namespace std::placeholders;
#define MEM_FN(func, ...) std::bind(&SelfType::func, std::dynamic_pointer_cast<SelfType>(shared_from_this()), ##__VA_ARGS__)
#define MEM_FN_UNSAFE(func, ...) std::bind(&SelfType::func, this, ##__VA_ARGS__)

#define HPT_ALIAS(x, y) \
using y = x; \
using y##Ptr = std::shared_ptr<x>; \
using y##WPtr = std::weak_ptr<x>;

#define HPT_PTR_ALIAS(x) \
class x; \
using x##Ptr = std::shared_ptr<x>; \
using x##WPtr = std::weak_ptr<x>;

namespace hpt {

HPT_ALIAS(asio::io_service, IoService)
HPT_ALIAS(asio::io_service::work, IoServiceWork)
HPT_ALIAS(asio::ip::tcp::endpoint, Endpoint)
HPT_ALIAS(asio::ip::tcp::socket, Socket)
HPT_ALIAS(asio::ip::tcp::acceptor, Acceptor)
HPT_ALIAS(asio::ip::address, Address)

HPT_PTR_ALIAS(RpcServerStream)
HPT_PTR_ALIAS(RpcListener)
HPT_PTR_ALIAS(IoServicePool)

} // namespace hpt


namespace hpt {

inline Endpoint MakeEndpoint(const std::string& address_str, uint16_t port)
{
    Address address = Address::from_string(address_str);
    return Endpoint(address, port);
}

} // namespace hpt


#endif //HEPTAPOD_COMMON_H
