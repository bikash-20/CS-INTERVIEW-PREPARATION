// Problem: Redundant Connection (LeetCode #684) — uses Union-Find
// Difficulty: Medium
// Approach: Process edges. If find(u) == find(v), adding this edge creates a cycle
//           (it's the redundant edge). Otherwise union them.
// Time:  O(V + E * α(V)), Space: O(V)

#include <iostream>
#include <vector>
using namespace std;

class UnionFind {
    vector<int> parent, rank_;
public:
    UnionFind(int n) : parent(n), rank_(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]); // path compression
        return parent[x];
    }
    bool unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        if (rank_[rx] < rank_[ry]) swap(rx, ry);
        parent[ry] = rx;
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
        return true;
    }
};

vector<int> findRedundantConnection(vector<vector<int>>& edges) {
    int n = edges.size();
    UnionFind uf(n + 1);
    for (auto& e : edges)
        if (!uf.unite(e[0], e[1])) return e;
    return {};
}

int main() {
    vector<vector<int>> edges = {{1,2},{1,3},{2,3}};
    auto ans = findRedundantConnection(edges);
    cout << ans[0] << " " << ans[1] << endl; // 2 3
    return 0;
}
