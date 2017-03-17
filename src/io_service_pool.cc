//
// Created by tiancai on 2017/3/10.
//

#include <iostream>
#include "io_service_pool.h"

namespace hpt {

IoServicePool::IoServicePool(std::size_t pool_size, std::size_t pool_thread_num)
        : _next_service(0)
{
    for (std::size_t i = 0; i < pool_size; ++i)
    {
        char tmp[50];
        sprintf(tmp, "io_service worker thread group %d", i);
        auto io_thread_group = new IoThreadGroup(pool_thread_num, tmp);
        _pool.push_back(io_thread_group);
    }
}

IoServicePool::~IoServicePool()
{
    Stop();

    for (auto&& io_thread_group : _pool)
    {
        delete io_thread_group;
    }
    _pool.clear();
}

bool IoServicePool::Run()
{
    for (auto&& io_thread_group : _pool)
    {
        if (!io_thread_group->Start())
        {
            return false;
        }
    }

    return true;
}

void IoServicePool::Stop()
{
    for (auto&& io_thread_group: _pool)
    {
        io_thread_group->Stop();
    }
}

IoService& IoServicePool::GetIoService()
{
    if (_next_service >= _pool.size())
    {
        _next_service = 0;
    }
    IoService& service = _pool[_next_service]->io_service();
    ++_next_service;
    return service;
}

}

