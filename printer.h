#pragma once

#include <cstddef>
#include <ostream>

namespace wrg {
    namespace aux {
        template<std::size_t...> struct seq{};

        template<std::size_t N, std::size_t... Is>
        struct gen_seq : gen_seq<N-1, N-1, Is...>{};

        template<std::size_t... Is>
        struct gen_seq<0, Is...> : seq<Is...>{};

        template<class Ch, class Tr, class Tuple, std::size_t... Is>
        void print_tuple(std::basic_ostream<Ch,Tr>& os, Tuple const& t, seq<Is...>){
            using swallow = int[];
            (void)swallow{0, (void(os << (Is == 0? "" : " ") << std::get<Is>(t)), 0)...};
        }
    }

    namespace detail {
        template<typename T, typename = void>
        class is_container : public std::false_type {
        };

        template<typename T>
        class is_container<T, decltype(void(std::distance(std::declval<T>().begin(), std::declval<T>().end())))>
                : public std::true_type {
        };

        template <typename> 
        struct is_tuple: std::false_type {};
        template <typename ...T> 
        struct is_tuple<std::tuple<T...>>: std::true_type {};
        template <class Lhs, class Rhs>
        struct is_tuple<std::pair<Lhs, Rhs>>: std::true_type {};
    }

    template<class Ch, class Tr, class... Args>
    auto operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
        -> std::basic_ostream<Ch, Tr>&
    {
        aux::print_tuple(os, t, aux::gen_seq<sizeof...(Args)>());
        return os;
    }    

    template<class Ch, class Tr, class Lhs, class Rhs>
    auto operator<<(std::basic_ostream<Ch, Tr>& os, std::pair<Lhs, Rhs> const& t)
        -> std::basic_ostream<Ch, Tr>&
    {
        os << std::make_tuple(t.first, t.second);
        return os;
    }

    template<class Ch, class Tr, class T>
    auto operator<<(std::basic_ostream<Ch, Tr>& os, T const& t)
        -> typename std::enable_if<detail::is_container<T>::value, std::basic_ostream<Ch, Tr>&>::type
    {
        bool is_begin = true;
        for(const auto& e : t) {
            if constexpr (detail::is_container<typename T::value_type>::value) {
                os << (is_begin ? "" : "\n") << e;
            }
            else if constexpr (detail::is_tuple<typename T::value_type>::value) {
                os << (is_begin ? "" : "\n") << e;
            } else {
                os << (is_begin ? "" : " ") << e;
            }
            is_begin = false;
        }
        return os;
    }

}