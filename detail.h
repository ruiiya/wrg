#pragma once

#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

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

        template<class T, class U, typename Enable = void>
        struct map_container{
            typedef std::map<T, U> type;
        };

        template<class T, class U>
        struct map_container<T, U,typename std::hash<T>::result_type> {
            typedef std::unordered_map<T, U> type;
        };
    }

}