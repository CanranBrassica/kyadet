#include "kyadet.hpp"
#include <iostream>

int main()
{
    using namespace kyadet;
    auto x = std::make_shared<Input<double>>(1);
    auto y = std::make_shared<Input<double>>(1);
    auto z = x + y / (x * y + 2.0);
    std::cout << z->diff(x)->diff(y)->value() << std::endl;
    return 0;
}
