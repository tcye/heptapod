//
// Created by tiancai on 2017/3/10.
//

#include "fmt/format.h"
#include "io_service_pool.h"
#include "io_thread_group.h"

namespace hpt {

IoServicePool::IoServicePool(std::size_t pool_size, std::size_t pool_thread_num)
        : _next_service(0)
{
    for (std::size_t i = 0; i < pool_size; ++i)
    {
        auto name = fmt::format("io_service worker thread group {}", i);
        auto io_thread_group = new IoThreadGroup(pool_thread_num, name);
        _pool.push_back(io_thread_group);
    }
}

IoServicePool::~IoServicePool()
{
    for (auto&& io_thread_group : _pool)
    {
        delete io_thread_group;
    }
}

bool IoServicePool::Run()
{
    for (auto&& io_thread_group : _pool)
    {
        if (!io_thread_group->Start())
            return false;
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
        _next_service = 0;

    return _pool[_next_service++]->io_service();
}

}

