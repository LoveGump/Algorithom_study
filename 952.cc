#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

const int N = 210;
vector<int> g[N]; // 邻接表表示图
int n, m;  // n为点数，m为边数
int match[N], vis[N]; // match[i]表示点i的匹配点，vis[i]表示点i是否被访问过
int pre[N], nxt[N]; // pre[i]表示点i的前驱点，nxt[i]表示点i的后继点

// 深度优先搜索寻找增广路径
bool dfs(int u) {
    for (int v : g[u]) {
        if (!vis[v]) {
            vis[v] = 1;
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

int main() {
    cin >> n >> m;
    for (int i = 1; i <= m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
    }

    memset(match, -1, sizeof(match));
    memset(pre, 0, sizeof(pre));
    memset(nxt, 0, sizeof(nxt));
    int res = 0; // 匹配的数量
    // 对每个点进行深度优先搜索
    for (int u = 1; u <= n; ++u) {
        memset(vis, 0, sizeof(vis));
        if (dfs(u)) res++;
    }

    cout << n - res << endl;

    // 输出路径
    vector<bool> used(n + 1, false);
    for (int u = 1; u <= n; ++u) {
        if (!pre[u] && !used[u]) {
            vector<int> path;
            int x = u;
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
    return 0;
}
