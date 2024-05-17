vector<vector<string>> parseCSV(const string& filename) {
    vector<vector<string>> data;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open the file: " << filename << endl;
        return data;
    }

    string line;
    getline(file, line); //skip first line
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string cell;

        bool inQuotes = false; /*pra tratar das virgulas dentro das aspas*/
        
        
        while (getline(ss, cell, ',')) {
            
            if (cell.empty()) continue;

            if (cell.front() == '"' && cell.back() == '"') {
                cell = cell.substr(1, cell.length() - 2);
                
            } else if (cell.front() == '"') {
                inQuotes = true;
                cell.erase(0, 1);
            } else if (cell.back() == '"') {
                inQuotes = false;
                cell.pop_back();
            }

            if (inQuotes) {
                std::string temp;
                while (std::getline(ss, temp, ',')) {
                    cell+= "," + temp;
                    if (temp.back() == '"' ) {
                        cell.pop_back();
                        inQuotes = false;
                        break;
                    }
                }
            }
            cell.erase(remove(cell.begin(), cell.end(), '"'), cell.end());
            cell.erase(remove(cell.begin(), cell.end(), ','), cell.end());
            row.push_back(cell);
        }

        data.push_back(row);
    }

    file.close();
    return data;
}

template <class T>
string populate_graph(Graph<T> &g, string dataset=""){

    cout << "parsing data..." << endl << endl;

    auto data = parseCSV(dataset);
    
    int nodeIndex = -1;
    //add nodes to graph
    for(auto row: data){

        int source = stoi(row[0]);
        int destination = stoi(row[1]);
        int distance = stoi(row[2]);
        
        //cout << "\t- adding node " << cityData[2] << " (" << cityData[0] << ")" << endl;
        if(g.addVertex(source)) cout << "\t- added node " << source << endl;
        if(g.addVertex(destination)) cout << "\t- added node " << destination << endl;

        g.addBidirectionalEdge(source, destination, distance);

    }
    
    //set flow of edges to zero
    auto verti = g.getVertexSet();
    for(auto v: verti){
        auto edges = v->getAdj();
        for(auto e: edges){
            e->setFlow(0);
        }
    }

    return dataset; 
    
}

template <class T>
string populate_graph_real_world(Graph<T> &g, string dataset=""){

    cout << "parsing data..." << endl << endl;

    auto data = parseCSV(dataset);
    
    int nodeIndex = -1;
    //add nodes to graph
    for(auto row: data){

        int source = stoi(row[0]);
        int destination = stoi(row[1]);
        int distance = stoi(row[2]);
        
        //cout << "\t- adding node " << cityData[2] << " (" << cityData[0] << ")" << endl;
        if(g.addVertex(source)) cout << "\t- added node " << source << endl;
        if(g.addVertex(destination)) cout << "\t- added node " << destination << endl;

        g.addBidirectionalEdge(source, destination, distance);

    }
    
    //set flow of edges to zero
    auto verti = g.getVertexSet();
    for(auto v: verti){
        auto edges = v->getAdj();
        for(auto e: edges){
            e->setFlow(0);
        }
    }

    return dataset; 
    
}


template <class T>
void printGraph(Graph<T> &g){
    cout << endl << "GRAPH: " << endl << endl;
    auto verti = g.getVertexSet();
    for(auto v: verti){
        auto edges = v->getAdj();
        for(auto e: edges){
            cout << "edge: " << e->getOrig()->getInfo() << "-" << e->getDest()->getInfo() << "  \tdistance: " << e->getWeight() <<  endl;
        }
    }
    cout << endl;
}
