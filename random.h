#pragma once

#include <any>
#include <random>
#include <chrono>
#include <iostream>

namespace wrg {
    template <class T, class Enable = void>
    class random {};

    class wrdrg {
        public:
        static std::mt19937_64 _device;

        template<class T, typename... Args>
        static T get(T& _t, Args... args) {
            static random<T> _device;
            return _t = _device.get(args...);
        }

        template<class T, typename... Args>
        static T tget(Args... args) {
            static random<T> _device;
            return _device.get(args...);
        }
    };

    std::mt19937_64 wrdrg::_device = std::mt19937_64(std::chrono::system_clock::now().time_since_epoch().count());
}