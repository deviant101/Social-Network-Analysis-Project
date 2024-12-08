#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <list>
#include <queue>
#include <limits>
#include <algorithm> // Include this header for reverse

using namespace std;

struct Edge {
    int target;
    double weight;
};

class Graph {
public:
    unordered_map<int, list<Edge>> adjList;

    void addEdge(int u, int v, double w) {
        adjList[u].push_back({v, w});
        adjList[v].push_back({u, w});
    }

    void loadGraph(const string& filename) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            int u, v;
            double w;
            if (!(iss >> u >> v >> w)) { break; }
            addEdge(u, v, w);
        }
    }
};

struct Node {
    int id;
    double cost;

    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

pair<vector<int>, double> dijkstra(const Graph& graph, int start, int goal) {
    priority_queue<Node, vector<Node>, greater<Node>> openSet;
    unordered_map<int, double> gScore;
    unordered_map<int, int> cameFrom;

    for (const auto& pair : graph.adjList) {
        gScore[pair.first] = numeric_limits<double>::infinity();
    }
    gScore[start] = 0.0;

    openSet.push({start, 0.0});

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.id == goal) {
            vector<int> path;
            for (int at = goal; at != start; at = cameFrom[at]) {
                path.push_back(at);
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return {path, gScore[goal]};
        }

        for (const Edge& edge : graph.adjList.at(current.id)) {
            double tentative_gScore = gScore[current.id] + edge.weight;
            if (tentative_gScore < gScore[edge.target]) {
                cameFrom[edge.target] = current.id;
                gScore[edge.target] = tentative_gScore;
                openSet.push({edge.target, tentative_gScore});
            }
        }
    }

    return {{}, numeric_limits<double>::infinity()}; // Return an empty path and infinity distance if no path is found
}

int main() {
    Graph graph;
    graph.loadGraph("social-network-proj-graph/social-network-proj-graph.txt");

    int start = 523; // Example start node
    int goal = 5229;  // Example goal node

    auto result = dijkstra(graph, start, goal);
    vector<int> path = result.first;
    double distance = result.second;

    if (!path.empty()) {
        cout << "Shortest path from " << start << " to " << goal << ": ";
        for (int node : path) {
            cout << node << " ";
        }
        cout << endl;
        cout << "Shortest distance: " << distance << endl;
    } else {
        cout << "No path found from " << start << " to " << goal << endl;
    }

    return 0;
}