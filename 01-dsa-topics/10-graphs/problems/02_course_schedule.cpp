// Problem: Course Schedule (LeetCode #207)
// Difficulty: Medium
// Approach: Detect cycle in directed graph using topological sort (Kahn's).
//           If we can process all nodes, no cycle exists.
// Time:  O(V + E), Space: O(V + E)

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
    vector<vector<int>> adj(numCourses);
    vector<int> indegree(numCourses, 0);
    for (auto& p : prerequisites) {
        adj[p[1]].push_back(p[0]);
        indegree[p[0]]++;
    }
    queue<int> q;
    for (int i = 0; i < numCourses; i++)
        if (indegree[i] == 0) q.push(i);

    int visited = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        visited++;
        for (int v : adj[u]) {
            if (--indegree[v] == 0) q.push(v);
        }
    }
    return visited == numCourses;
}

int main() {
    vector<vector<int>> pre = {{1,0},{2,0},{3,1},{3,2}};
    cout << canFinish(4, pre) << endl; // 1 (true)
    vector<vector<int>> cycle = {{1,0},{0,1}};
    cout << canFinish(2, cycle) << endl; // 0 (false)
    return 0;
}
