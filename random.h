#pragma once
#include <any>
#include <random>
#include <chrono>

namespace wrg {
    template <class T, class Enable = void>
    class random {};

    class wrdrg {
        public:
        static std::mt19937_64 _device;

        wrdrg() :_os(std::cout) {}
        wrdrg(std::ostream& _os) : _os(_os) {}

        template<typename T>
        wrdrg& operator >> (T& _t){
            if(_value.type() == typeid(T)) {
                _t = std::any_cast<T&&>(std::move(_value));
            }
            return *this;
        }

        template<class T>
        wrdrg& operator << (const T& _t) {
            _value = _t;
            _os << _t;
            return *this;
        }

        bool has_value() const {
            return _value.has_value();
        }

        void set_print(bool _value = false) {
            _print = _value;
        }

        template<class T, typename... Args>
        static T tget(T& _t, Args... args) {
            static random<T> _device;
            return _t = _device.get(args...);
        }

        private:
        std::any _value;
        bool _print = true;
        std::ostream& _os;
    };

    std::mt19937_64 wrdrg::_device = std::mt19937_64(std::chrono::system_clock::now().time_since_epoch().count());
}

wrg::wrdrg gout;