

bool edmonds_karp(Graph<string> &g, string city){

    if(g.findVertex(city)) cout << "edmonds karping without " << city << endl;
    else{
        cout << "city not found" << endl;       
        //return false;
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


    
    cout << endl << "PATHS: " << endl << endl;
    
    while(true){
        map<string,Edge<string> *> discoveryMap; //this will tell the path found by the bfs
        auto bfsResult = graph.bfs(source_name, discoveryMap); //run bfs
                                                               
        if(!discoveryMap["sink"]) break; //if there is no edge to sink it means that the algorithm is done


        //get path
        
        Edge<string>* path_edge = discoveryMap["sink"]; //get path's edge to sink

        string path; //this is just to print the path;
        path = "sink";

        path = path_edge->getOrig()->getInfo() + " -> " + path;

        Vertex<string>* parent_node = path_edge->getOrig(); //get path's parent node of sink
        Vertex<string>* child_node = graph.findVertex("sink"); //get sink
        graph.findVertex(parent_node->getInfo())->setPath(path_edge); 

        //find max flow of path
        int maxFlow = path_edge->getWeight() - path_edge->getFlow();
        while(parent_node->getInfo() != "source"){
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
        Vertex<string>* node = graph.findVertex("source");
        while(node->getInfo() != "sink"){
            auto edge = node->getPath();
            edge->setFlow(edge->getFlow() + maxFlow);
            node = edge->getDest();
        }

        //print graph with flow
        /*verti = graph.getVertexSet();
        for(auto v: verti){
            auto edges = v->getAdj();
            for(auto e: edges){
                cout << e->getOrig()->getInfo() << "-" << e->getDest()->getInfo() << " -> " << e->getFlow() << endl;
            }
        }*/

    }

    //print graph with flow
    cout << endl;
    verti = graph.getVertexSet();
    for(auto v: verti){
        auto edges = v->getAdj();
        for(auto e: edges){
            cout << e->getOrig()->getInfo() << "-" << e->getDest()->getInfo() << " -> " << e->getFlow() << endl;
        }
    }



    
    return true;
}

void edmonds_karp(Graph<string> &g){
    cout << "edmonds karping every city" << endl;

    edmonds_karp(g, "none");
    
}