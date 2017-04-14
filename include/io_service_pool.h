//
// Created by tiancai on 2017/3/10.
//

#pragma once
#ifndef HEPTAPOD_IO_SERVICE_POOL_H
#define HEPTAPOD_IO_SERVICE_POOL_H

#include <vector>
#include "common.h"

namespace hpt {

class IoThreadGroup;

class IoServicePool
{
    HPT_CLASS(IoServicePool)
public:
    IoServicePool(std::size_t pool_size, std::size_t pool_thread_num);
    ~IoServicePool();

    bool Run();
    void Stop();

    IoService& GetIoService();

private:
    std::vector<IoThreadGroup*> _pool;
    std::size_t _next_service;
};

}

#endif //HEPTAPOD_IO_SERVICE_POOL_H
