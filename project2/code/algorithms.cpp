/**
 * @file algorithms.cpp
 * @brief This file contains implementations of TSP algorithms using different techniques/approaches, as well as 
 * the utility functions used in them.
 */

#include <unordered_set>
#include <math.h>

const double PI = 3.14159265358979323846;

//----------------------------------------------T2.1----------------------------------------

/**
 * @brief Utility function to perform backtracking for TSP.
 * 
 * @param g Graph containing the vertices and edges.
 * @param curr Current vertex in the path.
 * @param path Vector containing the current path.
 * @param cost Current cost of the path.
 * @param min_cost Minimum cost found so far.
 * @param best_path Vector containing the best path found.
 * @return double Minimum cost found.
 * 
 * @timecomplexity The time complexity is O(n!), where n is the number of vertices in the graph.
 */
double tsp_backtracking_iteration(Graph<int> g, Vertex<int> *curr, std::vector<int> &path, double cost, double &min_cost, std::vector<int> &best_path) {
    if (path.size() == g.getNumVertex()) {
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

    // Get next node on path
    for (Edge<int>* edge : curr->getAdj()) {
        Vertex<int>* next = edge->getDest();
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

/**
 * @brief Function to start the TSP backtracking.
 * 
 * @param g Graph containing the vertices and edges.
 * @param start Starting vertex for TSP.
 * @param best_path Vector to store the best path found.
 * @return double Minimum cost found.
 * 
 * @timecomplexity The time complexity is O(n!), where n is the number of vertices in the graph.
 */
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

/**
 * @brief Auxiliary function that checks if a vector contains a given value.
 * 
 * @param vec Vector to be checked.
 * @param val Value to be checked for.
 * @return bool True if the vector contains the value, false otherwise.
 * 
 * @timecomplexity The time complexity is O(n), where n is the size of the vector.
 */
bool vectorContainsVal(std::vector<int> vec, int val) {
    return std::find(vec.begin(), vec.end(), val) != vec.end();
}

/**
 * @brief Swap the two values in the vector to generate a new neighbor.
 * 
 * @param original Original vector.
 * @param pos1 Position of the first value.
 * @param pos2 Position of the second value.
 * @param modified Vector to store the modified version.
 * 
 * @timecomplexity The time complexity is O(n), where n is the size of the vector.
 */
void swap_values(std::vector<int> original, int pos1, int pos2, std::vector<int> &modified) {
    for (auto val : original) {
        if (val == pos1) {
            val = pos2;
        }
        if (val == pos2) {
            val = pos1;
        }
        modified.push_back(val);  // Save all the changes
    }
}

//----------------------------------------------T2.3----------------------------------------

/*
*
*   2.3 -> Using MetaHeuristics to find a good but not optimal solution in a much faster time than other approaches
*   We have to make sure that the starting point is the same as the end point so we cannot change those things
*   In a nutshell: Select a path that has the same starting and ending point and then generate neighbors from that path
*   DO NOT LET THE START AND END POINT BE CHANGED AT ALL COSTS !!! 
*   BASICLY: THE OTHER ALGORITHMS MAY NEED SOME CHANGES
*
*/

/* 
*
*   Utility Functions 
*
*/

/**
 * @brief Verifies if a given value exists in the given vector.
 * 
 * @param vec Vector to be checked.
 * @param val Value to be checked for.
 * @return bool True if the vector contains the value, false otherwise.
 * 
 * @timecomplexity The time complexity is O(n), where n is the size of the vector.
 */
bool is_val_in_vector(std::vector<int> vec, int val) {
    return std::find(vec.begin(), vec.end(), val) != vec.end();
}


/**
 * @brief Generates a random unswapped vertex so that it can be swapped using our neighboring function.
 * 
 * @param swappedVertexes Vector of already swapped vertexes.
 * @param pathSize Size of the path.
 * @param isStartVertex Boolean indicating if it is the start vertex.
 * @return int Random vertex generated.
 * 
 * @timecomplexity The time complexity is O(1).
 */
int generates_random_vertex(std::vector<int> swappedVertexes, int pathSize, bool isStartVertex) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, pathSize - 2);

    int generatedVertex = dis(gen);

    if (isStartVertex) {
        while (generatedVertex == 0 || is_val_in_vector(swappedVertexes, generatedVertex)) {
            generatedVertex = dis(gen);
        }
    } else {
        while (generatedVertex == 0) {
            generatedVertex = dis(gen);
        }
    }

    return generatedVertex;
}


/* END OF UTILITY FUNCTIONS */


/**
 * @brief Generates the neighbors of a given path.
 * 
 * @param path Original path.
 * @param neighbors Vector to store the generated neighbors.
 * @param tweakable_param Boolean to tweak the number of neighbors generated.
 * 
 * @timecomplexity The time complexity is O(n^2), where n is the size of the path.
 */
void generate_neighbors(std::vector<int> path, std::vector<std::vector<int>> &neighbors, bool tweakable_param) {
    int max_number_of_neighbors = 100;
    int neighbor_counter = 0;

    std::vector<int> swappedVertexes;
    std::vector<int> modifiedPath;

    srand(time(NULL));

    int temp = -1;

    while (true) {
        if (swappedVertexes.size() == path.size() - 2)
            break;
        if ((neighbor_counter == max_number_of_neighbors) && tweakable_param)
            break;

        modifiedPath = path;

        int random_vertex_start = generates_random_vertex(swappedVertexes, path.size(), true);
        int random_vertex_end = generates_random_vertex(swappedVertexes, path.size(), false);

        temp = -1;

        temp = modifiedPath.at(random_vertex_start);
        modifiedPath.at(random_vertex_start) = random_vertex_end;
        modifiedPath.at(random_vertex_end) = temp;

        swappedVertexes.push_back(temp);

        neighbors.push_back(modifiedPath);

        neighbor_counter++;
    }
}


/**
 * @brief Gets the cost of a given path, as well as its feasibility.
 * 
 * @param path Path to be evaluated.
 * @param g Graph containing the vertices and edges.
 * @param isPathFeasible Boolean indicating if the path is feasible.
 * @param pathCost Cost of the path.
 * 
 * @timecomplexity The time complexity is O(n*m), where n is the number of vertices and m is the number of edges.
 */
void get_path_cost_feasibility(std::vector<int> path, Graph<int> &g, bool &isPathFeasible, double &pathCost) {
    bool found_edge = false;

    for (int it = 0; it < path.size(); it++) {
        int curr_vertex = path[it];

        for (auto edge : g.getVertexSet()[curr_vertex]->getAdj()) {
            found_edge = false;

            if (curr_vertex == 0 && (it == path.size() - 1)) {
                return;
            }

            int next_vertex = path[it + 1];

            int edgeStartingPoint = edge->getOrig()->getInfo();
            int edgeDestination = edge->getDest()->getInfo();

            if (edgeStartingPoint == curr_vertex && edgeDestination == next_vertex) {
                pathCost += edge->getWeight();
                found_edge = true;
                break;
            }

            if (edgeStartingPoint == next_vertex && edgeDestination == curr_vertex) {
                pathCost += edge->getWeight();
                found_edge = true;
                break;
            }
        }

        if (!found_edge) {
            isPathFeasible = false;
            return;
        }
    }
}

/**
 * @brief Generates a random path from the graph. Not known to be feasible, Since we load in the whole graph all the nodes are added to it.
 * But only some edges are loaded based on the dataset. There are some cases, ie. extra fully connected graphs dataset were we only need a small percentege of them.
 * So, in order to avoid generating a path with nodes that are not loaded in, we only load the nodes which have edges assigned.
 * Therefore avoiding the generation of paths with no edges assigned which would break our algorithm since we would no longer have a fully connected graph.
 * 
 * This function ensures that only nodes with edges assigned are included in the generated path,
 * avoiding paths that break the algorithm due to disconnected nodes.
 * 
 * @param g Graph containing the vertices and edges.
 * @param start_node The starting node for the path.
 * @param starting_path Vector to store the generated starting path.
 * 
 * @timecomplexity O(n) where n is the number of vertices in the graph.
 */
void find_start_path(Graph<int> &g, int start_node, vector<int> &starting_path){

    starting_path.push_back(start_node); //Add the starting node to the path (start of the path)

    for(auto verti : g.getVertexSet()){
        if(verti->getInfo() != start_node){
            //Check if the node has edges assigned
            if(verti->getAdj().size() > 0){
                starting_path.push_back(verti->getInfo()); //Add the node to the graph
            }
            //Otherwise don't consider the node to be important for the given dataset
            else 
                continue; 
        }
    }

    starting_path.push_back(start_node); //Add the starting node to the path (end of the path)
    
}

/**
 * @brief Updates the tabu list by decreasing the tenure of each path and removing those with expired tenure.
 * 
 * @param tabu_list The tabu list mapping paths to their remaining tenure.
 * 
 * @timecomplexity O(k) where k is the number of paths in the tabu list.
 */
void update_tabu_list(std::map<std::vector<int>, int> &tabu_list){

    std::vector<std::vector<int>> to_remove;

    for(auto it = tabu_list.begin(); it != tabu_list.end(); it++){
        it->second--; 
        //Tabu tenure expired 
        if(it->second == 0){
            to_remove.push_back(it->first);
        }
    }

    for(auto &path : to_remove){
        tabu_list.erase(path);
    }
}

/**
 * @brief Checks if a given path meets the aspiration criteria in Tabu Search.
 * 
 * @param path The current path.
 * @param tabu_list The list of tabu paths with their tenures.
 * @param tabu_tenure The tenure of the tabu list.
 * @param best_cost The best cost found so far.
 * @return True if the path meets the aspiration criteria, false otherwise.
 * 
 * @timecomplexity O(1)
 */
bool meets_aspiration_criteria(std::vector<int> path, std::map<std::vector<int>, int> tabu_list, int tabu_tenure, double best_cost){

    //The aspiration treshhold is something extra that we can add to the aspiration criteria 
    //To make it slightly "stronger" if we want to 
    //SEE: https://www.linkedin.com/advice/0/what-advantages-disadvantages-using-aspiration
    int aspiration_treshhold = 0; 

    //If the best neighbor is tabu but meets the aspiration criteria 
        //ASPIRATION CRITERIA: THE BEST NEIGHBOR IS TABU BUT IT HAS NOT BEEN INSERTED RECENTLY 
        //AND IT IS BETTER THAN THE CURRENT BEST COST FOUND UNTIL NOW
    if((tabu_list.find(path) != tabu_list.end()) && (tabu_list[path] < tabu_tenure) && (tabu_list[path] < (best_cost+aspiration_treshhold))){
        return true; 
    }

    return false; 
}

/**
 * @brief Performs the Tabu Search algorithm to find the optimal path in a graph.
 * 
 * @param g The graph on which the search is performed.
 * @param starting_path The initial path to start the search.
 * @param starting_cost The cost of the initial path.
 * @param best_path Output parameter to store the best path found.
 * @param best_cost Output parameter to store the cost of the best path found.
 * 
 * @timecomplexity O(iterations * neighbors * (log neighbors + feasibility check))
 */
void tabu_search(Graph<int> &g, std::vector<int> starting_path, double starting_cost, std::vector<int> &best_path, double &best_cost){

    std::vector<int> current_path = starting_path; 
    std::map<std::vector<int>, int> tabu_list; 
    std::vector<std::vector<int>> neighbors;
    std::map<double, std::vector<int>, std::greater<>> neighborMap;
    double current_cost = 0; 
    int tabu_tenure = 3; 
    int iter_counter = 0; 
    //Responsible for flagging the search for a possible worst solution, who is not tabu or meets the aspiration criteria
    bool find_next_no_tabu = false; 

    //Get a random iteration number in order to make the algorithm stronger 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(2, 4);
    int random_multiplier = dist(gen);
    int max_iterations = g.getNumVertex()*random_multiplier; 

    int total_iterations = 0; 
    int total_iterations_best_result = 0; 

    best_path = starting_path;
    best_cost = starting_cost; 

    while(iter_counter != max_iterations){

        std::cout << "Iteration number after reset: " << iter_counter << std::endl;
        std::cout << "Current Best Cost: " << best_cost << std::endl;  
        std::cout << "Best Result Found at Total Iteration Number: " << total_iterations_best_result << std::endl;

        //Best Path 
        std::cout << "Best Path: " << std::endl; 
        for(auto vertex : best_path){
            std::cout << vertex << " "; 
        }
        std::cout << std::endl; 

        update_tabu_list(tabu_list); 

        neighbors.clear(); //Resets the neighbors vector
        neighborMap.clear(); //Resets the neighbors and its costs map
        find_next_no_tabu = false; //Reset the flag

        //Generates the neighbors of a given path 
        generate_neighbors(starting_path, neighbors, false);

        //Get the best neighbor out of all the neighbors generated
        for(auto neighbor : neighbors){

            //Gets neighbor cost and feasibility
            bool isPathFeasible = true; 
            double pathCost = 0; 

            get_path_cost_feasibility(neighbor, g, isPathFeasible, pathCost);

            //Ignore the neighbor if it is not feasible
            if(!isPathFeasible){
                pathCost = INF; 
                continue; 
            }

            //Store them in a map (with values sorted in descending order (std::greater comparison function))
            neighborMap[pathCost] = neighbor;

        }

        //Map is sorted in ascending order so we are going to iterate through the neighbors 
        //Starting at the best and ending at the worst neighbor 
        for(auto neighbor : neighborMap){

            //It is not tabu 
            if(tabu_list.find(neighbor.second) == tabu_list.end()){
                current_path = neighbor.second; 
                current_cost = neighbor.first; 

                //Update the tabu list 
                tabu_list[neighbor.second] = tabu_tenure;
            }
            else{
                //Meets the aspiration criteria 
                if(meets_aspiration_criteria(neighbor.second, tabu_list, tabu_tenure, best_cost)){
                    current_path = neighbor.second; 
                    current_cost = neighbor.first; 

                    //Update the tabu list
                    tabu_list[neighbor.second] = tabu_tenure;
                } 
                //Is Tabu and does not meet aspiration criteria so we will look for another neighbor 
                //Who meets our requirements
                else{
                    continue; 
                }
            }

        }

        //Is our new found path and solution better than the best one found yet?
        //If it is, update it and reset the iteration counter
        if(current_cost < best_cost){
            best_path = current_path; 
            best_cost = current_cost; 
            iter_counter = 0; 
            total_iterations_best_result = total_iterations; 
        }

        iter_counter++; 
        total_iterations++; 
    }

}

/**
 * @brief Function to demonstrate the tabu search algorithm on a graph.
 * 
 * @param g The graph to be used for the demonstration.
 */
void T2_3(Graph<int> &g){

    std::vector<int> starting_path; 
    std::vector<int> best_path; 
    double best_cost; 

    //Generate a random path from the graph in order to have a good starting point for our neighboring function
    find_start_path(g, 0, starting_path); 

    bool isPathFeasible = true;
    double startingPathCost = 0;

    //Calculate the cost and feasiblity of the starting path 
    get_path_cost_feasibility(starting_path, g, isPathFeasible, startingPathCost);

    if(!isPathFeasible)
        startingPathCost = INF; 

    tabu_search(g, starting_path, startingPathCost, best_path, best_cost);

    std::cout << "Best Found Path: " << std::endl; 

    for(auto vertex : best_path){
        std::cout << vertex << " "; 
    }

    std::cout << std::endl; 
    std::cout << "Path Cost: " << best_cost << std::endl;

    return; 
    
}

//----------------------------------------------T2.2----------------------------------------

/**
 * @brief this is the prim algorithm for distance datasets (as opposed to coordinate datasets)
 * 
 * @param g The graph.
 * @param start The starting vertex.
 * @param mstEdges Output parameter to store the MST edges.
 * @return The total weight of the MST.
 * 
 * @timecomplexity O((n + m) log n)
 */
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


    while (!queue.empty()) {
        Vertex<int>* v = queue.extractMin(); //smallest distance vertex in queue
        inQueue.erase(v);
        v->setVisited(true);

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

    return totalWeight;
}

/**
 * @brief Calculates the distance between two vertices using their geographical coordinates.
 * 
 * @param v The first vertex.
 * @param w The second vertex.
 * @return The calculated distance between the two vertices.
 * 
 * @timecomplexity O(1)
 */
double calculateDistance(Vertex<int>* v, Vertex<int>* w){

    double v_longitude = v->getLongitude() * PI / 180;
    double w_longitude = w->getLongitude() * PI / 180;
    double v_latitude = v->getLatitude() * PI / 180;
    double w_latitude = w->getLatitude() * PI / 180;

    double delta_longitude = abs(v_longitude - w_longitude);
    double delta_latitude = abs(v_latitude - w_latitude);

    double a = sin(delta_latitude/2) * sin(delta_latitude/2) + cos(v_latitude)*cos(w_latitude)*sin(delta_longitude/2)*sin(delta_longitude);
    double c = 2*atan2(sqrt(a), sqrt(1-a));
    double d = 6371*1000 * c;

    return d;
}

/**
 * @brief Prim's algorithm for coordinates datasets.
 * 
 * @param g The graph containing the vertices.
 * @param start The starting vertex.
 * @param mstEdges Output parameter to store the MST edges.
 * @return The total weight of the MST.
 * 
 * @timecomplexity O(n^2 log n)
 */
double primUsingCoords(Graph<int> g, int start, vector<Edge<int>*> &mstEdges) {

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


    while (!queue.empty()) {
        Vertex<int>* v = queue.extractMin(); //smallest distance vertex in queue
        inQueue.erase(v);
        v->setVisited(true);

        if (v->getDist() < INF) { //if vertex has path, its on the mst
            totalWeight += v->getDist();
        }

        for (auto w : g.getVertexSet()) {
            if(v->getInfo() == w->getInfo()) continue;
            double distance = calculateDistance(v,w);
            //Edge<int>* proximationEdge = new Edge<int>(v,w,distance);
            if (!w->isVisited() && distance < w->getDist()) { //if there are multiple nodes pointing to w,
                                                                    //w will get the dist value of the lowest weight edge
                w->setDist(distance);
                //w->setPath(proximationEdge);

                if (inQueue.find(w) != inQueue.end()) {
                    queue.decreaseKey(w);
                } else {
                    queue.insert(w);
                    inQueue.insert(w);
                }
            }
        }
    }

    return totalWeight;
}

/**
 * @brief Function to demonstrate Prim's algorithm using both edge weights and vertex coordinates.
 * 
 * @param g The graph to be used for the demonstration.
 */
void T2_2(Graph<int> g){
    double result;

    vector<Edge<int>*> mstEdges2;

    double executionTime = measureExecutionTime(primUsingCoords, result, g, 0, mstEdges2);

    // for(auto edge: mstEdges2){
    //     cout << edge->getOrig()->getInfo() << "-" << edge->getDest()->getInfo() << "\tcost: " << edge->getWeight() << endl;
    // }

    cout << "\nApproximate Triangulation Distance: " << result << endl;
    cout << "execution time: " << executionTime << "s" << endl;

    cout << endl;

    vector<Edge<int>*> mstEdges;

    executionTime = measureExecutionTime(primUsingEdges, result, g, 0, mstEdges);

    // for(auto edge: mstEdges){
    //     cout << edge->getOrig()->getInfo() << "-" << edge->getDest()->getInfo() << "\tcost: " << edge->getWeight() << endl;
    // }

    cout << "MST Distance: " << result << endl;
    cout << "execution time: " << executionTime << "s" << endl;
}