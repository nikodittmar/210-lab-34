#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
using namespace std;

const int SIZE = 9;

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
            adjList[dest].push_back(make_pair(src, capacity));
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
};

int main() {
    vector<Edge> edges = {
        {0, 1, 800},
        {0, 2, 2100},
        {1, 2, 600},
        {1, 3, 500},
        {1, 4, 400},
        {2, 7, 1100},
        {2, 8, 800},
        {3, 4, 900},
        {6, 5, 1000},
        {7, 5, 1500},
        {5, 8, 500},
        {6, 7, 300},
        {6, 8, 700},
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

    return 0;
}
