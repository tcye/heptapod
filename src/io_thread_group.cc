//
// Created by tiancai on 2017/3/9.
//

#include "io_thread_group.h"

namespace hpt {

IoThreadGroup::IoThreadGroup(std::size_t thread_num, const std::string& name)
        : _thread_num(thread_num),
          _name(name),
          _is_running(false),
          _io_service_work(nullptr)
{
    if (_name.empty())
    {
        char tmp[20];
        sprintf(tmp, "%p", this);
        _name = tmp;
    }
}

IoThreadGroup::~IoThreadGroup()
{
    Stop();
}

bool IoThreadGroup::Start()
{
    if (_is_running) return true;
    _is_running = true;

    _io_service_work = new IoServiceWork(_io_service);
    for (int i = 0; i < _thread_num; ++i)
    {
        auto thread = new std::thread(&SelfType::RunThread, this);
        _threads.push_back(thread);
    }
}

void IoThreadGroup::Stop()
{
    if (!_is_running) return;
    _is_running = false;

    delete _io_service_work;
    _io_service_work = nullptr;

    for (auto&& thread : _threads)
    {
        thread->join();
        delete thread;
    }
    _threads.clear();
}

void IoThreadGroup::RunThread()
{
    _io_service.run();
}

} //namespace hpt