#pragma once

#include <memory>
#include <cmath>
#include <string>
#include <unordered_map>

namespace kyadet
{

struct Parameter {
};

template <class T, std::enable_if_t<!std::is_base_of_v<Parameter, T>, std::nullptr_t> = nullptr>
class Input : public Parameter
{
    std::shared_ptr<T> t_;

public:
    explicit Input(T t)
        : t_(std::make_shared<T>(t)) {}

    auto value() const
    {
        return *t_;
    }

    auto diffGraph(const std::shared_ptr<Parameter>& p) const
    {
        return this == p.get() ? std::make_shared<Input<T>>(1) : std::make_shared<Input<T>>(0);
    }

    /*
    void reset(const T& t)
    {
        *t_ = t;
    }
     */

};  //class Input

template <class L, class R>
class Add : public Parameter
{
    std::shared_ptr<L> l_;
    std::shared_ptr<R> r_;

public:
    explicit Add(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
        : l_(l), r_(r) {}

    explicit Add(L&& l, const std::shared_ptr<R>& r)
        : l_(std::make_shared<L>(l)), r_(r) {}

    explicit Add(const std::shared_ptr<L>& l, R&& r)
        : l_(l), r_(std::make_shared<R>(r)) {}

    auto value() const
    {
        return l_->value() + r_->value();
    }

    auto diffGraph(const std::shared_ptr<Parameter>& p) const
    {
        return l_->diffGraph(p) + r_->diffGraph(p);
    }

    /*
    template<class U>
    auto diff(){
        std::unordered_map<std::shared_ptr<Parameter>,U>& umap;
        diffImpl(umap);
        return umap;
    }

    template<class U>
    void diffImpl(std::unordered_map<std::shared_ptr<Parameter>,U>& umap){

    }
     */

};  //class Add

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter, R>, std::nullptr_t> = nullptr>
auto operator+(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
{
    return std::make_shared<Add<L, R>>(l, r);
}

template <class L, class R,
    std::enable_if_t<std::is_arithmetic_v<L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter, R>, std::nullptr_t> = nullptr>
auto operator+(L&& l, const std::shared_ptr<R>& r)
{
    return std::make_shared<Add<Input<L>, R>>(Input<L>{l}, r);
}

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_arithmetic_v<R>, std::nullptr_t> = nullptr>
auto operator+(const std::shared_ptr<L>& l, R&& r)
{
    return std::make_shared<Add<L, Input<R>>>(l, Input<R>{r});
}

template <class L, class R>
class Sub : public Parameter
{
    std::shared_ptr<L> l_;
    std::shared_ptr<R> r_;

public:
    explicit Sub(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
        : l_(l), r_(r) {}

    explicit Sub(L&& l, const std::shared_ptr<R>& r)
        : l_(std::make_shared<L>(l)), r_(r) {}

    explicit Sub(const std::shared_ptr<L>& l, R&& r)
        : l_(l), r_(std::make_shared<R>(r)) {}

    auto value() const
    {
        return l_->value() - r_->value();
    }

    auto diffGraph(const std::shared_ptr<Parameter>& p) const
    {
        return l_->diffGraph(p) - r_->diffGraph(p);
    }

};  //class Sub

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter, R>, std::nullptr_t> = nullptr>
auto operator-(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
{
    return std::make_shared<Sub<L, R>>(l, r);
}

template <class L, class R,
    std::enable_if_t<std::is_arithmetic_v<L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter, R>, std::nullptr_t> = nullptr>
auto operator-(L&& l, const std::shared_ptr<R>& r)
{
    return std::make_shared<Sub<Input<L>, R>>(Input<L>{l}, r);
}

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_arithmetic_v<R>, std::nullptr_t> = nullptr>
auto operator-(const std::shared_ptr<L>& l, R&& r)
{
    return std::make_shared<Sub<L, Input<R>>>(l, Input<R>(r));
}

template <class L, class R>
class Mult : public Parameter
{
    std::shared_ptr<L> l_;
    std::shared_ptr<R> r_;

public:
    explicit Mult(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
        : l_(l), r_(r) {}

    explicit Mult(L&& l, const std::shared_ptr<R>& r)
        : l_(std::make_shared<L>(l)), r_(r) {}

    explicit Mult(const std::shared_ptr<L>& l, R&& r)
        : l_(l), r_(std::make_shared<R>(r)) {}

    auto value() const
    {
        return l_->value() * r_->value();
    }

    auto diffGraph(const std::shared_ptr<Parameter>& p) const
    {
        return l_->diffGraph(p) * r_ + l_ * r_->diffGraph(p);
    }

};  //class Mult

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter, R>, std::nullptr_t> = nullptr>
auto operator*(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
{
    return std::make_shared<Mult<L, R>>(l, r);
}

template <class L, class R,
    std::enable_if_t<std::is_arithmetic_v<L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter, R>, std::nullptr_t> = nullptr>
auto operator*(L&& l, const std::shared_ptr<R>& r)
{
    return std::make_shared<Mult<Input<L>, R>>(Input<L>{l}, r);
}

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_arithmetic_v<R>, std::nullptr_t> = nullptr>
auto operator*(const std::shared_ptr<L>& l, R&& r)
{
    return std::make_shared<Mult<L, Input<R>>>(l, Input<R>(r));
}

template <class L, class R>
class Div : public Parameter
{
    std::shared_ptr<L> l_;
    std::shared_ptr<R> r_;

public:
    explicit Div(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
        : l_(l), r_(r) {}

    explicit Div(L&& l, const std::shared_ptr<R>& r)
        : l_(std::make_shared<L>(l)), r_(r) {}

    explicit Div(const std::shared_ptr<L>& l, R&& r)
        : l_(l), r_(std::make_shared<R>(r)) {}

    auto value() const
    {
        return l_->value() / r_->value();
    }

    auto diffGraph(const std::shared_ptr<Parameter>& p) const
    {
        return (l_->diffGraph(p) * r_ - l_ * r_->diffGraph(p)) / r_ / r_;
    }

};  //class Div

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter, R>, std::nullptr_t> = nullptr>
auto operator/(const std::shared_ptr<L>& l, const std::shared_ptr<R>& r)
{
    return std::make_shared<Div<L, R>>(l, r);
}

template <class L, class R,
    std::enable_if_t<std::is_arithmetic_v<L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_base_of_v<Parameter, R>, std::nullptr_t> = nullptr>
auto operator/(L&& l, const std::shared_ptr<R>& r)
{
    return std::make_shared<Div<Input<L>, R>>(Input<L>{l}, r);
}

template <class L, class R,
    std::enable_if_t<std::is_base_of_v<Parameter, L>, std::nullptr_t> = nullptr,
    std::enable_if_t<std::is_arithmetic_v<R>, std::nullptr_t> = nullptr>
auto operator/(const std::shared_ptr<L>& l, R&& r)
{
    return std::make_shared<Div<L, Input<R>>>(l, Input<R>(r));
}

template <class T>
class Exp : public Parameter
{
    std::shared_ptr<T> t_;

public:
    explicit Exp(const std::shared_ptr<T>& t)
        : t_(t) {}

    explicit Exp(T&& t)
        : t_(std::make_shared<T>(t)) {}

    auto value() const
    {
        return std::exp(t_->value());
    }

    auto diffGraph(const std::shared_ptr<Parameter>& p) const
    {
        return exp(t_) * t_->diffGraph(p);
    }

};  //class Exp

template <class T, std::enable_if_t<std::is_base_of_v<Parameter, T>, std::nullptr_t> = nullptr>
auto exp(std::shared_ptr<T> t)
{
    return std::make_shared<Exp<T>>(t);
}

}  // namespace kyadet
