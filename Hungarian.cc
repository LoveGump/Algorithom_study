#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

class Hungarian {
public:
    vector<vector<int>> g; // 邻接表
    int n, m; // 左节点数和 边数
    vector<int> match; // 右节点匹配的左节点
    vector<bool> vis; // 是否访问过
    vector<int> pre; // 右节点的前驱
    vector<int> nxt; // 左节点的后继


    /**
     * @brief 构造函数 初始化数组
     * @param n_ 左部点数
     * @param m_ 右部点数
     */
    Hungarian(int n_, int m_) {
        n = n_;
        m = m_;
        g = vector<vector<int>>(n + 1);
        match = vector<int>(n + 1, -1);
        vis = vector<bool>(n + 1, false);
        pre = vector<int>(n + 1, 0); // 右节点
        nxt = vector<int>(n + 1, 0); // 左节点
    }
    /**
     * @brief 添加边
     * @param u 左节点
     * @param v 右节点
     */
    void add_edge(int u, int v) {
        g[u].push_back(v);
    }

    /**
     * @brief 匈牙利算法 寻找增广路
     * @param u 左部点
     * @return 是否找到增广路
     */
    bool dfs(int u) {
        for (int v : g[u]) {
            if (!vis[v]) {
                vis[v] = true;
                // 如果v没有匹配或者v的匹配可以找到增广路
                if (match[v] == -1 || dfs(match[v])) {
                    match[v] = u;
                    nxt[u] = v;
                    pre[v] = u;
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * @brief 匈牙利算法 求解最大匹配
     */
    void solve() {
        int res = 0;
        for (int i = 1; i <= n; ++i) {
            fill(vis.begin(), vis.end(), false); // 每次都要重置vis
            if (dfs(i)) res++;
        }
        cout << n - res << endl;
        vector<bool> used(n + 1, false);  // 记录左部点是否已经访问过
        for (int i = 1; i <= n; ++i) {
            if (!pre[i] && !used[i]) {
                vector<int> path;
                int x = i;
                while (x && !used[x]) {
                    path.push_back(x);
                    used[x] = true;
                    x = nxt[x];
                }
                cout << path.size();
                for (int v : path) cout << " " << v;
                cout << endl;
            }
        }
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    Hungarian solver(n, m);
    for (int i = 1; i <= m; ++i) {
        int u, v;
        cin >> u >> v;
        solver.add_edge(u, v);
    }

    solver.solve();
    return 0;
}
