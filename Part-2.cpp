#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Edge {
    int node;
};

// Graph and influence scores
unordered_map<int, vector<Edge>> graph;
unordered_map<int, int> influenceScores;

// DP memoization table
unordered_map<int, pair<int, vector<int>>> memo;

// Recursive DP function to find the longest increasing path
pair<int, vector<int>> longestIncreasingPath(int node) {
    if (memo.find(node) != memo.end()) {
        return memo[node];
    }

    int maxLength = 1;  // Minimum length is 1 (the node itself)
    vector<int> maxPath = {node};

    for (const Edge& edge : graph[node]) {
        int neighbor = edge.node;

        // Only consider paths with increasing influence scores
        if (influenceScores[neighbor] > influenceScores[node]) {
            pair<int, vector<int>> result = longestIncreasingPath(neighbor);

            // Update maxLength and maxPath if a longer path is found
            if (result.first + 1 > maxLength) {
                maxLength = result.first + 1;
                maxPath = {node};
                maxPath.insert(maxPath.end(), result.second.begin(), result.second.end());
            }
        }
    }

    // Store the result in the memoization table
    memo[node] = {maxLength, maxPath};
    return memo[node];
}

int main() {
    // Reading graph data from "social-network-proj-graph.txt"
    ifstream graphFile("./social-network-proj-graph/social-network-proj-graph.txt");
    if (!graphFile) {
        cerr << "Error opening graph file." << endl;
        return 1;
    }

    int node1, node2, weight;
    while (graphFile >> node1 >> node2 >> weight) {
        graph[node1].push_back({node2});
        graph[node2].push_back({node1});  // Undirected graph
    }
    graphFile.close();

    // Reading influence scores from "social-network-proj-Influences.txt"
    ifstream influenceFile("./social-network-proj-graph/social-network-proj-Influences.txt");
    if (!influenceFile) {
        cerr << "Error opening influence file." << endl;
        return 1;
    }

    int node, score;
    while (influenceFile >> node >> score) {
        influenceScores[node] = score;
    }
    influenceFile.close();

    // Find the longest increasing path for each node
    int globalMaxLength = 0;
    vector<int> globalMaxPath;

    for (const auto& entry : graph) {
        int startNode = entry.first;
        pair<int, vector<int>> result = longestIncreasingPath(startNode);

        // Update the global maximum
        if (result.first > globalMaxLength) {
            globalMaxLength = result.first;
            globalMaxPath = result.second;
        }
    }

    // Output the results
    cout << "Maximum length of the chain: " << globalMaxLength << endl;
    cout << "Sequence of users in the chain: ";
    for (int user : globalMaxPath) {
        cout << user << " ";
    }
    cout << endl;

    return 0;
}
