//
// Created by tiancai on 2017/5/9.
//

#ifndef HEPTAPOD_CALL_HELPER_H
#define HEPTAPOD_CALL_HELPER_H

#include <tuple>

namespace hpt {
namespace detail {


template<typename F, typename... Args, std::size_t... I>
decltype(auto) Call(F func, std::tuple<Args...>&& args, std::index_sequence<I...>)
{
    return func(std::get<I>(args)...);
}

template<typename F, typename... Args>
decltype(auto) Call(F func, std::tuple<Args...>& args)
{
    return Call(func, std::forward<std::tuple<Args...>>(args), std::index_sequence_for<Args...>{});
};

} // namespace detail
} // namespace hpt

#endif //HEPTAPOD_CALL_HELPER_H
