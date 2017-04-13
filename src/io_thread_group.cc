//
// Created by tiancai on 2017/3/9.
//

#include "fmt/format.h"
#include "spdlog/spdlog.h"
#include "io_thread_group.h"
#include "logging.h"

namespace hpt {

IoThreadGroup::IoThreadGroup(std::size_t thread_num, const std::string& name)
        : _thread_num(thread_num),
          _name(name),
          _is_running(false),
          _io_service_work(nullptr)
{
    if (_name.empty())
        _name = fmt::format("{:p}", static_cast<void*>(this));

    LOG->info("IoThreadGroup({}) constructed.", _name);
}

IoThreadGroup::~IoThreadGroup()
{
    Stop();
    LOG->info("IoThreadGroup({}) destroyed.", _name);
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
    LOG->info("IoThreadGroup({}) DoStart.", _name);
    _io_service_work = new IoServiceWork(_io_service);
    for (int i = 0; i < _thread_num; ++i)
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
    LOG->info("IoThreadGroup({}) DoStop.", _name);
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
    LOG->info("IoThreadGroup({}) io_service run.", _name);
    _io_service.run();
}

} //namespace hpt