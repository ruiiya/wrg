#pragma once
#include <utility>
#include "random.h"

namespace wrg {
    template <class Lhs, class Rhs>
        class random<std::pair<Lhs, Rhs>> {
            public:
            template <typename... Args>
            auto get(Args... args) -> typename std::enable_if<std::is_same<Lhs, Rhs>::value, std::pair<Lhs, Rhs>>::type {
                return std::make_pair(_device_lhs.get(args...), _device_rhs.get(args...));
            }
            private:
            random<Lhs> _device_lhs;
            random<Rhs> _device_rhs;
    };
}