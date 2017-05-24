//
// Created by tiancai on 2017/3/10.
//

#ifndef HEPTAPOD_IO_SERVICE_POOL_H
#define HEPTAPOD_IO_SERVICE_POOL_H

#include "common.h"

namespace hpt {

class IoThreadGroup;

class IoServicePool
{
    HPT_CLASS(IoServicePool)
public:
    explicit IoServicePool(std::size_t pool_size = 2, std::size_t pool_thread_num = 2);
    ~IoServicePool();

    bool Run();
    void Stop();

    asio::io_service& GetIoService();

private:
    std::vector<IoThreadGroup*> _pool;
    size_t _next_service;
};

}

#endif //HEPTAPOD_IO_SERVICE_POOL_H
