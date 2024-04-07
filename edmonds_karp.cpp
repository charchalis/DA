void edmonds_karp(Graph<string> &graph, string source, string sink){

    cout << endl << "PATHS: " << endl << endl;
    
    while(true){
        map<string,Edge<string> *> discoveryMap; //this will tell the path found by the bfs
        auto bfsResult = graph.bfs(source, discoveryMap); //run bfs
                                                               
        if(!discoveryMap[sink]) break; //if there is no edge to sink it means that the algorithm is done


        //get path
        
        Edge<string>* path_edge = discoveryMap[sink]; //get path's edge to sink

        string path = sink; //this is just to print the path;

        path = path_edge->getOrig()->getInfo() + " -> " + path;

        Vertex<string>* parent_node = path_edge->getOrig(); //get path's parent node of sink
        Vertex<string>* child_node = graph.findVertex(sink); //get sink vertex
        graph.findVertex(parent_node->getInfo())->setPath(path_edge); 

        //find max flow of path
        int maxFlow = path_edge->getWeight() - path_edge->getFlow();
        while(parent_node->getInfo() != source){
            child_node = parent_node;
            path_edge = discoveryMap[parent_node->getInfo()];
            parent_node = path_edge->getOrig();

            path = path_edge->getOrig()->getInfo() + " -> " + path;

            graph.findVertex(parent_node->getInfo())->setPath(path_edge);
            
            int edgeAvailableFlow = path_edge->getWeight() - path_edge->getFlow();

            if(edgeAvailableFlow < maxFlow) maxFlow = edgeAvailableFlow;
        }
        cout << path;
        cout << "\t(flow: " << maxFlow << ")" << endl;


        //set flow of path
        Vertex<string>* node = graph.findVertex(source);
        while(node->getInfo() != sink){
            auto edge = node->getPath();
            edge->setFlow(edge->getFlow() + maxFlow);
            node = edge->getDest();
        }

        //printGraph(graph);

    }

}

//Setup the graph for general use (Without excluding any city)
Graph<string> default_graph_setup_general(Graph<string> g,vector<string> sources, vector<string> &destinations){
   
    //General Graph Setup
    auto verti = g.getVertexSet();

    for(auto vertex: verti){

        //add reservoirs to sources and add cities to destinations
        string vertexCode = vertex->getInfo();
        std :: cout << "INFO: " << vertexCode << endl; 
        char vertexType = vertexCode[0];

        if(vertexType == 'R') sources.push_back(vertexCode);
        else if(vertexType == 'C') destinations.push_back(vertexCode);
        
    }

    Graph<string> graph = g;


    //create a super-source that connects to the sources
    string source_name = "source";
    graph.addVertex(source_name);
    
    for(auto destination_name: sources){
        graph.addEdge(source_name, destination_name, 1000000000);        
    }

    //set flow of new edges to 0
    auto sourceVertexEdges = graph.findVertex("source")->getAdj();
    for(auto e: sourceVertexEdges){
        e->setFlow(0);
    }

    return graph; 
}

string default_graph_setup_sink(Graph<string> &graph, vector<string> destinations){


    //create a super-sink that the sinks connect to.
    string sink_name = "sink";
    graph.addVertex(sink_name);
    
    for(auto s: destinations){
        graph.addEdge(s, sink_name, 1000000000);        
    }
    
    //set flow of new edges that connect to sink to 0
    auto sinkVertexEdges = graph.findVertex("sink")->getIncoming();
    for(auto e: sinkVertexEdges){
        e->setFlow(0);
    }

    return sink_name; 
}


//T2.2
void list_affected_cities(Graph<string> &graph, string city, map<string,double> *pumpingMap) {
    cout << endl << "DEFICITS:" << endl << endl;

    //check if all cities have full incoming edges
    vector<Vertex<string> *> sinks;
    if (city == "all") {
        sinks = graph.getVertexSet();
    } else {
        sinks.push_back(graph.findVertex(city));
    }

    for (auto vertex : sinks) {
        //if vertex is a city check if it has full incoming edges
        if (vertex->getInfo()[0] == 'C') {
            
            float demand = vertex->getDemand();
            double flow = 0;

            auto incoming = vertex->getIncoming();
            for (auto e : incoming) {
                flow += e->getFlow();
            }
            
            if(flow < demand){
                cout << "\tcity: " << vertex->getInfo() << "\tdeficit: " << demand-flow;
            }else continue;

            if(pumpingMap == nullptr){
                cout << endl;
                continue;
            }

            double deficit = demand - flow;

            string name = vertex->getInfo();
            
            auto it = pumpingMap->find(name);
            double extra = deficit;
            if(it != pumpingMap->end()){
                extra -= it->second;
            }
            cout << "(" << (extra > 0 ? "+" : "") << to_string((int)extra) << ")" << endl;

        }
    }
}

