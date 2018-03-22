#include "kyadet.hpp"
#include <iostream>

int main()
{
    using namespace kyadet;
    auto x = std::make_shared<Input<double>>(1);
    auto y = std::make_shared<Input<double>>(2);
    auto z = x / y;
    std::cout << z->diff(x)->value() << std::endl;
    std::cout << z->diff(x)->diff(y)->value() << std::endl;
    return 0;
}
