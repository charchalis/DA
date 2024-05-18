
/**
 * @file utils.cpp
 * @brief This file contains utility functions for the project. Namely graph population and csv file parsing.
 * There are also functions used to compute the execution time of a function and to clear a graph.
 */


/**
 * @brief Parses a CSV file and returns the data as a 2D vector of strings.
 * 
 * @param filename The name of the CSV file to parse.
 * @return A 2D vector of strings containing the parsed data.
 * 
 * @timecomplexity O(n * m), where n is the number of lines and m is the average number of cells per line.
 */
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

/**
 * @brief Populates a graph with nodes and edges from a dataset.
 * 
 * @param g The graph to be populated.
 * @param dataset The dataset file containing nodes and edges information.
 * @return The name of the dataset file.
 * 
 * @timecomplexity O(n), where n is the number of rows in the dataset.
 */
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
        
        if(g.addVertex(source)) cout << "\t- added node " << source << endl;
        if(g.addVertex(destination)) cout << "\t- added node " << destination << endl;

        if(g.addBidirectionalEdge(source, destination, distance)) cout << "\t- added edge " << source << "-" << destination << " (" << distance << ")" << endl;
        
    }
    

    return dataset; 
    
}

/**
 * @brief Populates a graph with real-world data from nodes and edges datasets.
 * 
 * @param g The graph to be populated.
 * @param nodesDataset The dataset file containing nodes information.
 * @param edgesDataset The dataset file containing edges information.
 * 
 * @timecomplexity O(n + m), where n is the number of nodes and m is the number of edges.
 */
template <class T>
void populate_graph_real_world(Graph<T> &g, string nodesDataset="", string edgesDataset=""){

    cout << "parsing data..." << endl << endl;

    auto data = parseCSV(nodesDataset);
    
    int nodeIndex = -1;
    //add nodes to graph
    for(auto row: data){

        int id = stoi(row[0]);
        double longitude = stod(row[1]);
        double latitude = stod(row[2]);
        
        //cout << "\t- adding node " << cityData[2] << " (" << cityData[0] << ")" << endl;
        if(g.addVertex(id)){
            cout << "\t- added node " << id << "(" << longitude << ", " << latitude <<  ")" << endl;
            auto v = g.findVertex(id);
            v->setLongitude(longitude);
            v->setLatitude(latitude);
        }
    }

    data = parseCSV(edgesDataset);

    for(auto row: data){
        int source = stoi(row[0]);
        int destination = stoi(row[1]);
        double weight = stod(row[2]);

        g.addBidirectionalEdge(source, destination, weight);

        cout << "\t- added edge " << source << "-" << destination << " (" << weight <<  ")" << endl;
    }
    
    
}

/**
 * @brief Prints the graph's edges and their weights.
 * 
 * @param g The graph to be printed.
 * 
 * @timecomplexity O(n + m), where n is the number of nodes and m is the number of edges.
 */
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

/**
 * @brief Measures the execution time of a function.
 * 
 * @param func The function to measure.
 * @param result The result of the function.
 * @param args The arguments to pass to the function.
 * @return The duration in seconds.
 * 
 * @timecomplexity O(t), where t is the time complexity of the function being measured.
 */
template<typename Func, typename... Args>
double measureExecutionTime(Func&& func, double &result, Args&&... args) {
    // Record the start time
    auto start = std::chrono::high_resolution_clock::now();

    // Call the function passed as argument with provided arguments
    result = std::forward<Func>(func)(std::forward<Args>(args)...);

    // Record the end time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> duration = end - start;

    // Return the duration in seconds
    return duration.count();
}

/**
 * @brief Clears the graph by removing all vertices and edges.
 * 
 * @param g The graph to be cleared.
 * 
 * @timecomplexity O(n + m), where n is the number of nodes and m is the number of edges.
 */
void clearGraph(Graph<int> &g){
    for(auto v: g.getVertexSet()){
        for(auto e: v->getAdj()){
            v->removeEdge(e->getDest()->getInfo());
        }
        g.removeVertex(v->getInfo());
    }
}
