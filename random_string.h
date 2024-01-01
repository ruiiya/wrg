#pragma once

#include "random_array.h"
#include "pattern.h"



namespace wrg {
    template<>
    class random<std::string> {
        public:
        std::string get(std::size_t _n, char _a, char _b) {
            static random<std::vector<char>> _device;
            return __to_string(_device.get(NORMAL, _n, _a, _b));
        }

        std::string get(std::size_t _n, std::string _pattern) {
            static random<std::size_t> _device;
            std::vector<char> chars;
            for(std::size_t i = 0; i < _pattern.size(); i++) {
                if(i + 2 < _pattern.size() && _pattern[i+1] == '-') {
                    for(char c = _pattern[i]; c <= _pattern[i+2]; c++) {
                        chars.push_back(c);
                    }
                    i += 2;
                } else {
                    chars.push_back(_pattern[i]);
                }
            }
            std::sort(chars.begin(), chars.end());
            
            std::string _result;
            _result.resize(_n);
            for(char& c : _result) c = chars[_device.get(chars.size())];
            return _result;
        }

        std::string get(std::string _pattern) {
            static parser _device;
            return _device(_pattern).get();
        }

        private:
        std::string __to_string(const std::vector<char>& _t) {
            return std::string(_t.begin(), _t.end());
        }
    };
}