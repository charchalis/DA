#include <unordered_set>
#include <math.h>

const double PI = 3.14159265358979323846;

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
                    cout << path_point << "-";
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


//Auxiliary function that dictates wheter or not values occur/are in a given vector
bool vectorContainsVal(std::vector<int> vec, int val){
    return std::find(vec.begin(), vec.end(), val) != vec.end();
}

//Swap the two values in the vector in order to generate a new neighbor
void swap_values(std::vector<int> original, int pos1, int pos2, std::vector<int> &modified){
    for(auto val : original){

        //Swap the values 
        if(val == pos1){
            val = pos2; 
        }

        if(val == pos2){
            val = pos1; 
        }

        modified.push_back(val); //Save all the changes
    }
      
}

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

//Verifies if a given val exists in the given vector 
bool is_val_in_vector(std::vector<int> vec, int val){
    return std::find(vec.begin(), vec.end(), val) != vec.end();
}


int generates_random_vertex(std::vector<int> swappedVertexes, int pathSize, bool isStartVertex) {
    
    //Seeding and Random Number generation
    std::random_device rd;
    std::mt19937 gen(rd());

    //The vertexes are from 1 to pathSize-2, since the start and end vertex will always be 0 
    //So, for instance a path with 14 vertexes will only have vertexes from 1 to 12
    std::uniform_int_distribution<> dis(1, pathSize - 2); 


    int generatedVertex = dis(gen);

    //Note: If we are generating the Start Vertex we need to make sure that it hasn't been swapped yet
    //Otherwise we do not care. Why? Because we want to generate the max number of neighbors possible
    //So we make sure that our neighboring functions provide us a vast ammount of neighbors 
    //Therefore making the global optimum search as optimized as possible 

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


//Neighboring function: 
//Generates the neighbors of a given path
void generate_neighbors(std::vector<int> path, std::vector<std::vector<int>> &neighbors){

    std::vector<int> swappedVertexes; 
    std::vector<int> modifiedPath; 

    srand(time(NULL)); //Initializes the seed for the "C code like" random number generator 

    bool valid_generated_vertex = false; 

    int temp = -1;

    while(true){

        //The path has NumberOfVertexes+2 elements, since the start element appears twice
        //So we are going to iterate until the swapped_vertexes vector has the same size as the PathVector-2
        if(swappedVertexes.size() == path.size()-2)
            break; 

        //Resets the path in order to start from the original "unmodified" path 
        modifiedPath = path; 

        //Select two random vertexes to swap their order
        int random_vertex_start = generates_random_vertex(swappedVertexes, path.size(), true);
        int random_vertex_end = generates_random_vertex(swappedVertexes, path.size(), false);
        
        temp = -1; 

        //Swap the vertexes values 
        temp = modifiedPath.at(random_vertex_start);
        modifiedPath.at(random_vertex_start) = random_vertex_end; 
        modifiedPath.at(random_vertex_end) = temp; 

        //Update the swapped vertexes vector
        swappedVertexes.push_back(temp);

        //Add the modified path to the neighbors vector
        neighbors.push_back(modifiedPath); 
    }

}

//Since we only have the vertex combination we need to get the edges of the path and the cost of the path
//TO SUM UP: Get the edges of the path, as well as the total cost of the path
void get_path_cost_feasibility(std::vector<int> path, Graph<int> &g, bool &isPathFeasible, double &pathCost){

    bool found_edge = false; 
    
    //Iterates through every vertex in the graph
    for(int it=0; it<path.size(); it++){

        int curr_vertex = path[it]; 

        for(auto edge: g.getVertexSet()[curr_vertex]->getAdj()){

            found_edge = false; //Resets the flag 

            //Is the current edge the edge @ end of the path (vertex 0)-> End 
            if(curr_vertex == 0 && (it == path.size()-1)){
                return; 
            }

            int next_vertex = path[it+1]; //Next Vertex in the Path 

            int edgeStartingPoint = edge->getOrig()->getInfo();
            int edgeDestination = edge->getDest()->getInfo();

            //Neighboring vertexes are linked by an edge where curr_vertex is the starting point 
            if(edgeStartingPoint == curr_vertex && edgeDestination == next_vertex){
                //Add the cost of the edge to the total cost of the path 
                pathCost += edge->getWeight();
                found_edge = true; //There is a edge linking the two vertexes
                break; 
            }

            //Neighboring vertexes are linked by an edge where curr_vertex is the ending point
            if(edgeStartingPoint == next_vertex && edgeDestination == curr_vertex){
                //Add the cost of the edge to the total cost of the path 
                pathCost += edge->getWeight();
                found_edge = true; //There is a edge linking the two vertexes 
                break;
            }

        }

        //Neighboring vertexes are not linked by an edge, so the path is infeasible 
        //There is no point in continuing to process the path, so we will stop the process
        if(found_edge == false){
            isPathFeasible = false;
            return; 
        }

    }

}


//Generates a random path from the graph
//Not known to be feasible
void find_start_path(Graph<int> &g, int start_node, vector<int> &starting_path){

    starting_path.push_back(start_node); //Add the starting node to the path (start of the path)

    for(auto verti : g.getVertexSet()){
        if(verti->getInfo() != start_node){
            starting_path.push_back(verti->getInfo()); //Add the rest of the nodes to the path
        }
    }

    starting_path.push_back(start_node); //Add the starting node to the path (end of the path)
    
}

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

//Check if meets the aspiration criteria
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

void tabu_search(Graph<int> &g, std::vector<int> starting_path, double starting_cost, std::vector<int> &best_path, double &best_cost){

    std::vector<int> current_path = starting_path; 
    std::map<std::vector<int>, int> tabu_list; 
    std::vector<std::vector<int>> neighbors;
    std::map<double, std::vector<int>, std::greater<>> neighborMap;
    double current_cost = 0; 
    int max_iterations = 500;
    int tabu_tenure = 3; 
    int iter_counter = 0; 
    //Responsible for flagging the search for a possible worst solution, who is not tabu or meets the aspiration criteria
    bool find_next_no_tabu = false; 

    best_path = starting_path;
    best_cost = starting_cost; 

    while(iter_counter != max_iterations){

        update_tabu_list(tabu_list); 

        neighbors.clear(); //Resets the neighbors vector
        neighborMap.clear(); //Resets the neighbors and its costs map
        find_next_no_tabu = false; //Reset the flag

        //Generates the neighbors of a given path 
        generate_neighbors(starting_path, neighbors);

        //Get the best neighbor out of all the neighbors generated
        for(auto neighbor : neighbors){

            //Gets neighbor cost and feasibility
            bool isPathFeasible = true; 
            double pathCost = 0; 

            get_path_cost_feasibility(neighbor, g, isPathFeasible, pathCost);

            //Ignore the neighbor if it is not feasible
            if(!isPathFeasible){
                pathCost = -1; 
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
        }

        iter_counter++; 
    }

}

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
        startingPathCost = -1; 

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

//this is the prim algorithm for coordinates datasets
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


//----------------------------------------------T2.4----------------------------------------

struct Edge {
    int to;
    double weight;
};

struct Node {
    int id;
    double lat, lon;
    std::vector<Edge> edges;
};

//  Add Haversine Distance Calculation
const double EARTH_RADIUS = 6371000; 

double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;

    double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
               std::cos(lat1) * std::cos(lat2) *
               std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return EARTH_RADIUS * c;
}

//  Add Graph Connectivity Check
bool isConnected(const std::vector<Node> &nodes, int start) {
    std::unordered_set<int> visited;
    std::queue<int> q;
    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (const Edge &e : nodes[node].edges) {
            if (visited.find(e.to) == visited.end()) {
                visited.insert(e.to);
                q.push(e.to);
            }
        }
    }
    return visited.size() == nodes.size();
}

//  Add Nearest Neighbor Heuristic
std::vector<int> nearestNeighborTSP(const std::vector<Node> &nodes, int start) {
    std::vector<int> tour;
    std::unordered_set<int> visited;
    int current = start;
    tour.push_back(current);
    visited.insert(current);

    while (tour.size() < nodes.size()) {
        double minDist = std::numeric_limits<double>::infinity();
        int nextNode = -1;
        for (const Edge &e : nodes[current].edges) {
            if (visited.find(e.to) == visited.end() && e.weight < minDist) {
                minDist = e.weight;
                nextNode = e.to;
            }
        }
        if (nextNode == -1) {
            // Handle case where no connected node is found
            break;
        }
        current = nextNode;
        tour.push_back(current);
        visited.insert(current);
    }

    return tour;
}

//  Add Main Function for Real-World TSP Solution
std::vector<int> solveRealWorldTSP(std::vector<Node> &nodes, int start) {
    if (!isConnected(nodes, start)) {
        std::cerr << "The graph is not fully connected from the starting point." << std::endl;
        return {};
    }
    return nearestNeighborTSP(nodes, start);
}