void T2_1(Graph<string> &g, string city){

    cout << endl << "-----------------------------------------------------------------------------------" << endl << endl;
    cout << endl << "edmonds karping, having every reservoir as source and having " << (city == "all" ? "every city" : city) << " as sink" << endl << endl;
    
    vector<string> sources;
    vector<string> destinations; 

    Graph<string> graph = default_graph_setup_general(g, sources, destinations);

    if(city != "all"){

        edmonds_karp(graph, "source", city);

    }else{

       string sink = default_graph_setup_sink(graph, destinations);

        edmonds_karp(graph, "source", sink); 

    }

    //print edges and corresponding flow
    printGraph(graph);

    //2.2
    list_affected_cities(graph, city,nullptr);

    return;
}


bool T3_1(Graph<string> g, string waterReservoir){

    if(waterReservoir.empty())
        return false; 

    //Removes the Vertex from the Graph 
    if(g.removeVertex(waterReservoir)) 
        std::cout << "Water Reservatory Removed Succesfully" << std::endl; 
    else{
        std::cout << "Water Reservatory Not Found" << std::endl;
        return false; 
    }
    
    //Setups Edmonds Karp 
    vector<string> sources;
    vector<string> destinations;

    Graph<string> graph = default_graph_setup_general(g, sources, destinations); 
    string sink = default_graph_setup_sink(graph, destinations);    

    //Runs Edmonds Karp on the graph without the water reservoir 
    edmonds_karp(graph, "source", sink);

    //print edges and corresponding flow
    printGraph(graph);

    //Lists the affected cities 
    list_affected_cities(graph, "all",nullptr); 

    return true; 
}

//We will use this to measure the stock deficits of the cities (if there are any)
map<string,double> getStockDeficits(Graph<string> g){

    //Setups Edmonds Karp 
    vector<string> sources;
    vector<string> destinations;

    Graph<string> graph = default_graph_setup_general(g, sources, destinations); 
    string sink = default_graph_setup_sink(graph, destinations);    

    //Runs Edmonds Karp on the graph 
    edmonds_karp(graph, "source", sink);
    list_affected_cities(g,"all", nullptr);

    //Measure the stock deficits of the cities (With all the Pumping stations)
    auto verti = graph.getVertexSet(); 

    map<string, double> pumping_map;

    for (auto vertex : verti) {
        string name = vertex->getInfo();
        if (name[0] == 'C') {
            auto incoming = vertex->getIncoming();

            double flow = 0;
            float demand = vertex->getDemand();
            for (auto e : incoming) {
                flow += e->getFlow();
            }
            //There is a deficit -> It is worth to be added to the map
            if (demand > flow){
                pumping_map.insert({name, demand - flow}); 
            }
        }
    }

    return pumping_map;

}

bool T3_2(Graph<string> g, string pumpingStation){

    if(pumpingStation.empty())
        return false; 
    
    map<string, double> pumping_map = getStockDeficits(g); //Computes the defices of the stock graph
                                                        //And inserts them into the map
                                                        //
    for(auto v: g.getVertexSet()){
        for(auto e: v->getAdj()){
            e->setFlow(0);
        }
        for(auto e: v->getIncoming()){
            e->setFlow(0);
        }
    }

    //Removes the Vertex from the Graph
    if(g.removeVertex(pumpingStation)) 
        std::cout << endl << "Pumping Station " << pumpingStation << " Removed Succesfully" << std::endl; 
    else{
        std::cout << endl << "Pumping Station Not Found" << std::endl;
        return false; 
    } 

    vector<string> sources; 
    vector<string> destinations; 
    
    Graph<string> graph = default_graph_setup_general(g, sources, destinations);
    string sink = default_graph_setup_sink(graph, destinations);  

    //Runs Edmonds Karp on the graph with modified the pumping stations
    edmonds_karp(graph, "source", sink);
    list_affected_cities(g,"all",&pumping_map);

    //Check the one without the pumping station
    auto verti = graph.getVertexSet();

    std::cout << endl << "Cities affected by the removal of the pumping station: " <<  pumpingStation << endl;

    for (auto v : verti) {

        string name = v->getInfo(); 
        if (name[0] == 'C') {
            auto incoming = v->getIncoming();

            float demand = v->getDemand();
            double flow = 0;
            for (auto e : incoming) {
                flow += e->getFlow();
            }
            double deficit = demand - flow;
            if(demand > flow){
                // Element was found and has worse metrics 
                auto it = pumping_map.find(name);
                if(it != pumping_map.end()){
                    double old_deficit = it->second; 
                    if(old_deficit < deficit){
                        double new_deficit = deficit-old_deficit;
                        std::cout << name << ": " << "new deficit after removing " << pumpingStation << " :" << new_deficit << endl; 
                    }
                }
                // Element was not found, therefore a deficit did not exist
                else {
                    std::cout << name << ": " << "new deficit after removing " << pumpingStation << " :" << deficit << endl; 
                }
            } 
        }
    }

    return true; 

}

