
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

    //print edges and corresponding flow
    printGraph(graph);
}


void T2_1(Graph<string> &g, string city){

    cout << endl << "-----------------------------------------------------------------------------------" << endl << endl;
    cout << endl << "edmonds karping, having every reservoir as source and having " << (city == "all" ? "every city" : city) << " as sink" << endl << endl;
    
    
    auto verti = g.getVertexSet();
    
    vector<string> sources;
    vector<string> destinations;

    for(auto vertex: verti){

        //add reservoirs to sources and add cities to destinations
        string vertexCode = vertex->getInfo();
        char vertexType = vertexCode[0];

        if(vertexCode == city){
            continue;
        }
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

    if(city != "all"){

        edmonds_karp(graph, "source", city);

    }else{

        //create a super-sink that the sinks connect to.
        string sink_name = "sink";
        graph.addVertex(sink_name);
        
        for(auto s: destinations){
            graph.addEdge(s, sink_name, 1000000000);        
        }
        
        //set flow of new edges to 0
        auto sinkVertexEdges = graph.findVertex("sink")->getIncoming();
        for(auto e: sinkVertexEdges){
            e->setFlow(0);
        }

        edmonds_karp(graph, "source", "sink");

    }

    //T2.2
    cout << endl << "DEFICITS:" << endl << endl;

    //check if all cities have full incoming edges
    
    vector<Vertex<string> *> sinks;
    if(city == "all") sinks = graph.getVertexSet(); 
    else sinks.push_back(g.findVertex(city));

    bool foundDeficit = false;

    for(auto vertex: sinks){

        //if vertex is a city check if has full incoming edges
        if(vertex->getInfo()[0] == 'C'){

            auto incoming = vertex->getIncoming();                

            double deficit = 0;
            for(auto e: incoming){
                double flow = e->getFlow();
                double capacity = e->getWeight();

                deficit += capacity - flow;
            }
            if(deficit > 0){
                foundDeficit = true;
                cout << "\t" << vertex->getInfo() << " deficit: " << deficit << endl;
            }
        }
    }
    if(!foundDeficit) {
        cout << "\tNo deficit found!" << endl;
    }

    return;
}