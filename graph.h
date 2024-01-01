#pragma once

#include "detail.h"

#include <optional>
#include <vector>
#include <ostream>
#include <numeric>

namespace wrg {
    template<class We = int>
    struct edge {
        int u, v;
        std::optional<We> w;
    };
    template<class We = int, class Wv = int>
    class base_graph {
        public:

        void add(int u,int v, std::optional<We> w = {}) {
            extend(u + 1), extend(v + 1);
            u = by_label[u];
            v = by_label[v];
            std::size_t index = edges.size();
            edges.push_back({u, v, w});
            adj[u].push_back(index);
            if(!directed && u != v) {
                adj[v].push_back(index);
            }
        }

        void extend(int size) {
            std::size_t _size = static_cast<std::size_t>(size);
            std::size_t _old_size = this->size().first;
            if(_size > _old_size) {
                adj.resize(_size);
                std::vector<int> _vl(_size - _old_size);
                std::iota(_vl.begin(), _vl.end(), _old_size);
                for(int& v : _vl) {
                    label.push_back(v);
                    by_label.push_back(v);
                }
            }
        }

        std::pair<int,int> size() {
            return std::make_pair(adj.size(), edges.size());
        }

        std::vector<edge<We>> get_edges() {
            std::vector<edge<We>> _edges = this->edges;
            for(auto& v : _edges) {
                v.u = label[v.u];
                v.v = label[v.v];
            }
            return _edges;
        }

        private:
        bool directed = false;
        std::vector<edge<We>> edges;
        std::vector<std::vector<int>> adj;
        std::vector<int> label;
        std::vector<int> by_label;
        std::vector<std::optional<Wv>> vertex_weights;
    };
}