double compute_metrics(Graph<string> g, int iternum=0){

    // Get the initial metrics without heuristics, that will aid is in improving the results
    vector<int> differences;
    int totalDifference = 0;
    int numPipes = 0; 
    int maxDifference = 0; 
    Edge<string>* criticalEdge = nullptr;

    auto vertexSet = g.getVertexSet(); 

    for (auto v : vertexSet) {
        for (auto e : v->getAdj()) {
            if(e->getWeight() > 99999) //Ignore edges linking to super sources or super sinks
                continue; 
            if(e->getFlow() == 0) //Ignore edges that were not computed during Edmonds Karp, so they have flow = 0 
                continue; 
            int difference = e->getWeight() - e->getFlow();
            totalDifference += difference;
            differences.push_back(difference);
            numPipes++;
            if(difference > maxDifference){
                maxDifference = difference; 
                criticalEdge = e;   
            }
        }
    }

    double averageDifference = static_cast<double>(totalDifference) / numPipes;
    std::cout << "Average Difference after Edmond's Karp iteration number " << iternum << ": " << averageDifference << std::endl;
    std::cout << "Max Difference between Weight and Flow after iteration number " << iternum << ": " << maxDifference << " Occurs on edge -> " << criticalEdge->getOrig()->getInfo() << " to " << criticalEdge->getDest()->getInfo() << std::endl;

    return averageDifference; 
}

map<Edge<string> *, int> get_critical_edges(Graph<string> g, vector<string>& critical_vertexes){

    map<Edge<string> *, int> critical_edges; 
    auto vertexSet = g.getVertexSet(); 

    int maxCapacity = 0; 
    int difference = 0; 
    int maxDifference = 0; 
    Edge<string>* criticalEdge = nullptr;

    for (auto v : vertexSet) {

        //Verifies if vertex has aready been marked as critical
            //If it was then the problems have already been solved for this vertex, so we can skip it (in order to avoid infinite loops)
        int occurences = count(critical_vertexes.begin(), critical_vertexes.end(), v->getInfo());

        if(occurences > 0)
            continue; 

        //Get Vertex Neighbouring Edges
        int neighbors_count = 0; 
        int neighbors_capacity_sum = 0; 
        double averageCapacity = 0; 
        maxDifference = 0; 
        criticalEdge = nullptr;
        vector<Edge<string> *> neighbors;

        for (auto e : v->getAdj()) {
            if(e->getWeight() > 99999) //Ignore edges linking to super sources or super sinks
                continue;
            //Get the Edges with the highest capacity and the highest difference between weight and flow
                //Because if we have a high capacity edge with a high flow input we can't consider it a critical edge
                //However if we have a high capacity with a high difference (very little flow input) we can consider it a critical edge
                //And, therefore, we need to update its capacity to a more suitable value
            difference = e->getWeight() - e->getFlow();
            if(e->getWeight() > maxCapacity && difference > maxDifference){
                criticalEdge = e;
                maxCapacity = e->getWeight();
                maxDifference = difference;
            }
            neighbors_count++; 
            neighbors_capacity_sum += e->getWeight();
            neighbors.push_back(e);
        }

        //Get the second biggest capacity neighbor
        int biggestNeighbor = 0; 
        for(auto e: neighbors){
            if(e == criticalEdge)
                continue;
            else{
                if(e->getWeight() > biggestNeighbor)
                    biggestNeighbor = e->getWeight(); 
            }
        }

        //The vertex has more than one neighboring edges and its value is above the average
        if(neighbors_count > 1){
            averageCapacity = neighbors_capacity_sum / neighbors_count;
            if(static_cast<double>(maxCapacity) > averageCapacity && criticalEdge != nullptr){
                critical_vertexes.push_back(v->getInfo());
                critical_edges[criticalEdge] = biggestNeighbor;
            }
        }
    }

    return critical_edges; 
}


