#ifndef SIGMA_PARAMETER_LIST_HPP_INCLUDED
#define SIGMA_PARAMETER_LIST_HPP_INCLUDED

#include <tuple>

namespace sigma
{

template<typename... Ps>
struct parameter_list
{
    template<std::size_t N>
    struct at
    {
        static_assert(N < sizeof...(Ps), "requested index is out of bounds");
        using type = std::tuple_element_t<N, std::tuple<Ps...>>;
    };
};

// functionality to merge any parameter_list and other elements into a
// parameter_list
template<typename... Ts>
struct merge_parameter_list
{
    using type = sigma::parameter_list<Ts...>;
};

template<>
struct merge_parameter_list<>
{
    using type = sigma::parameter_list<>;
};

template<typename T>
struct merge_parameter_list<T>
{
    using type = sigma::parameter_list<T>;
};

template<typename... Ts>
struct merge_parameter_list<sigma::parameter_list<Ts...>>
{
    using type = sigma::parameter_list<Ts...>;
};

template<typename... T1s, typename... T2s>
struct merge_parameter_list<sigma::parameter_list<T1s...>,
                            sigma::parameter_list<T2s...>>
{
    using type = sigma::parameter_list<T1s..., T2s...>;
};

template<typename T1, typename T2>
struct merge_parameter_list<T1, T2>
{
    using type = sigma::parameter_list<T1, T2>;
};

template<typename T, typename... Ts>
struct merge_parameter_list<T, sigma::parameter_list<Ts...>>
{
    using type = sigma::parameter_list<T, Ts...>;
};

template<typename... Ts, typename T>
struct merge_parameter_list<sigma::parameter_list<Ts...>, T>
{
    using type = sigma::parameter_list<Ts..., T>;
};

template<typename... Ts, typename T, typename... Rest>
struct merge_parameter_list<sigma::parameter_list<Ts...>, T, Rest...>
{
    using temp = typename merge_parameter_list<Rest...>::type;
    using type = typename merge_parameter_list<sigma::parameter_list<Ts..., T>,
                                               temp>::type;
};

template<typename T, typename... Ts, typename... Rest>
struct merge_parameter_list<T, sigma::parameter_list<Ts...>, Rest...>
{
    using temp = typename merge_parameter_list<Rest...>::type;
    using type = typename merge_parameter_list<sigma::parameter_list<T, Ts...>,
                                               temp>::type;
};

template<typename... T1s, typename... T2s, typename... Rest>
struct merge_parameter_list<sigma::parameter_list<T1s...>,
                            sigma::parameter_list<T2s...>,
                            Rest...>
{
    using temp = typename merge_parameter_list<Rest...>::type;
    using type =
        typename merge_parameter_list<sigma::parameter_list<T1s..., T2s...>,
                                      temp>::type;
};

template<typename T1, typename T2, typename... Rest>
struct merge_parameter_list<T1, T2, Rest...>
{
    using temp = typename merge_parameter_list<Rest...>::type;
    using type = typename merge_parameter_list<sigma::parameter_list<T1, T2>,
                                               temp>::type;
};

} // namespace sigma

#endif
