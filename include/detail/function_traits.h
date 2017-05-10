//
// Created by tiancai on 2017/4/19.
//

#ifndef HEPTAPOD_RPC_HANDLER_TRAITS_H
#define HEPTAPOD_RPC_HANDLER_TRAITS_H

#include <tuple>

namespace hpt {
namespace detail {

template<typename T>
struct IsConstRef
{
    static constexpr bool value =
        std::is_reference<T>::value && std::is_const<std::remove_reference_t<T>>::value;
};

template<typename T, typename... Args>
struct IsAllConstRef
{
    static constexpr bool value =
        IsConstRef<T>::value && IsAllConstRef<Args...>::value;
};

template<typename T>
struct IsAllConstRef<T>
{
    static constexpr bool value = IsConstRef<T>::value;
};

template<typename T>
struct FunctionTraits : FunctionTraits<decltype(&T::operator())>
{

};

template<typename ClassType, typename Ret, typename... Args>
struct FunctionTraits<Ret(ClassType::*)(Args...)> : FunctionTraits<Ret(*)(Args...)>
{

};

template<typename ClassType, typename Ret, typename... Args>
struct FunctionTraits<Ret(ClassType::*)(Args...) const> : FunctionTraits<Ret(*)(Args...)>
{

};

template<typename Ret, typename... Args>
struct FunctionTraits<Ret(*)(Args...)>
{
    static constexpr int Arity = sizeof...(Args);
    using ResultType = Ret;

    template<size_t I>
    struct Arg
    {
        using Type = typename std::tuple_element<I, std::tuple<Args...>>::type;
    };

    using DecayedArgs = typename std::tuple<std::decay_t<Args>...>;
};

} // namespace detail
} // namespace hpt

#endif //HEPTAPOD_RPC_HANDLER_TRAITS_H
