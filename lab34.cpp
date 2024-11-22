#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

const int SIZE = 9;

struct Edge {
    int src, dest, weight;
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
            int weight = edge.weight;

            adjList[src].push_back(make_pair(dest, weight));
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }

    void DFS(int start) {
        vector<bool> visited(SIZE, false);
        stack<int> stack;

        stack.push(start);

        cout << "DFS starting from vertex " << start << ":" << endl;

        while (!stack.empty()) {
            int v = stack.top();
            stack.pop();

            if (!visited[v]) {
                cout << v << " ";
                visited[v] = true;

                // Push neighbors onto the stack in the same order as they appear in the adjacency list
                for (auto it = adjList[v].begin(); it != adjList[v].end(); ++it) {
                    if (!visited[it->first])
                        stack.push(it->first);
                }
            }
        }
        cout << endl;
    }

    void BFS(int start) {
        vector<bool> visited(SIZE, false);
        queue<int> queue;

        visited[start] = true;
        queue.push(start);

        cout << "BFS starting from vertex " << start << ":" << endl;

        while (!queue.empty()) {
            int v = queue.front();
            queue.pop();

            cout << v << " ";

            for (auto &neighbor : adjList[v]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    queue.push(neighbor.first);
                }
            }
        }
        cout << endl;
    }
};

int main() {
    vector<Edge> edges = {
        {0,1,8},
        {0,2,21},
        {1,2,6},
        {1,3,5},
        {1,4,4},
        {2,7,11},
        {2,8,8},
        {3,4,9},
        {6,5,10},
        {7,5,15},
        {5,8,5},
        {6,7,3},
        {6,8,7},
    };

    Graph graph(edges);

    graph.printGraph();

    graph.DFS(0);
    graph.BFS(0);

    return 0;
}
