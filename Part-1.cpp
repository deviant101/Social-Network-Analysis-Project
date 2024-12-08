#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <climits>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Edge {
    int node;
    int weight;
};

// Custom comparator for the priority queue (min-heap)
struct Compare {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second;
    }
};

// Function to calculate the heuristic (number of neighbors)
int heuristic(const unordered_map<int, vector<Edge>>& graph, int node) {
    return graph.at(node).size();
}

// Function to implement A* algorithm
vector<int> a_star(const unordered_map<int, vector<Edge>>& graph, int start, int goal) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, Compare> pq;
    unordered_map<int, int> g_score; // Cost from start to current node
    unordered_map<int, int> came_from; // To reconstruct the path
    set<int> visited;

    for (const auto& entry : graph) {
        g_score[entry.first] = INT_MAX;
    }
    g_score[start] = 0;

    pq.push({start, 0});

    while (!pq.empty()) {
        int current = pq.top().first;
        pq.pop();

        if (current == goal) {
            vector<int> path;
            while (current != start) {
                path.push_back(current);
                current = came_from[current];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        visited.insert(current);

        for (const Edge& edge : graph.at(current)) {
            int neighbor = edge.node;
            if (visited.find(neighbor) != visited.end()) {
                continue;
            }

            int tentative_g_score = g_score[current] + edge.weight;
            if (tentative_g_score < g_score[neighbor]) {
                came_from[neighbor] = current;
                g_score[neighbor] = tentative_g_score;

                int f_score = g_score[neighbor] + heuristic(graph, neighbor);
                pq.push({neighbor, f_score});
            }
        }
    }

    return {};
}

int main() {
    unordered_map<int, vector<Edge>> graph;

    // Reading graph data from "social-network-proj-graph.txt"
    ifstream graphFile("social-network-proj-graph.txt");
    if (!graphFile) {
        cerr << "Error opening graph file." << endl;
        return 1;
    }

    int node1, node2, weight;
    while (graphFile >> node1 >> node2 >> weight) {
        graph[node1].push_back({node2, weight});
        graph[node2].push_back({node1, weight});
    }
    graphFile.close();

    cout << "Enter start and goal nodes: ";
    int start, goal;
    cin >> start >> goal;

    vector<int> path = a_star(graph, start, goal);

    if (!path.empty()) {
        cout << "Path found: ";
        for (int node : path) {
            cout << node << " ";
        }
        cout << endl;
    } else {
        cout << "No path found." << endl;
    }

    return 0;
}
