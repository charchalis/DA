//TODO: everything .-.

// Utility function to perform backtracking for TSP
double tsp_backtracking_iteration(Graph<int> g, Vertex<int> *curr, std::vector<bool> &visited, std::vector<int> &path, double cost, double &min_cost, std::vector<int> &best_path) {
    //cout << "current node: " << curr->getInfo() << endl;
    if (path.size() == g.getNumVertex()) {
        for (Edge<int>* edge : curr->getAdj()) {
            if (edge->getDest() == g.getVertexSet()[0]) {  // Check if there's a path back to the start
                double total_cost = cost + edge->getWeight();
                

                //------------for testing. should be taken off???? or nah ????-----------------//
                cout << "path: ";
                for(int path_point: path){
                    cout << path_point ;
                }
                cout << "0\t cost: " << total_cost << endl;
                //------------for testing. should be taken off???? or nah ????-----------------//
                


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
        //cout << "current edge: " << edge->getOrig()->getInfo() << "-" << edge->getDest()->getInfo() << endl;
        Vertex<int>* next = edge->getDest();
        int index = next->getInfo() - g.getVertexSet()[0]->getInfo();
        if (!visited[index]) {
            visited[index] = true;
            path.push_back(next->getInfo());
            tsp_backtracking_iteration(g, next, visited, path, cost + edge->getWeight(), min_cost, best_path);
            path.pop_back();
            visited[index] = false;
        }
        
    }

    return min_cost;
}

// Function to start the TSP backtracking
double tsp_backtracking(Graph<int> g, const int start, std::vector<int> &best_path) {
    Vertex<int>* startVertex = g.findVertex(start);
    if (!startVertex) return INF;  // Return a large value if start vertex is not found

    std::vector<bool> visited(g.getVertexSet().size(), false);
    std::vector<int> path;
    double min_cost = INF;

    visited[startVertex - g.getVertexSet()[0]] = true;
    path.push_back(start);
    tsp_backtracking_iteration(g, startVertex, visited, path, 0, min_cost, best_path);

    return min_cost;
}