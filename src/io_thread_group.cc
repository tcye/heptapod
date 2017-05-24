//
// Created by tiancai on 2017/3/9.
//

#include "fmt/format.h"
#include "io_thread_group.h"

namespace hpt {

IoThreadGroup::IoThreadGroup(std::size_t thread_num, const std::string& name)
    : _io_service_work(nullptr),
      _thread_num(thread_num),
      _name(name),
      _is_running(false)
{
    if (_name.empty())
        _name = fmt::format("{:p}", static_cast<void*>(this));
}

IoThreadGroup::~IoThreadGroup()
{
    Stop();
}

bool IoThreadGroup::Start()
{
    if (_is_running)
        return true;

    if (!DoStart())
        return false;

    _is_running = true;
    return true;
}

bool IoThreadGroup::DoStart()
{
    _io_service_work = new asio::io_service::work(_io_service);
    for (size_t i = 0; i < _thread_num; ++i)
    {
        auto thread = new std::thread(&SelfType::RunThread, this);
        _threads.push_back(thread);
    }
    return true;
}

void IoThreadGroup::Stop()
{
    if (!_is_running)
        return;

    DoStop();
    _is_running = false;
}

void IoThreadGroup::DoStop()
{
    delete _io_service_work;
    _io_service_work = nullptr;
    _io_service.stop();

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
