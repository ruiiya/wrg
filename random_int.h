#pragma once

#include "random.h"

namespace wrg {
    template <class T>
    class random<T, typename std::enable_if<std::is_integral<T>::value>::type> {
        public:
        T get(T _a, T _b) const {
            return std::uniform_int_distribution<T>(_a, _b)(wrdrg::_device);
        }
        T get(T _n) const {
            return std::uniform_int_distribution<T>(0,_n-1)(wrdrg::_device);
        }
    };
}