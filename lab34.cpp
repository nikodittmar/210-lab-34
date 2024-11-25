#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <string>
#include <algorithm> // For std::sort
#include <functional> // For std::function
using namespace std;

const int SIZE = 9;
const int INF = numeric_limits<int>::max();

struct Edge {
    int src, dest, capacity;
};

typedef pair<int, int> Pair;

class Graph {
public:
    vector<vector<Pair>> adjList;

    Graph(vector<Edge> const &edges) {
        adjList.resize(SIZE);

        for (auto &edge : edges) {
            int src = edge.src;
            int dest = edge.dest;
            int capacity = edge.capacity;

            adjList[src].push_back(make_pair(dest, capacity));
            adjList[dest].push_back(make_pair(src, capacity)); // Undirected graph
        }
    }

    void printGraph(const vector<string> &cityNames) {
        cout << "Power Grid Topology:\n";
        cout << "================================\n";
        for (int i = 0; i < adjList.size(); i++) {
            cout << cityNames[i] << " connects to:\n";
            for (Pair v : adjList[i]) {
                cout << "  → " << cityNames[v.first] << " (Line Capacity: " << v.second << " amps)\n";
            }
        }
        cout << endl;
    }

    void DFS(int start, const vector<string> &cityNames) {
        vector<bool> visited(SIZE, false);
        stack<int> stack;

        stack.push(start);

        cout << "Network Trace (DFS) from " << cityNames[start] << ":\n";
        cout << "Purpose: Identifying potential power flow paths\n";
        cout << "================================\n";

        while (!stack.empty()) {
            int v = stack.top();
            stack.pop();

            if (!visited[v]) {
                cout << "Inspecting " << cityNames[v] << "\n";
                visited[v] = true;

                for (auto it = adjList[v].begin(); it != adjList[v].end(); ++it) {
                    if (!visited[it->first]) {
                        cout << "  → Potential flow to " << cityNames[it->first]
                             << " - Capacity: " << it->second << " amps\n";
                        stack.push(it->first);
                    }
                }
            }
        }
        cout << endl;
    }

    void BFS(int start, const vector<string> &cityNames) {
        vector<bool> visited(SIZE, false);
        queue<int> queue;

        visited[start] = true;
        queue.push(start);

        cout << "Layer-by-Layer Network Inspection (BFS) from " << cityNames[start] << ":\n";
        cout << "Purpose: Analyzing power distribution reach by layers\n";
        cout << "================================\n";

        while (!queue.empty()) {
            int v = queue.front();
            queue.pop();

            cout << "Checking " << cityNames[v] << "\n";

            for (auto &neighbor : adjList[v]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    cout << "  → Next area: " << cityNames[neighbor.first]
                         << " - Capacity: " << neighbor.second << " amps\n";
                    queue.push(neighbor.first);
                }
            }
        }
        cout << endl;
    }

    void dijkstra(int start) {
        vector<int> dist(SIZE, INF);
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (auto &neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        // Output the shortest paths
        cout << "Shortest path from node " << start << ":\n";
        for (int i = 0; i < SIZE; i++) {
            if (dist[i] == INF)
                cout << start << " -> " << i << " : INF\n";
            else
                cout << start << " -> " << i << " : " << dist[i] << "\n";
        }
        cout << endl;
    }

    void kruskalMST(const vector<string> &cityNames) {
        vector<Edge> allEdges;
        vector<Edge> mstEdges;

        // Collect all edges
        for (int i = 0; i < adjList.size(); i++) {
            for (auto &neighbor : adjList[i]) {
                if (i < neighbor.first) { // Avoid duplicate edges in an undirected graph
                    allEdges.push_back({i, neighbor.first, neighbor.second});
                }
            }
        }

        // Sort edges by increasing capacity
        sort(allEdges.begin(), allEdges.end(), [](Edge a, Edge b) {
            return a.capacity < b.capacity;
        });

        // Union-Find structures
        vector<int> parent(SIZE);
        vector<int> rank(SIZE, 0);

        // Initialize parent pointers
        for (int i = 0; i < SIZE; i++) {
            parent[i] = i;
        }

        // Lambda function for finding the root of a set
        std::function<int(int)> find = [&](int v) -> int {
            if (v != parent[v]) {
                parent[v] = find(parent[v]); // Path compression
            }
            return parent[v];
        };

        // Lambda function for union of two sets
        auto unionSets = [&](int u, int v) {
            int rootU = find(u);
            int rootV = find(v);

            if (rootU != rootV) {
                if (rank[rootU] > rank[rootV]) {
                    parent[rootV] = rootU;
                } else if (rank[rootU] < rank[rootV]) {
                    parent[rootU] = rootV;
                } else {
                    parent[rootV] = rootU;
                    rank[rootU]++;
                }
            }
        };

        // Process edges for MST
        for (auto &edge : allEdges) {
            int u = edge.src;
            int v = edge.dest;

            if (find(u) != find(v)) {
                mstEdges.push_back(edge);
                unionSets(u, v);
            }
        }

        // Output MST edges
        cout << "Minimum Spanning Tree edges:\n";
        cout << "================================\n";
        for (auto &edge : mstEdges) {
            cout << "Edge from " << cityNames[edge.src] << " to " << cityNames[edge.dest]
                 << " with capacity: " << edge.capacity << " amps\n";
        }
        cout << endl;
    }
};

int main() {
    vector<Edge> edges = {
        {0, 1, 8},
        {0, 2, 21},
        {1, 2, 6},
        {1, 3, 5},
        {1, 4, 4},
        {2, 7, 11},
        {2, 8, 8},
        {3, 4, 9},
        {6, 5, 10},
        {7, 5, 15},
        {5, 8, 5},
        {6, 7, 3},
        {6, 8, 7},
    };

    vector<string> cityNames = {
        "City A (Power Plant)",
        "City B (Hub 1)",
        "City C (Hub 2)",
        "City D (Substation 1)",
        "City E (Substation 2)",
        "City F (Distribution Center)",
        "City G (Switchyard)",
        "City H (Substation 3)",
        "City I (Generator)"
    };

    Graph graph(edges);

    while (true) {
        cout << "Power Grid Network Menu:\n";
        cout << "[1] Display power grid topology\n";
        cout << "[2] Check Power Distribution Reach (BFS)\n";
        cout << "[3] Plan inspection route  (DFS)\n";
        cout << "[4] Calculate shortest paths\n";
        cout << "[5] Find Minimum Spanning Tree\n";
        cout << "[0] Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                graph.printGraph(cityNames);
                break;
            case 2:
                graph.BFS(0, cityNames); // Assuming starting from node 0
                break;
            case 3:
                graph.DFS(0, cityNames); // Assuming starting from node 0
                break;
            case 4:
                graph.dijkstra(0); // Assuming starting from node 0
                break;
            case 5:
                graph.kruskalMST(cityNames);
                break;
            case 0:
                cout << "Exiting program. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
