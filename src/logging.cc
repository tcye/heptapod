//
// Created by tiancai on 2017/4/12.
//

#include "logging.h"

namespace hpt {

static spdlog::logger* _logger = nullptr;

void InitHptLogging()
{
    _logger = spdlog::stdout_color_mt("console").get();
}

void ShutdownHptLogging()
{
    _logger = nullptr;
    spdlog::drop_all();
}

spdlog::logger* GetLogger()
{
    return _logger;
}


}

