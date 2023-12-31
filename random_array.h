#pragma once

#include "random_int.h"
#include "random_float.h"

#include <vector>
#include <set>
#include <unordered_set>


namespace wrg {

    namespace detail {
        template<class T, typename Enable = void>
        struct dict_container{
            typedef std::set<T> type;
        };

        template<class T>
        struct dict_container<T,typename std::hash<T>::result_type> {
            typedef std::unordered_set<T> type;
        };

        template<class T>
        std::vector<std::vector<T>> split(std::vector<T> const& t, std::size_t bunch_size) {
            std::vector<std::vector<T>> bunches;
            bunches.reserve(t.size() / bunch_size + 1);
            for(std::size_t i = 0;i < t.size(); i += bunch_size) {
                bunches.emplace_back();
                auto last = std::min(t.size(), i + bunch_size);
                auto& vec = bunches.back();
                vec.reserve(last - i);
                move(t.begin() + i, t.begin() + last, back_inserter(vec));
            }
            return bunches;
        }
    }

    enum ARRAY_TYPE {
        NORMAL,
        UNIQUE,
        ALL,
    };

    template<class T>
    class random<std::vector<T>> {
        public:
        template <typename... Args>
        std::vector<T> get(ARRAY_TYPE type, Args... args) {
            switch (type) {
                case UNIQUE:
                    return get_unique(args...);
                // case ALL:
                //     return get_all(args...);
                case NORMAL:
                default:
                    return get_normal(args...);
            }
        }

        template <typename Function, typename... Args>
        std::vector<T> get(Function&& _func, ARRAY_TYPE _type, Args... args) {
            switch (_type) {
                case UNIQUE:
                    return get_unique_func(_func, args...);
                // case ALL:
                //     return get_all_func(_func, args...);
                case NORMAL:
                default:
                    return get_normal_func(_func, args...);
            }
        }

        template <typename Function, typename... Args>
        std::vector<T> get_normal_func(Function&& _func, std::size_t _n, Args... args) {
            std::vector<T> _result(_n);
            for(T& _value : _result) _value = _func(args...);
            return _result;
        }

        template <typename... Args>
        std::vector<T> get_normal(std::size_t _n, Args... args) {
            return get_normal_func([&](Args... args) {return _device.get(args...);}, _n, args...);
        }

        template <typename Function, typename... Args>
        std::vector<T> get_unique_func(Function&& _func, std::size_t _n, Args... args) {
            typename detail::dict_container<T>::type _dict;
            std::vector<T> _result;
            size_t _retries = (_n + 10) * static_cast<size_t>(log(static_cast<double>(_n) + 10)) * 2;
            _result.reserve(_n);
            while(_result.size() != _n) {
                T _temp = _device.get(args...);
                if(_dict.insert(_temp).second) {
                    _result.push_back(_temp);
                }
                if(--_retries == 0) {
                    return {};
                }
            }
            return _result;
        }

        template <typename... Args>
        std::vector<T> get_unique(std::size_t _n, Args... args) {
            return get_unique_func([&](Args... args) {return _device.get(args...);}, _n, args...);
        }

        template <typename Function, typename... Args>
        std::vector<T> get_all_func(Function&& _func, std::size_t& _n, Args... args) {
            typename detail::dict_container<T>::type _dict;
            std::vector<T> _result;
            size_t _count = 0;
            while(true) {
                T _temp = _device.get(args...);
                if(_dict.insert(_temp).second) {
                    _result.push_back(_temp);
                    _count = 0;
                }
                if(++_count > (_result.size() + 10) * 20) {
                    _n = _result.size();
                    return _result;
                }
            }
        }

        template <typename... Args>
        std::vector<T> get_all(std::size_t& _n, Args... args) {
            return get_all_func([&](Args... args) {return _device.get(args...);}, _n, args...);
        }

        private:
        random<T> _device;
    };


}