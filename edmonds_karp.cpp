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

double compute_metrics(Graph<string> g){

    // Get the initial metrics without heuristics, that will aid is in improving the results
    vector<int> differences;
    int totalDifference = 0;
    int numPipes = 0; 
    int maxDifference = 0; 

    auto vertexSet = g.getVertexSet(); 

    for (auto v : vertexSet) {
        for (auto e : v->getAdj()) {
            if(e->getWeight() > 99999) //Ignore edges linking to super sources or super sinks
                continue; 
            int difference = e->getWeight() - e->getFlow();
            totalDifference += difference;
            differences.push_back(difference);
            numPipes++;
            if(difference > maxDifference) 
                maxDifference = difference; 
        }
    }

    double averageDifference = static_cast<double>(totalDifference) / numPipes;
    cout << "Average Difference without balancing: " << averageDifference << endl;
    cout << "Max Difference between Weight and Flow without balancing: " << maxDifference << endl;

    return averageDifference; 
}

    
void T2_3(Graph<string> g){

    vector<string> sources;
    vector<string> destinations;

    //General Graph Setup
    Graph<string> graph = default_graph_setup_general(g, sources, destinations);
    string sink = default_graph_setup_sink(graph, destinations); 

    //Create two copies of the original graph
    Graph<string> g_without_heuristics = graph;

    //Without using heuristics
    edmonds_karp(g_without_heuristics, "source", sink); 
    compute_metrics(g_without_heuristics); 


    //Using heuristics
    //Get the max an min capacity of the edges 
    int maxEdgeCapacity = 0; 
    int minEdgeCapacity = 999999; 
    auto verti = g_without_heuristics.getVertexSet();

    for (auto v : verti) {
        for (auto e : v->getAdj()) {
            if(e->getWeight() > 99999) //Ignore edges linking to super sources or super sinks
                continue; 
            if(e->getWeight() > maxEdgeCapacity)
                maxEdgeCapacity = e->getWeight();
                
            if(e->getWeight() < minEdgeCapacity)
                minEdgeCapacity = e->getWeight(); 
        }
    }

    //Compute n times the edmonds_karp algorithm modifying the max capacity of the vertexes 
    int incremental_factor = 0; // Initialize the incremental factor with a default value

    // Determine the size of the graph
    int graphSize = g.getVertexSet().size();

    // Adjust the incremental factor based on the graph size
    if (graphSize <= 100) {
        incremental_factor = 1; // For small datasets, set a smaller incremental factor
    } else if (graphSize <= 1000) {
        incremental_factor = 10; // For medium-sized datasets, set a moderate incremental factor
    } else {
        incremental_factor = 100; // For large datasets, set a larger incremental factor
    }

    double metrics_arr[maxEdgeCapacity-minEdgeCapacity]; 
    int metrics_cntr = 0; 

    for(int i = minEdgeCapacity; i < maxEdgeCapacity; i=i+incremental_factor){
        //Resets the Graph on each iteration
        Graph<string> gra; 
        populate_graph(gra); 
        //General Graph Setup
        Graph<string> gr = default_graph_setup_general(gra, sources, destinations);
        string sink = default_graph_setup_sink(gr, destinations); 

        //Sets the maximum capacity of the edges to i 
        auto vertexes = gr.getVertexSet();
        for(auto v: vertexes){
            for(auto e: v->getAdj()){
                if(e->getWeight() > 99999) //Ignore edges linking to super sources or super sinks
                    continue; 
                e->setWeight(i); 
            }
        }

        //Compute edmonds karp with the modified edge capacities
        edmonds_karp(gr, "source", sink); 
        //Compute the metrics of the graph
        metrics_arr[metrics_cntr++] = compute_metrics(gr);
    }

    for(int i = 0; i < metrics_cntr; i++){
        cout << "Average Difference with balancing: " << metrics_arr[i] << endl;
    }

    cout << "Fodasi: " << metrics_cntr; 
    
}
