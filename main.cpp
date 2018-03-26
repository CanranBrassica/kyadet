#include "kyadet.hpp"
#include <iostream>

int main() {
    using namespace kyadet;
    auto x = std::make_shared<Input<double>>(1);
    auto y = std::make_shared<Input<double>>(1);
    auto z = exp(x + y);
    std::cout << z->diffGraph(x)->value() << std::endl;
    //auto d = z->diff(z);
    //std::cout << d[y] << std::endl;
    return 0;
}
