#include <iostream>
#include <tuple>
#include <cmath>
#include <memory>

namespace kyadet {
/*
 * Forward型自動微分における中間変数を抽象するclass
 */
template<class Derived>
struct Parameter {
    /* Concept
     * メンバ変数はすべてParameterを継承した型（Inputは例外？)
     * 型が演算を、メンバ変数（1つないし2つ）が引数を表す
     */
    /*
     * 計算グラフが閉じた構造を持っている場合の対策
     * メンバ変数をポインタにする
     * y.diff(x)は\frac{\partial y}{\partial x}
     */
};


template<class T>
class Input : Parameter<Input<T>> {
    //右辺値参照とかそのへんの考慮が必要そう？
    T t_;
public:
    explicit Input(T t)
        : t_(t) {}

    auto value() -> T {
        std::cout << "this: " << this << std::endl;
        return t_;
    }

    template<class U>
    auto diff(const U &p) {
        std::cout << this << " " << &p << std::endl;
        return this == &p ? T(1) : T(0);
    }

};


// Parameterを継承していなければInputをかます
template<class T>
auto toInput(T arg) -> std::enable_if_t<std::is_base_of_v<Parameter<T>, T>, T> {
    return arg;
};

template<class T>
auto toInput(T arg) -> std::enable_if_t<!std::is_base_of_v<Parameter<T>, T>, Input<T>> {
    return Input<T>(arg);
};


template<class L, class R>
class Add : public Parameter<Add<L, R>> {
    L l_;
    R r_;
public:
    explicit Add(L l, R r)
        : l_(l), r_(r) {}

    auto value() {
        return l_.value() + r_.value();
    }

    template<class U>
    auto diff(const U &p) {
        return l_.diff(p) + r_.diff(p);
    }

};

template<class L, class R>
auto operator+(L l, R r) {
    return Add<decltype(toInput(l)), decltype(toInput(r))>(toInput(l), toInput(r));
};

template<class L, class R>
class Mult : public Parameter<Mult<L, R>> {
    L l_;
    R r_;
public:
    explicit Mult(L l, R r)
        : l_(l), r_(r) {}

    auto value() {
        return l_.value() * r_.value();
    }

    template<class U>
    auto diff(const U &p) {
        return l_.diff(p) * r_.value() + l_.value() * r_.diff(p);
    }

};

template<class L, class R>
auto operator*(L l, R r) {
    return Mult<decltype(toInput(l)), decltype(toInput(r))>(toInput(l), toInput(r));
};

/*
template<class X>
class Sin : public Parameter {
    X x_;
public:
    explicit Sin(X x)
        : x_(x) {}

    auto value() {
        return std::sin(x_.value());
    }
};

template<class X>
auto sin(X x) {
    return Sin<decltype(toInput(x))>(x);
}
 */

}//namespace kyadet


int main() {
    using namespace kyadet;

    Input<int> x1(1);
    Input<int> x2(2);

    std::cout << x1.value() << std::endl;
    std::cout << x2.value() << std::endl;
    std::cout << (x1 + x2).value() << std::endl;
    /*
    auto y = x1 * x2 + x1 + 4;
    std::cout << y.value() << std::endl; //1*2+1+4=7
    std::cout << y.diff(x1) << std::endl; //x2+1=3
     */

    return 0;
}
