//
// Created by tiancai on 2017/3/9.
//

#ifndef HEPTAPOD_IO_THREAD_GROUP_H
#define HEPTAPOD_IO_THREAD_GROUP_H

#include "common.h"

namespace hpt {

class IoThreadGroup
{
    HPT_CLASS(IoThreadGroup)
public:
    IoThreadGroup(std::size_t thread_num, const std::string& name = "");
    ~IoThreadGroup();

    bool Start();
    void Stop();

    int thread_num() const { return _thread_num; }
    const std::string& name() const { return _name; }
    asio::io_service& io_service() { return _io_service; }
    bool is_running() { return _is_running; }

private:
    bool DoStart();
    void DoStop();
    void RunThread();

    asio::io_service _io_service;
    asio::io_service::work* _io_service_work;
    std::vector<std::thread*> _threads;

    std::string _name;
    std::size_t _thread_num;
    bool _is_running;
};

} //namespace hpt

#endif //HEPTAPOD_IO_THREAD_GROUP_H
