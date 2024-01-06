#pragma once

#include "printer.h"

#include <filesystem>
#include <unistd.h>
#include <chrono>
#include <thread>


namespace wrg {
    namespace tm {
        class test_maker {
            private:
            std::string _name;
            unsigned _num;
            std::filesystem::path _base_dir;
            unsigned _p;
            public:

            test_maker(std::string _name, unsigned _num, std::filesystem::path _base_dir = std::filesystem::current_path())
            :   _name(_name), _num(_num), _base_dir(_base_dir) {}

            template<class Func, typename... Args>
            void themis_build_input(Func func, Args... args) {

                std::string _input_name = _name + ".inp";

                for(_p = 1;_p <= _num;++_p) {

                    std::string _test_name = _name + std::to_string(_p);
                    auto path = _base_dir / _name / _test_name;

                    std::cerr << "[Themis Build Input][Info] Current: " << path << " - Progress: " << _p << "/" << _num << "\r";
                    std::cerr.flush();

                    if(std::filesystem::exists(path)) {
                        continue;
                    } else {
                        std::filesystem::create_directories(path);
                        wout.open((path / _input_name).string().c_str());

                        func(args...);
                    }
                }
                std::cerr << std::endl;
#ifdef __unix__ 
                wout.open("/dev/null");
#elif defined(_WIN32) || defined(WIN32)  
                wout.open("nul");
#endif
                wout.os.rdbuf(std::cout.rdbuf());
            }

            template<class Func, typename... Args>
            void themis_solve_input(Func func, Args... args) {
                std::streambuf* cin_buf = std::cin.rdbuf();
                std::streambuf* cout_buf = std::cout.rdbuf();

                std::ifstream ifs;
                std::ofstream ofs;

                std::string _input_name = _name + ".inp", _output_name = _name + ".out";

                for(_p = 1;_p <= _num;++_p) {
                    std::string _test_name = _name + std::to_string(_p);
                    auto path = _base_dir / _name / _test_name;

                    std::cerr << "[Themis Solve Input][Info] Current: " << path << " - Progress: " << _p << "/" << _num << "\r";
                    std::cerr.flush();

                    if(!std::filesystem::exists(path)) {
                        continue;
                    }else if(std::filesystem::exists(path / _output_name)){
                        continue;
                    } else {
                        if(ifs.is_open()) ifs.close();
                        if(ofs.is_open()) ofs.close();

                        ifs.open((path / _input_name).string());
                        ofs.open((path / _output_name).string());

                        std::cin.rdbuf(ifs.rdbuf());
                        std::cout.rdbuf(ofs.rdbuf());

                        func(args...);
                    }
                }
                std::cerr << std::endl;

                std::cin.rdbuf(cin_buf);
                std::cout.rdbuf(cout_buf);

                ifs.close();
                ofs.close();
            }
        };
    }
}