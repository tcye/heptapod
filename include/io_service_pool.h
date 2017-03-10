//
// Created by tiancai on 2017/3/10.
//

#ifndef HEPTAPOD_IO_SERVICE_POOL_H
#define HEPTAPOD_IO_SERVICE_POOL_H

#include <vector>
#include "type_alias.h"
#include "io_thread_group.h"

namespace hpt {

class IoServicePool : private noncopyable
{
public:
    IoServicePool(size_t pool_size, size_t pool_thread_num);
    ~IoServicePool();

    bool Run();
    void Stop();

    IoService& GetIoService();

private:
    size_t _next_service;
    std::vector<IoThreadGroup> _pool;
};

}

#endif //HEPTAPOD_IO_SERVICE_POOL_H
