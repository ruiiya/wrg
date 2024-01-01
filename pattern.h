#pragma once 
#include "random_int.h"
#include "printer.h"

#include <cassert>
#include <algorithm>

namespace wrg {
    struct pattern {
        struct pattern_quantity {
            int min, max;
            pattern_quantity (int _min, int _max) : min(_min), max(_max) {}
        };
        std::vector<pattern_quantity> _pq;
        std::vector<char> _cs;
        std::vector<pattern> _vp;
        bool _iop = false;  

        uint64_t random_quantity() const {
            static random<int> _device;
            uint64_t q = 1;
            for(auto v: _pq) {
                q = q * _device.get(v.min, v.max);
            }
            return q;
        }

        std::string get() const {
            static random<std::size_t> _device;
            if(_iop) {
                assert(_vp.size());
                return _vp[_device.get(_vp.size())].get();
            }

            uint64_t count = random_quantity();
            std::string res = "";
            for(uint64_t i = 0;i < count;i++) {
                if(!_vp.empty()) {
                    for(const auto& _cp : _vp) {
                        res += _cp.get();
                    }
                } else {
                    res += _cs[_device.get(_cs.size())];
                }
            }

            return res;
        }
    };

    template<class Ch, class Tr, class... Args>
    std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, pattern const& t) {
        if(t._vp.size()) {
            os << t._vp << t._pq;
        } else {
            os << t._cs << t._pq; 
        }
        return os;
    }

    template<class Ch, class Tr, class... Args>
    std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, pattern::pattern_quantity const& t) {
        os << "{" << t.min << ", " << t.max << "}";
        return os;
    }

    class parser {
        public:
        parser() {}
        parser(std::string _s) : _s(_s),_pos(0) {}

        pattern operator() (const std::string& _str) {
            _s = _str;
            _pos = 0;
            return parse();
        }

        parser& operator<< (const std::string& _str) {
            _s = _str;
            _pos = 0;
            parse();
            return *this;
        }

        pattern parse() {
            std::vector<pattern> _vop;
            pattern _cur;
            while(true) {
                char c = move();
                pattern _child;
                if(c == -1 || c == ')') break;
                else if (c == '(') {
                    pattern _child = parse();
                    _cur._vp.push_back(_child);
                    parse_quantity(_child);
                } else if (c == '|') {
                    _vop.emplace_back();
                    std::swap(_vop.back(), _cur);
                } else {
                    if(c == '[') {
                        parse_block(_child._cs);
                    } else if (c == '\\') {
                        char _nc = peak();
                        if (_nc != -1) {
                            _child._cs.push_back(_nc);
                            move();
                        }
                    } else {
                        _child._cs.push_back(c);
                    }
                    parse_quantity(_child);
                    _cur._vp.push_back(_child);
                }
            }

            if(!_vop.empty()) {
                _vop.emplace_back();
                std::swap(_vop.back(), _cur);
                _cur._vp = _vop;
                _cur._iop = true;
            }
            parse_quantity(_cur);

            return _cur;
        }

        void parse_quantity(pattern& _p) {
            while(peak() == '?' || peak() == '{') {
                char c = move();
                if(c == '?') {
                    _p._pq.emplace_back(0,1);
                } else {
                    int min = parse_int();
                    if(peak() == ',' || peak() == '-') {
                        move();
                        int max = parse_int();
                        assert(move() == '}');
                        _p._pq.emplace_back(min, max);
                    } else {
                        assert(move() == '}');
                        _p._pq.emplace_back(min, min);
                    }
                }
            }
        }

        int parse_int() {
            assert(isdigit(peak()));
            int res = 0;
            while(isdigit(peak())) res = res * 10 + move() - '0';
            return res;
        }

        void parse_block(std::vector<char>& _vc) {
            bool in_range = false;
            char last = -1;
            while(peak() != ']') {
                char c = move();
                assert(c != -1);
                if(c == '-') {
                    if(last != -1) {
                        assert(!in_range);
                        in_range = true;
                    } else {
                        last = c;
                    }
                } else if (in_range) {
                    bool ipn = false;
                    if(c == '\\') c = peak(), ipn = true;
                    assert(c >= last);
                    for(char i = last;i <= c;i++) {
                        _vc.push_back(i);
                    }
                    in_range = false;
                    last = -1;
                    if(ipn) move();
                } else {
                    if(last != -1) {
                        _vc.push_back(last);
                    }
                    bool ipn = false;
                    if(c == '\\') c = peak(), ipn = true;
                    assert(c != -1);
                    last = c;
                    if(ipn) move();
                }
            }
            assert(move() == ']');
            assert(!in_range);
            if(last != -1) {
                _vc.push_back(last);
            }

            std::sort(_vc.begin(), _vc.end());
        }

        private:

        char peak() {
            if(_pos == _s.size()) return -1;
            return _s[_pos];
        }

        char move() {
            if(_pos == _s.size()) return -1;
            return _s[_pos++];
        }

        int _pos = 0;
        std::string _s;
    };
}