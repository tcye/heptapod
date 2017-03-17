//
// Created by tiancai on 2017/3/8.
//

#ifndef HEPTAPOD_COMMON_H
#define HEPTAPOD_COMMON_H

#include <cstddef>
#include <memory>
#include <functional>

#define HPT_ALIAS(x, y) \
using y = x; \
using y##Ptr = std::shared_ptr<x>; \
using y##WPtr = std::weak_ptr<x>;

using namespace std::placeholders;
#define MEM_FN(func, ...) std::bind(&SelfType::func, shared_from_this(), ##__VA_ARGS__)
#define MEM_FN_UNSAFE(func, ...) std::bind(&SelfType::func, this, ##__VA_ARGS__)

#endif //HEPTAPOD_COMMON_H
