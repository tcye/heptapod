//
// Created by tiancai on 2017/4/12.
//

#include <iostream>

class Base
{
    void f()
    {
        std::cout << "Base::f" << std::endl;
    }
};

class Dev : public Base
{
    int a;
};

int main(int argc, char** argv)
{
    Base b;
    Dev d;

    std::cout << sizeof(Base) << sizeof(Dev);



    return 0;
}
