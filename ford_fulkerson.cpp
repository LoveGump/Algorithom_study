#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

class FordFulkerson {
private:
    int vertices;
    vector<vector<int>> residualGraph;  // 残量图
    vector<vector<int>> capacity;       // 容量图
    vector<vector<int>> flow;           // 流量图

    /**
     * @brief 使用BFS寻找增广路径
     * @param source 源点
     * @param sink 汇点
     * @param parent 用于存储路径的父节点数组
     * @return 如果找到增广路径返回true，否则返回false
     */
    bool bfs(int source, int sink, vector<int>& parent) {
        vector<bool> visited(vertices, false);
        queue<int> q;
        q.push(source);
        visited[source] = true;
        parent[source] = -1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < vertices; v++) {
                if (!visited[v] && residualGraph[u][v] > 0) {
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }

        return visited[sink];
    }

public:
    FordFulkerson(int v) : vertices(v) {
        // 初始化图
        residualGraph = vector<vector<int>>(v, vector<int>(v, 0));
        capacity = vector<vector<int>>(v, vector<int>(v, 0));
        flow = vector<vector<int>>(v, vector<int>(v, 0));
    }

    /**
     * @brief 添加一条边到图中
     * @param from 起点
     * @param to 终点
     * @param capacity 边的容量
     */
    void addEdge(int from, int to, int cap) {
        capacity[from][to] = cap;
        residualGraph[from][to] = cap;
    }

    /**
     * @brief 计算从源点到汇点的最大流
     * @param source 源点
     * @param sink 汇点
     * @return 最大流的值
     */
    int maxFlow(int source, int sink) {
        int max_flow = 0;
        vector<int> parent(vertices);

        // 当存在增广路径时继续
        while (bfs(source, sink, parent)) {
            // 找到增广路径上的最小残量
            int path_flow = numeric_limits<int>::max();
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                path_flow = min(path_flow, residualGraph[u][v]);
            }

            // 更新残量图和流量图
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                residualGraph[u][v] -= path_flow;
                residualGraph[v][u] += path_flow;
                flow[u][v] += path_flow;
                flow[v][u] -= path_flow;
            }

            max_flow += path_flow;
        }

        return max_flow;
    }

    /**
     * @brief 获取边的流量
     * @param from 起点
     * @param to 终点
     * @return 边的流量
     */
    int getFlow(int from, int to) {
        return flow[from][to];
    }

    /**
     * @brief 打印网络中的流量分布
     */
    void printFlow() {
        cout << "网络中的流量分布：" << endl;
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (capacity[i][j] > 0) {
                    cout << "边 " << i << " -> " << j
                         << " 的流量: " << flow[i][j]
                         << "/" << capacity[i][j] << endl;
                }
            }
        }
    }
};

int main() {
    // 创建一个有6个顶点的网络
    FordFulkerson g(6);

    // 添加边（包含容量）
    g.addEdge(0, 1, 16);
    g.addEdge(0, 2, 13);
    g.addEdge(1, 2, 10);
    g.addEdge(1, 3, 12);
    g.addEdge(2, 1, 4);
    g.addEdge(2, 4, 14);
    g.addEdge(3, 2, 9);
    g.addEdge(3, 5, 20);
    g.addEdge(4, 3, 7);
    g.addEdge(4, 5, 4);

    // 计算从顶点0到顶点5的最大流
    int max_flow = g.maxFlow(0, 5);
    cout << "最大流: " << max_flow << endl;

    // 打印网络中的流量分布
    g.printFlow();

    return 0;
}
