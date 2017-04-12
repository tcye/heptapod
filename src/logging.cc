//
// Created by tiancai on 2017/4/12.
//

#include "logging.h"
#include "spdlog/spdlog.h"

namespace hpt {

static spdlog::logger _logger = nullptr;

void InitHptLogging()
{
    _logger = spdlog::stdout_color_mt("console");
}

void ShutdownHptLogging()
{
    spdlog::drop_all();
}

spdlog::logger* GetLogger()
{
    return _logger;
}


}

