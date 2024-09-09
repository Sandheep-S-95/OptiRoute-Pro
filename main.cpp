#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <algorithm>
#include <stdexcept> // For std::runtime_error

using namespace std;

// Define a custom Node class
class Node {
public:
    string name;
    Node(const string& name) : name(name) {}

    // Define equality operator for Node
    bool operator==(const Node& other) const {
        return name == other.name;
    }

    // Overload the less than operator (<)
    bool operator<(const Node& other) const {
        return name < other.name;
    }
};

// Define a custom comparison function for std::map
struct NodeComparison {
    bool operator()(const Node& node1, const Node& node2) const {
        return node1 < node2;
    }
};

class Edge {
public:
    string from;
    string to;
    int weight;

    Edge(const string& _from, const string& _to, int _weight) : from(_from), to(_to), weight(_weight) {}

    // Define a custom comparison operator for Edge (less than)
    bool operator<(const Edge& other) const {
        return weight > other.weight;  // Reverse comparison for priority queue (min-heap behavior)
    }
};

// Function to add an edge to the graph
void addEdge(map<Node, vector<Edge>, NodeComparison> &graph, const Node& from, const Node& to, int weight) {
    graph[from].push_back(Edge(from.name, to.name, weight));
    graph[to].push_back(Edge(to.name, from.name, weight));
}

// Prim's algorithm to find the minimum spanning tree
vector<Edge> prim(map<Node, vector<Edge>, NodeComparison> &graph) {
    vector<Edge> minimumSpanningTree;
    set<Node> visited;
    priority_queue<Edge> pq;

    if (graph.empty()) {
        throw runtime_error("Graph is empty. Cannot perform Prim's algorithm.");
    }

    // Start from the first node in the graph
    Node startLocation = graph.begin()->first;
    visited.insert(startLocation);

    // Add edges of the starting node to the priority queue
    for (const Edge& edge : graph[startLocation]) {
        pq.push(edge);
    }

    while (!pq.empty()) {
        Edge currentEdge = pq.top();
        pq.pop();

        Node currentLocation(currentEdge.to);

        // If the current location has already been visited, skip it
        if (visited.count(currentLocation) > 0) {
            continue;
        }

        // Mark the current location as visited and add the edge to the minimum spanning tree
        visited.insert(currentLocation);
        minimumSpanningTree.push_back(currentEdge);

        // Add edges from the current location to the priority queue
        for (const Edge& edge : graph[currentLocation]) {
            if (visited.count(Node(edge.to)) == 0) {
                pq.push(edge);
            }
        }
    }

    return minimumSpanningTree;
}

// Function to print the path coordinates
void printPathCoordinates(const vector<Edge>& minimumSpanningTree) {
    int totalWeight = 0;
    for (const Edge& edge : minimumSpanningTree) {
        cout << "From: " << edge.from << " To: " << edge.to << " Distance: " << edge.weight << endl;
        totalWeight += edge.weight;
    }
    cout << "Total Minimum Spanning Tree Weight: " << totalWeight << endl;
}

int main() {
    map<Node, vector<Edge>, NodeComparison> graph;

    int numNodes;
    cout << "Enter the number of nodes: ";
    cin >> numNodes;

    if (numNodes <= 0) {
        cerr << "Number of nodes must be greater than 0." << endl;
        return 1;
    }

    vector<Node> nodes;

    cout << "Enter node names:" << endl;
    for (int i = 0; i < numNodes; ++i) {
        string nodeName;
        cin >> nodeName;
        nodes.push_back(Node(nodeName));
        graph[Node(nodeName)]; // Initialize the node in the graph
    }

    cout << "Enter the edges and weights (from to weight):" << endl;
    int numEdges;
    cin >> numEdges;

    if (numEdges < 0) {
        cerr << "Number of edges cannot be negative." << endl;
        return 1;
    }

    for (int i = 0; i < numEdges; ++i) {
        string fromName, toName;
        int weight;
        cin >> fromName >> toName >> weight;

        auto from = find_if(nodes.begin(), nodes.end(), [&fromName](const Node& node) {
            return node.name == fromName;
        });

        auto to = find_if(nodes.begin(), nodes.end(), [&toName](const Node& node) {
            return node.name == toName;
        });

        if (from != nodes.end() && to != nodes.end() && weight > 0) {
            addEdge(graph, *from, *to, weight);
        } else {
            cerr << "Invalid input. Skipping edge." << endl;
        }
    }

    try {
        vector<Edge> mst = prim(graph);
        printPathCoordinates(mst);
    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
