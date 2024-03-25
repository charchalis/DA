void edmonds_karp(Graph<string> &g){
    cout << "edmonds karping every city" << endl;

    //TODO
}

bool edmonds_karp(Graph<string> &g, string city){

    if(g.findVertex(city)) cout << "edmonds karping without " << city << endl;
    else{
        cout << "city not found" << endl;       
        return false;
    } 
    
    
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


    //create a super-sink that the sinks connect to.
    string sink_name = "sink";
    graph.addVertex(sink_name);
    
    for(auto s: destinations){
        graph.addEdge(s, sink_name, 1000000000);        
    }
    

    //set flow of new edges to 0
    auto sourceVertexEdges = graph.findVertex("source")->getAdj();
    for(auto e: sourceVertexEdges){
        e->setFlow(0);
    }
    auto sinkVertexEdges = graph.findVertex("sink")->getIncoming();
    for(auto e: sinkVertexEdges){
        e->setFlow(0);
    }
    
    //temporario. so pra ver as iteracoes
    for(int i = 0; i < 1; i++){


    map<string,Edge<string> *> discoveryMap; //this will tell the path found by the bfs
    auto bfsResult = graph.bfs(source_name, discoveryMap); //run bfs

    vector<Edge<string>*> path;
    
    //get path
    cout << "path: " << endl;

    Edge<string>* path_edge = discoveryMap["sink"];
    cout << "\t" << path_edge->getOrig()->getInfo() << "-" << path_edge->getDest()->getInfo() << endl;
    Vertex<string>* parent_node = path_edge->getOrig();
    Vertex<string>* child_node = graph.findVertex("sink");
    graph.findVertex(parent_node->getInfo())->setPath(path_edge);

    //find max flow of path
    int maxFlow = path_edge->getWeight() - path_edge->getFlow();
    while(parent_node->getInfo() != "source"){

        path.push_back(path_edge);
        child_node = parent_node;
        path_edge = discoveryMap[parent_node->getInfo()];
        parent_node = path_edge->getOrig();

        cout << "\t" << path_edge->getOrig()->getInfo() << "-" << path_edge->getDest()->getInfo() << endl;

        graph.findVertex(parent_node->getInfo())->setPath(path_edge);
        
        int edgeAvailableFlow = path_edge->getWeight() - path_edge->getFlow();

        if(edgeAvailableFlow < maxFlow) maxFlow = edgeAvailableFlow;
    }
    cout << "flow: " << maxFlow << endl;




    
    //set flow of path
    Vertex<string>* node = graph.findVertex("source");
    while(node->getInfo() != "sink"){
        auto edge = node->getPath();
        edge->setFlow(maxFlow);
        node = edge->getDest();
    }

    verti = graph.getVertexSet();
    
    for(auto v: verti){
        auto edges = v->getAdj();
        for(auto e: edges){
            cout << e->getOrig()->getInfo() << "-" << e->getDest()->getInfo() << " -> " << e->getFlow() << endl;
        }
    }

    }

    //TODO
    
    return true;
}