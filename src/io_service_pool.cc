//
// Created by tiancai on 2017/3/10.
//

#include <iostream>
#include "io_service_pool.h"

namespace hpt {

IoServicePool::IoServicePool(size_t pool_size, size_t pool_thread_num)
        : _next_service(0)
{
    for (size_t i = 0; i < pool_size; ++i)
    {
        char tmp[50];
        sprintf(tmp, "io_service worker thread group %d", i);
        _pool.emplace_back(pool_thread_num, tmp);
    }
}

bool IoServicePool::Run()
{
    auto pool_size = _pool.size();
    for (size_t i = 0; i < pool_size; ++i)
    {
        if (!_pool[i].Start())
        {
            std::cerr << "IoServicePool Run Failed!!!" << std::endl;
            return false;
        }
    }
    return true;
}

void IoServicePool::Stop()
{
    auto pool_size = _pool.size();
    for (size_t i = 0; i < pool_size; ++i)
    {
        _pool[i].Stop();
    }
    _pool.clear();
    _next_service = 0;
}


IoService& IoServicePool::GetIoService()
{
    if (_next_service >= _pool.size())
    {
        _next_service = 0;
    }
    IoService& service = _pool[_next_service].io_service();
    ++_next_service;
    return service;
}


}

