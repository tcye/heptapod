//
// Created by tiancai on 2017/3/8.
//

#ifndef HEPTAPOD_COMMON_H
#define HEPTAPOD_COMMON_H

#include <cstddef>
#include <memory>
#include <functional>
#include "asio.hpp"

#define HPT_ALIAS(x, y) \
using y = x; \
using y##Ptr = std::shared_ptr<x>; \
using y##WPtr = std::weak_ptr<x>;

#define HPT_PTR_ALIAS(x) \
class x; \
using x##Ptr = std::shared_ptr<x>; \
using x##WPtr = std::weak_ptr<x>;

using namespace std::placeholders;
#define MEM_FN(func, ...) std::bind(&SelfType::func, shared_from_this(), ##__VA_ARGS__)
#define MEM_FN_UNSAFE(func, ...) std::bind(&SelfType::func, this, ##__VA_ARGS__)

namespace hpt {

using noncopyable = asio::noncopyable;
HPT_ALIAS(asio::io_service, IoService)
HPT_ALIAS(asio::io_service::work, IoServiceWork)
HPT_ALIAS(asio::ip::tcp::endpoint, Endpoint)
HPT_ALIAS(asio::ip::tcp::socket, Socket)
HPT_ALIAS(asio::ip::tcp::acceptor, Acceptor)
HPT_ALIAS(asio::ip::address, Address)

HPT_PTR_ALIAS(RpcServerStream)

} // namespace hpt


#endif //HEPTAPOD_COMMON_H
