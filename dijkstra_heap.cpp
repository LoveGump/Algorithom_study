#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>
using namespace std;

class DijkstraHeap {
   private:
    int vertices;
    vector<vector<pair<int, int>>> graph;  // 邻接表存储图，pair<目标顶点, 权重>

   public:
    DijkstraHeap(int v) : vertices(v), graph(v) {}

    /**
     * @brief 添加一条边到图中
     * @param u 起点
     * @param v 终点
     * @param weight 边的权重
     */
    void addEdge(int u, int v, int weight) { graph[u].push_back({v, weight}); }

    /**
     * @brief 使用Dijkstra算法计算从起点到所有顶点的最短路径
     * @param start 起点
     * @return 返回一个pair，包含距离数组和前驱节点数组
     */
    pair<vector<int>, vector<int>> dijkstra(int start) {
        // 初始化距离数组和前驱节点数组
        vector<int> distances(vertices, numeric_limits<int>::max());
        vector<int> predecessors(vertices, -1);
        distances[start] = 0;

        // 优先队列，存储{距离, 顶点}对
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            int currentDistance = pq.top().first;
            int currentVertex = pq.top().second;
            pq.pop();

            // 如果当前距离大于已知距离，跳过
            if (currentDistance > distances[currentVertex]) {
                continue;
            }

            // 遍历当前顶点的所有邻居
            for (const auto& neighbor : graph[currentVertex]) {
                int nextVertex = neighbor.first;
                int weight = neighbor.second;
                int distance = currentDistance + weight;

                // 如果找到更短的路径，更新距离和前驱节点
                if (distance < distances[nextVertex]) {
                    distances[nextVertex] = distance;
                    predecessors[nextVertex] = currentVertex;
                    pq.push({distance, nextVertex});
                }
            }
        }

        return {distances, predecessors};
    }

    /**
     * @brief 获取从起点到终点的最短路径
     * @param start 起点
     * @param end 终点
     * @return 返回一个包含路径顶点的向量
     */
    vector<int> getShortestPath(int start, int end) {
        auto [distances, predecessors] = dijkstra(start);
        vector<int> path;

        if (distances[end] == numeric_limits<int>::max()) {
            return path;  // 没有路径
        }

        // 重建路径
        for (int current = end; current != -1;
             current = predecessors[current]) {
            path.push_back(current);
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

int main() {
    // 创建一个有6个顶点的图
    DijkstraHeap g(6);

    // 添加边
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 4);
    g.addEdge(2, 4, 3);
    g.addEdge(3, 4, 2);
    g.addEdge(3, 5, 1);
    g.addEdge(4, 5, 6);

    // 计算从顶点0到所有其他顶点的最短路径
    auto [distances, _] = g.dijkstra(0);
    cout << "从顶点0到各顶点的最短距离：" << endl;
    for (int i = 0; i < distances.size(); i++) {
        cout << "到顶点" << i << "的最短距离: " << distances[i] << endl;
    }

    // 获取从顶点0到顶点5的具体路径
    vector<int> path = g.getShortestPath(0, 5);
    cout << "\n从顶点0到顶点5的最短路径：" << endl;
    for (int i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;

    return 0;
}
