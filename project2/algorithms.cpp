//TODO: everything .-.

// Utility function to perform backtracking for TSP
double tsp_backtracking_util(Graph<int> g, Vertex<int> *curr, std::vector<bool> &visited, int count, double cost, double &min_cost) {
    if (count == g.getVertexSet().size()) {
        for (Edge<int>* edge : curr->getAdj()) {
            if (edge->getDest() == g.getVertexSet()[0]) {  // Check if there's a path back to the start
                min_cost = std::min(min_cost, cost + edge->getWeight());
                break;
            }
        }
        return min_cost;
    }

    for (Edge<int>* edge : curr->getAdj()) {
        Vertex<int>* next = edge->getDest();
        int index = next - g.getVertexSet()[0];
        if (!visited[index]) {
            visited[index] = true;
            tsp_backtracking_util(g, next, visited, count + 1, cost + edge->getWeight(), min_cost);
            visited[index] = false;
        }
    }

    return min_cost;
}

double tsp_backtracking(Graph<int> g, const int &start) {
    Vertex<int>* startVertex = g.findVertex(start);
    if (!startVertex) return INF;  // Return a large value if start vertex is not found

    std::vector<bool> visited(g.getVertexSet().size(), false);
    double min_cost = INF;

    visited[startVertex - g.getVertexSet()[0]] = true;
    tsp_backtracking_util(g, startVertex, visited, 1, 0, min_cost);

    return min_cost;
}

