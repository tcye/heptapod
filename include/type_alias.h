//
// Created by tiancai on 2017/3/7.
//

#ifndef HEPTAPOD_TYPE_ALIAS_H
#define HEPTAPOD_TYPE_ALIAS_H

#include <memory>
#include <functional>
#include <asio.hpp>

#define SMART_ALIAS(x, y) \
using y = x; \
using y##Ptr = std::shared_ptr<x>; \
using y##WPtr = std::weak_ptr<x>;


using namespace std::placeholders;
#define MEM_FN(func, ...) std::bind(&SelfType::func, shared_from_this(), ##__VA_ARGS__)

namespace hpt {

using noncopyable = asio::noncopyable;

SMART_ALIAS(asio::io_service, IOService)
SMART_ALIAS(asio::ip::tcp::endpoint, EndPoint)
SMART_ALIAS(asio::ip::tcp::socket, Socket)
SMART_ALIAS(asio::ip::address, Address)
SMART_ALIAS(asio::ip::tcp::acceptor, Acceptor)

}

#endif //HEPTAPOD_TYPE_ALIAS_H
