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
        Stop();

    if (!DoStart())
        return false;

    _is_running = true;
    return true;
}

bool IoThreadGroup::DoStart()
{
    _io_service_work = new IoServiceWork(_io_service);
    for (int i = 0; i < _thread_num; ++i)
    {
        auto thread = new std::thread(&SelfType::RunThread, this);
        _threads.push_back(thread);
    }

    LOG->info("IoThreadGroup({}) DoStart.", _name);
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

    for (auto&& thread : _threads)
    {
        thread->join();
        delete thread;
    }
    _threads.clear();
    LOG->info("IoThreadGroup({}) DoStop.", _name);
}

void IoThreadGroup::RunThread()
{
    _io_service.run();
    LOG->info("IoThreadGroup({}) io_service run.", _name);
}

} //namespace hpt