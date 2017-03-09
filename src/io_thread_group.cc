//
// Created by tiancai on 2017/3/9.
//

#include "io_thread_group.h"

namespace hpt {

IoThreadGroup::IoThreadGroup(int thread_num, const std::string& name)
        : _thread_num(thread_num), _name(name), _is_running(false)
{
    if (_name.empty())
    {
        char tmp[20];
        sprintf(tmp, '%p', this);
        _name = tmp;
    }
}

IoThreadGroup::~IoThreadGroup()
{
    Stop();
}

bool IoThreadGroup::Start()
{
    if (_is_running)
    {
        return true;
    }
}

void IoThreadGroup::Stop()
{
    if (!_is_running)
    {
        return;
    }
}

} //namespace hpt