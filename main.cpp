#include <iostream>
#include <memory>

namespace kyadet
{
template <class Derived>
struct Parameter {
};

template <class T>
class Input : public Parameter<Input<T>>
{
    std::shared_ptr<T> t_ = nullptr;

public:
    explicit Input(T t)
        : t_(std::make_shared<T>(t)) {}

    T value() const
    {
        return *t_;
    }

    T diff(const std::shared_ptr<void>& p) const
    {
        return this == p.get() ? T(1) : T(0);
    }
};  //class Input

template <class L, class R>
class Add : public Parameter<Add<L, R>>
{
    std::shared_ptr<L> l_ = nullptr;
    std::shared_ptr<R> r_ = nullptr;

public:
    explicit Add(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
        : l_(l), r_(r)
    {
        std::cout << "どっちもshared_ptrなAdd constructer" << std::endl;
    }

    explicit Add(L&& l, const std::shared_ptr<R>& r)
        : l_(std::make_shared<L>(l)), r_(r)
    {
        std::cout << "L&&なAdd constructer" << std::endl;
    }

    explicit Add(const std::shared_ptr<L>& l, R&& r)
        : l_(l), r_(std::make_shared<R>(r))
    {
        std::cout << "R&&なAdd constructer" << std::endl;
    }

    auto value() const
    {
        return l_->value() + r_->value();
    }

    auto diff(const std::shared_ptr<void>& p) const
    {
        return l_->diff(p) + r_->diff(p);
    }

};  //class Add

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter<L>, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter<R>, R>, std::nullptr_t> = nullptr>
auto operator+(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
{
    std::cout << "operator+ & &" << std::endl;
    return std::make_shared<Add<L, R>>(l, r);
}

template <class L, class R,
    std::enable_if_t<std::is_arithmetic_v<L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter<R>, R>, std::nullptr_t> = nullptr>
auto operator+(L&& l, const std::shared_ptr<R>& r) -> std::shared_ptr<Add<Input<L>, R>>
{
    std::cout << "operator+ && &" << std::endl;
    return std::make_shared<Add<Input<L>, R>>(Input<L>{l}, r);
}

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter<L>, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_arithmetic_v<R>, std::nullptr_t> = nullptr>
auto operator+(const std::shared_ptr<L>& l, R&& r) -> std::shared_ptr<Add<L, Input<R>>>
{
    std::cout << "operator+ && &" << std::endl;
    return std::make_shared<Add<L, Input<R>>>(l, Input<R>(r));
}

template <class L, class R>
class Mult : public Parameter<Mult<L, R>>
{
    std::shared_ptr<L> l_ = nullptr;
    std::shared_ptr<R> r_ = nullptr;

public:
    explicit Mult(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
        : l_(l), r_(r)
    {
        std::cout << "どっちもshared_ptrなMult constructer" << std::endl;
    }

    explicit Mult(L&& l, const std::shared_ptr<R>& r)
        : l_(std::make_shared<L>(l)), r_(r)
    {
        std::cout << "L&&なMult constructer" << std::endl;
    }

    explicit Mult(const std::shared_ptr<L>& l, R&& r)
        : l_(l), r_(std::make_shared<R>(r))
    {
        std::cout << "R&&なMult constructer" << std::endl;
    }

    auto value() const
    {
        return l_->value() * r_->value();
    }

    auto diff(const std::shared_ptr<void>& p) const
    {
        return l_->diff(p) * r_->value() + l_->value() * r_->diff(p);
    }

};  //class Mult

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter<L>, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter<R>, R>, std::nullptr_t> = nullptr>
auto operator*(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
{
    std::cout << "operator* & &" << std::endl;
    return std::make_shared<Mult<L, R>>(l, r);
}

template <class L, class R,
    std::enable_if_t<std::is_arithmetic_v<L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter<R>, R>, std::nullptr_t> = nullptr>
auto operator*(L&& l, const std::shared_ptr<R>& r) -> std::shared_ptr<Mult<Input<L>, R>>
{
    std::cout << "operator* && &" << std::endl;
    return std::make_shared<Mult<Input<L>, R>>(Input<L>{l}, r);
}

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter<L>, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_arithmetic_v<R>, std::nullptr_t> = nullptr>
auto operator*(const std::shared_ptr<L>& l, R&& r) -> std::shared_ptr<Mult<L, Input<R>>>
{
    std::cout << "operator* && &" << std::endl;
    return std::make_shared<Mult<L, Input<R>>>(l, Input<R>(r));
}

}  //namespace kyadet

int main()
{
    using namespace kyadet;
    auto x1 = std::make_shared<Input<int>>(1);
    auto x2 = std::make_shared<Input<int>>(1);
    auto y = x1 * x1 + x1 + x2 * x1;
    std::cout << y->value() << std::endl;
    std::cout << y->diff(x1) << std::endl;
    return 0;
}
