//
// Created by tiancai on 2017/4/20.
//

#include <thread>
#include <csignal>
#include "wait_signal_func.h"

using namespace std::chrono_literals;

namespace hpt {

static volatile bool s_quit = false;
static void SignalHandler(int /*sig*/)
{
    s_quit = true;
}

void WaitSignalFunc::WaitSignal()
{
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    while (!s_quit)
    {
        std::this_thread::sleep_for(1s);
    }
}

}
