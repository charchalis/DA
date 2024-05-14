//TODO: everything .-.

// Utility function to perform backtracking for TSP
double tsp_backtracking_util(Graph<int> g, Vertex<int> *curr, std::vector<bool> &visited, std::vector<int> &path, double cost, double &min_cost, std::vector<int> &best_path) {
    cout << "current node: " << curr->getInfo() << endl;
    if (path.size() == g.getVertexSet().size()) {
        for (Edge<int>* edge : curr->getAdj()) {
            if (edge->getDest() == g.getVertexSet()[0]) {  // Check if there's a path back to the start
                double total_cost = cost + edge->getWeight();
                if (total_cost < min_cost) {
                    min_cost = total_cost;
                    best_path = path;
                    best_path.push_back(edge->getDest()->getInfo());  // Add the start node to complete the cycle
                }
                break;
            }
        }
        return min_cost;
    }

    for (Edge<int>* edge : curr->getAdj()) {
        cout << "current edge: " << edge->getOrig()->getInfo() << "-" << edge->getDest()->getInfo() << endl;
        Vertex<int>* next = edge->getDest();
        int index = next->getInfo() - g.getVertexSet()[0]->getInfo();
        cout << "index: " << index << endl;
        if (!visited[index]) {
            cout << "not visited" << endl;
            visited[index] = true;
            path.push_back(next->getInfo());
            tsp_backtracking_util(g, next, visited, path, cost + edge->getWeight(), min_cost, best_path);
            path.pop_back();
            visited[index] = false;
        }else{
            cout << "visited" << endl;
        }
        //cout << "popo" << endl;
    }

    return min_cost;
}

// Function to start the TSP backtracking
double tsp_backtracking(Graph<int> g, const int start, std::vector<int> &best_path) {
    Vertex<int>* startVertex = g.findVertex(start);
    if (!startVertex) return INF;  // Return a large value if start vertex is not found

    cout << "start vertex: " << startVertex->getInfo() << endl;

    std::vector<bool> visited(g.getVertexSet().size(), false);
    std::vector<int> path;
    double min_cost = INF;

    visited[startVertex - g.getVertexSet()[0]] = true;
    path.push_back(start);
    tsp_backtracking_util(g, startVertex, visited, path, 0, min_cost, best_path);

    return min_cost;
}