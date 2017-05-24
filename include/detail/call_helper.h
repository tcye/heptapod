//
// Created by tiancai on 2017/5/9.
//

#ifndef HEPTAPOD_CALL_HELPER_H
#define HEPTAPOD_CALL_HELPER_H

#include <tuple>

namespace hpt {
namespace detail {


template<typename Ret, typename ClassType, typename... Args, std::size_t... I>
Ret CallHelper(Ret(ClassType::*func)(Args...), ClassType* s, std::tuple<std::decay_t<Args>...>&& args, std::index_sequence<I...>)
{
    return (s->*func)(std::get<I>(args)...);
}

template<typename Ret, typename ClassType, typename... Args>
Ret Call(Ret(ClassType::*func)(Args...), ClassType* s, std::tuple<std::decay_t<Args>...>& args)
{
    return CallHelper(func, s, std::forward<std::tuple<std::decay_t<Args>...>>(args), std::index_sequence_for<Args...>{});
};

} // namespace detail
} // namespace hpt

#endif //HEPTAPOD_CALL_HELPER_H
