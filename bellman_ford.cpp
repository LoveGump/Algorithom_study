#include <iostream>
#include <vector>
#include <limits>
using namespace std;

class BellmanFord {
private:
    int vertices;
    struct Edge {
        int from, to, weight;
        Edge(int f, int t, int w) : from(f), to(t), weight(w) {}
    };
    vector<Edge> edges;

public:
    BellmanFord(int v) : vertices(v) {}

    /**
     * @brief 添加一条边到图中
     * @param from 起点
     * @param to 终点
     * @param weight 边的权重
     */
    void addEdge(int from, int to, int weight) {
        edges.emplace_back(from, to, weight);
    }

    /**
     * @brief 使用Bellman-Ford算法计算从起点到所有顶点的最短路径
     * @param start 起点
     * @return 返回一个pair，包含距离数组和前驱节点数组，如果存在负环则返回空数组
     */
    pair<vector<int>, vector<int>> bellmanFord(int start) {
        // 初始化距离数组和前驱节点数组
        vector<int> distances(vertices, numeric_limits<int>::max());
        vector<int> predecessors(vertices, -1);
        distances[start] = 0;

        // 进行V-1次松弛操作
        for (int i = 1; i < vertices; i++) {
            for (const Edge& edge : edges) {
                if (distances[edge.from] != numeric_limits<int>::max() &&
                    distances[edge.from] + edge.weight < distances[edge.to]) {
                    distances[edge.to] = distances[edge.from] + edge.weight;
                    predecessors[edge.to] = edge.from;
                }
            }
        }

        // 检查是否存在负环
        for (const Edge& edge : edges) {
            if (distances[edge.from] != numeric_limits<int>::max() &&
                distances[edge.from] + edge.weight < distances[edge.to]) {
                // 存在负环，返回空数组
                return {{}, {}};
            }
        }

        return {distances, predecessors};
    }

    /**
     * @brief 获取从起点到终点的最短路径
     * @param start 起点
     * @param end 终点
     * @return 返回路径列表，如果不存在路径或存在负环则返回空列表
     */
    vector<int> getShortestPath(int start, int end) {
        auto [distances, predecessors] = bellmanFord(start);

        // 检查是否存在负环或无法到达终点
        if (distances.empty() || distances[end] == numeric_limits<int>::max()) {
            return {};
        }

        // 重建路径
        vector<int> path;
        for (int current = end; current != -1; current = predecessors[current]) {
            path.push_back(current);
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

int main() {
    // 创建一个有5个顶点的图
    BellmanFord g(5);

    // 添加边（包含负权边）
    g.addEdge(0, 1, -1);
    g.addEdge(0, 2, 4);
    g.addEdge(1, 2, 3);
    g.addEdge(1, 3, 2);
    g.addEdge(1, 4, 2);
    g.addEdge(3, 2, 5);
    g.addEdge(3, 1, 1);
    g.addEdge(4, 3, -3);

    // 计算从顶点0到所有其他顶点的最短路径
    auto [distances, _] = g.bellmanFord(0);

    if (distances.empty()) {
        cout << "图中存在负环！" << endl;
        return 0;
    }

    cout << "从顶点0到各顶点的最短距离：" << endl;
    for (int i = 0; i < distances.size(); i++) {
        cout << "到顶点" << i << "的最短距离: " << distances[i] << endl;
    }

    // 获取从顶点0到顶点2的具体路径
    vector<int> path = g.getShortestPath(0, 2);
    cout << "\n从顶点0到顶点2的最短路径：" << endl;
    if (path.empty()) {
        cout << "不存在路径或存在负环" << endl;
    } else {
        for (int i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    }

    return 0;
}
