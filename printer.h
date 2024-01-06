#pragma once

#include <cstddef>
#include <iostream>
#include <optional>
#include <list>
#include <cassert>
#include <fstream>
#include <iomanip>

namespace wrg {
    namespace detail {

        template<std::size_t...> struct seq{};

        template<std::size_t N, std::size_t... Is>
        struct gen_seq : gen_seq<N-1, N-1, Is...>{};

        template<std::size_t... Is>
        struct gen_seq<0, Is...> : seq<Is...>{};

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

        template<typename T, typename = void>
        class hasOstreamMethod : public std::false_type {
        };

        template<typename T>
        class hasOstreamMethod<T, decltype(void(std::declval<std::ostream &>().operator<<(std::declval<T>())))>
                : public std::true_type {
        };

        template<typename T, typename = void>
        class hasOstreamFreeFunction : public std::false_type {
        };

        template<typename T>
        class hasOstreamFreeFunction<T, decltype(void(
                std::operator<<(std::declval<std::ostream &>(), std::declval<T>())))> : public std::true_type {
        };

        template<class T>
        auto get_vector_size(T const& t) -> typename std::enable_if<detail::is_container<T>::value, std::list<std::size_t>>::type {
            std::list<std::size_t> res;
            if constexpr (detail::is_container<typename T::value_type>::value) {
                std::optional<std::list<std::size_t>> child {};
                for(auto const& v : t) {
                    auto v_size = get_vector_size(v);
                    if(!child.has_value()) child = v_size;
                    else if (child.value() != v_size) assert(false);
                }
                res.splice(res.end(), child.value());
            }
            res.emplace_front(t.size());
            return res;
        }
    }

    class wrg_ostream {
        public:
        std::ostream os;
        wrg_ostream() : os(std::cout.rdbuf()) {}
        wrg_ostream(std::ostream& os) : os(os.rdbuf()) {}
        wrg_ostream(const char* file_name, std::ios_base::openmode __mode = std::ios_base::out) : os(&__temp_null_streambuf) {
            open(file_name, __mode);
        }

        void open(const char* __s, std::ios_base::openmode __mode = std::ios_base::out) {
            static std::ofstream __temp_file_stream;
            if (__temp_file_stream.is_open()) {
                __temp_file_stream.close();
            }
            __temp_file_stream.open(__s, std::ofstream::out);
            os.rdbuf(__temp_file_stream.rdbuf());
        }

        void print_size(bool _value) {
            __print_size = _value;
        }

        const bool& is_print_size() const {
            return __print_size;
        }

        const std::string& sep_vector() const {
            return __print_sep_vector;
        }

        const std::string& sep_vector_value() const {
            return __print_sep_vector_value;
        }

        void set_sep_vector(std::string const& _value) {
            __print_sep_vector = _value;
        }

        void set_sep_vector_value(std::string const& _value) {
            __print_sep_vector_value = _value;
        }

        const std::string& sep_tuple() const {
            return __print_sep_tuple;
        }

        void set_sep_tuple(std::string const& _value) {
            __print_sep_tuple = _value;
        }

        private:
        bool __print_size = false;
        std::string __print_sep_vector = "\n";
        std::string __print_sep_tuple = " ";
        std::string __print_sep_vector_value = " ";

        class NulStreambuf : public std::streambuf
        {
            char                dummyBuffer[ 64 ];
        protected:
            virtual int         overflow( int c ) 
            {
                setp( dummyBuffer, dummyBuffer + sizeof( dummyBuffer ) );
                return (c == traits_type::eof()) ? '\0' : c;
            }
        }__temp_null_streambuf;
    };

    template<class T>
    auto operator<<(wrg_ostream& os, T const& t) 
    -> typename std::enable_if<detail::hasOstreamMethod<T>::value || detail::hasOstreamFreeFunction<T>::value, wrg_ostream&>::type {
        os.os << t;
        return os;
    }

    template<class Tuple, std::size_t... Is>
    void print_tuple(::wrg::wrg_ostream& os,std::string sep, Tuple const& t, detail::seq<Is...>){
        using swallow = int[];
        (void)swallow{0, (void(os << (Is == 0? "" : sep) << std::get<Is>(t)), 0)...};
    }

    struct __set_print_size {bool _value;};
    __set_print_size print_size(bool _value = true) {return __set_print_size{_value};} 
    auto operator<<(wrg_ostream& os, __set_print_size const& t) -> wrg_ostream&
    {
        os.print_size(t._value);
        return os;
    }

    struct __set_print_sep_vector {std::string _value;};
    __set_print_sep_vector sep_vector(std::string _value) {return __set_print_sep_vector{_value};} 
    auto operator<<(wrg_ostream& os, __set_print_sep_vector const& t) -> wrg_ostream&
    {
        os.set_sep_vector(t._value);
        return os;
    }

    struct __set_print_sep_vector_value {std::string _value;};
    __set_print_sep_vector_value sep_vector_value(std::string _value) {return __set_print_sep_vector_value{_value};} 
    auto operator<<(wrg_ostream& os, __set_print_sep_vector_value const& t) -> wrg_ostream&
    {
        os.set_sep_vector_value(t._value);
        return os;
    }

    struct __set_print_sep_tuple {std::string _value;};
    __set_print_sep_tuple sep_tuple(std::string _value) {return __set_print_sep_tuple{_value};} 
    auto operator<<(wrg_ostream& os, __set_print_sep_tuple const& t) -> wrg_ostream&
    {
        os.set_sep_tuple(t._value);
        return os;
    }

    template<class... Args>
    auto operator<<(wrg_ostream& os, std::tuple<Args...> const& t)
        -> wrg_ostream&
    {
        if(os.is_print_size()) os.os << std::tuple_size<std::tuple<Args...>>::value << '\n';
        print_tuple(os,os.sep_tuple(), t, detail::gen_seq<sizeof...(Args)>());
        return os;
    }    

    template<class Lhs, class Rhs>
    auto operator<<(wrg_ostream& os, std::pair<Lhs, Rhs> const& t)
        -> wrg_ostream&
    {
        os << std::make_tuple(t.first, t.second);
        return os;
    }

    template<class T>
    auto operator<<(wrg_ostream& os, T const& t)
        -> typename std::enable_if<detail::is_container<T>::value, wrg_ostream&>::type
    {
        bool __is_print_size = os.is_print_size();
        if(os.is_print_size()) {
            os.print_size(false);
            os << detail::get_vector_size(t);
            os.os << '\n';
        }
        bool is_begin = true;
        for(const auto& e : t) {
            if constexpr (detail::is_container<typename T::value_type>::value) {
                os.os << (is_begin ? "" : os.sep_vector());
                os << e;
            }
            else if constexpr (detail::is_tuple<typename T::value_type>::value) {
                os.os << (is_begin ? "" : os.sep_vector());
                os << e;
            } else {
                os.os << (is_begin ? "" : os.sep_vector_value());
                os << e;
            }
            is_begin = false;
        }
        os.print_size(__is_print_size);
        return os;
    }

}

wrg::wrg_ostream wout;