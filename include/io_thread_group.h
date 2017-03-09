//
// Created by tiancai on 2017/3/9.
//

#ifndef HEPTAPOD_IO_THREAD_GROUP_H
#define HEPTAPOD_IO_THREAD_GROUP_H

#include <string>
#include <thread>
#include <vector>
#include "type_alias.h"

class std::thread;

namespace hpt {

class IoThreadGroup : noncopyable
{
public:
    using SelfType = IoThreadGroup;
public:
    IoThreadGroup(int thread_num, const std::string& name = "");
    ~IoThreadGroup();

    bool Start();
    void Stop();

    int thread_num() const { return _thread_num; }
    const std::string& name() const { return _name; }
    IoService& io_service() { return _io_service; }
    bool is_running() { return _is_running; }

private:
    void RunThread();

private:
    IoService _io_service;
    IoServiceWork* _io_service_work;
    std::string _name;
    int _thread_num;
    bool _is_running;
    std::vector<std::thread> _threads;
};

} //namespace hpt

#endif //HEPTAPOD_IO_THREAD_GROUP_H