void T2_3(Graph<string> g){


    vector<string> sources;
    vector<string> destinations;
    
    Graph<string> best_graph; 
    double best_avg = 9999999999; 
    int best_iter_num = 0; 

    int iter_num = 0; 

    //General Graph Setup
    Graph<string> g_without_heuristics = default_graph_setup_general(g, sources, destinations);
    string sink = default_graph_setup_sink(g_without_heuristics, destinations); 

    //Without using heuristics
    edmonds_karp(g_without_heuristics, "source", sink); 
    compute_metrics(g_without_heuristics, iter_num); 

    //gets the critical edges, as well as the critical vertexes of the graph after running Edmonds Karp
    map<Edge <string> *, int> critical_edges; 
    vector<string> critical_vertexes;
    critical_edges = get_critical_edges(g_without_heuristics, critical_vertexes);
 
    while(true){

        iter_num++; 
        int start_critical_vertexes = critical_vertexes.size(); 

        //Creates a graph and fills it with the information needed to run Edmonds Karp (creates super source, super sink, etc)
        Graph<string> gr; 
        vector<string> gr_sources;
        vector<string> gr_dest; 
        populate_graph(gr);
        Graph<string> graph = default_graph_setup_general(gr, gr_sources, gr_dest); 
        string gr_sink = default_graph_setup_sink(graph, gr_dest); 

        //Find the critical edges and update the graph with the critical edges
            //In order to find it we have to compare the edge start and finish point
        for (auto it = critical_edges.begin(); it != critical_edges.end(); ++it) {
            //Iterate Through All the edges in the graph
            for(auto v : graph.getVertexSet()){
                for(auto e : v->getAdj()){
                    if(e->getOrig()->getInfo() == it->first->getOrig()->getInfo() && e->getDest()->getInfo() == it->first->getDest()->getInfo()){
                        e->setWeight(it->second);
                    }
                }
            }
        }

        //Runs Edmonds Karp and computes the metrics on the graph with the updated critical edges
        edmonds_karp(graph, "source", gr_sink);
        double avg_diff = compute_metrics(graph, iter_num); 

        if(avg_diff < best_avg){
            best_graph = graph; 
            best_avg = avg_diff;
            best_iter_num = iter_num; 
        }

        //Gets the critical vertexes of the graph with the we have just updated critical edges and performed edmonds karp in
        critical_edges = get_critical_edges(graph, critical_vertexes);

        int end_critical_vertexes = critical_vertexes.size(); 

        std::cout << "Critical Vertexes start: " << start_critical_vertexes << " Critical Vertexes end: " << end_critical_vertexes << std::endl;

        //The critical vertexes vector didn't update which means that we have found all the critical vertexes
        if(start_critical_vertexes == end_critical_vertexes)
            break; 

    }

    //Prints the best graph found
    std::cout << "The best Average Difference was " << best_avg << " and it was found in iteration number " << best_iter_num << std::endl;
    std::cout << "Graph :" << std::endl; 
    printGraph(best_graph);
    std::cout << "Detailed Metrics: " << std::endl; 
    compute_metrics(best_graph, best_iter_num);

    //OBJECTIVE: MINIMIZE THE FLOW VS CAPACITY ON EACH EDGE
    //Smaller Capacity Edges bottleneck the flow of the path
    //Therefore, edges with higher capacity suffering from neighboring edges with lower capacity will have 
    //significant capacity-flow >>>> 0 distribution
    //What can we do:
        //Locate the neighboring edges(Edges that are connected to the same vertex) and compute their difference 
        //After that we update the edge with the most critical (flow vs capacity) difference with the capacity of the edge with the second highest difference
            //By doing this we make sure we are not losing a lot of flow, and we are able to balance out the neighborhood of a node 
        //Then we mark the edge as critical and we add it to the critical edges vector
        //After that we update the visited critical nodes vector with the node that is connected to the critical edge, so 
        //We do not enter in a loop where we try to balance out the same vertex neighborhood multiple times
        //After that we update the critical edge's capacity and run the algorithm again in order to balance out the graph
        //We repeat this process until the critical vertexes vector is "full" or there are no more critical edges to be found
        //Also, this may introduce some stochascity in the results (since we may get different results each time we run the algorithm by modifying 
        //new edges and running Edmonds Karp again (which outputs new results)) 
        //So we need to keep track of our best result found so far, so we choose that one instead of the one in the last iteration

}
