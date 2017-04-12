//
// Created by tiancai on 2017/4/12.
//

#ifndef HEPTAPOD_LOGGING_H
#define HEPTAPOD_LOGGING_H

#include "spdlog/spdlog.h"

namespace hpt {

void InitHptLogging();
void ShutdownHptLogging();

spdlog::logger* GetLogger();

} // namespace hpt

#define LOG (hpt::GetLogger())

#endif //HEPTAPOD_LOGGING_H
