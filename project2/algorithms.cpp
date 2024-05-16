#include <unordered_set>
//----------------------------------------------T2.1----------------------------------------


// Utility function to perform backtracking for TSP
double tsp_backtracking_iteration(Graph<int> g, Vertex<int> *curr, std::vector<int> &path, double cost, double &min_cost, std::vector<int> &best_path) {
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

    //get next node on path
    for (Edge<int>* edge : curr->getAdj()) {
        //cout << "current edge: " << edge->getOrig()->getInfo() << "-" << edge->getDest()->getInfo() << endl;
        Vertex<int>* next = edge->getDest();
        int index = next->getInfo() - g.getVertexSet()[0]->getInfo();
        if (!next->isVisited()) {
            next->setVisited(true);
            path.push_back(next->getInfo());
            tsp_backtracking_iteration(g, next, path, cost + edge->getWeight(), min_cost, best_path);
            path.pop_back();
            next->setVisited(false);
        }
        
    }

    return min_cost;
}

// Function to start the TSP backtracking
double tsp_backtracking(Graph<int> g, const int start, std::vector<int> &best_path) {
    Vertex<int>* startVertex = g.findVertex(start);
    if (!startVertex) return INF;  // Return a large value if start vertex is not found

    startVertex->setVisited(true);

    std::vector<int> path;
    double min_cost = INF;

    path.push_back(start);
    tsp_backtracking_iteration(g, startVertex, path, 0, min_cost, best_path);

    return min_cost;
}



//----------------------------------------------T2.2----------------------------------------


//this is the prim algorithm for distance datasets (as opposed to coordinate datasets)
double primUsingEdges(Graph<int> g, int start, vector<Edge<int>*> &mstEdges) {
    for (auto v : g.getVertexSet()) {
        v->setDist(INF);  //distance
        v->setPath(nullptr);
        v->setVisited(false);
    }

    auto s = g.findVertex(start);
    if (s == nullptr) return 0;

    s->setDist(0);  //distance
    MutablePriorityQueue<Vertex<int>> queue;
    std::unordered_set<Vertex<int>*> inQueue; //keeps track of what's in queue 

    queue.insert(s);
    inQueue.insert(s);

    double totalWeight = 0;


    int numNodes = g.getNumVertex();
    int numVisitedNodes = 0;
    Vertex<int>* lastNode;


    while (!queue.empty()) {
        Vertex<int>* v = queue.extractMin(); //smallest distance vertex in queue
        inQueue.erase(v);
        v->setVisited(true);
        lastNode = v;

        if (v->getPath() != nullptr) { //if vertex has path, its on the mst
            mstEdges.push_back(v->getPath());
            totalWeight += v->getPath()->getWeight();
        }

        for (auto e : v->getAdj()) {
            auto w = e->getDest();
            if (!w->isVisited() && e->getWeight() < w->getDist()) { //if there are multiple nodes pointing to w,
                                                                    //w will get the dist value of the lowest weight edge
                w->setDist(e->getWeight());
                w->setPath(e);

                if (inQueue.find(w) != inQueue.end()) {
                    queue.decreaseKey(w);
                } else {
                    queue.insert(w);
                    inQueue.insert(w);
                }
            }
        }
    }

    //from last node go back to zero if possible


    return totalWeight;
}