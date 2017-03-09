//
// Created by tiancai on 2017/3/7.
//

#ifndef HEPTAPOD_TYPE_ALIAS_H
#define HEPTAPOD_TYPE_ALIAS_H

#include <asio.hpp>
#include "common.h"

namespace hpt {

using noncopyable = asio::noncopyable;

HPT_ALIAS(asio::io_service, IoService)
HPT_ALIAS(asio::io_service::work, IoServiceWork)
HPT_ALIAS(asio::ip::tcp::endpoint, Endpoint)
HPT_ALIAS(asio::ip::tcp::socket, Socket)
HPT_ALIAS(asio::ip::tcp::acceptor, Acceptor)
HPT_ALIAS(asio::ip::address, Address)

} //namespace hpt

#endif //HEPTAPOD_TYPE_ALIAS_H
