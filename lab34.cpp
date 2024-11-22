#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <string>
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

    graph.printGraph(cityNames);

    graph.DFS(0, cityNames);
    graph.BFS(0, cityNames);

    graph.dijkstra(0);

    return 0;
}
