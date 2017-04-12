//
// Created by tiancai on 2017/4/12.
//

#include "fmt/format.h"

int main(int argc, char** argv)
{
    int* p = new int;
    fmt::print("{}hello world{}", 1, (void*)p);
    delete p;
    return 0;
}
