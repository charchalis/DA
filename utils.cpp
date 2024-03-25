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
void populate_graph(Graph<T> &g){

    cout << "parsing data..." << endl << endl;

    auto cities = parseCSV("./Project1DataSetSmall/Cities_Madeira.csv");
    auto stations = parseCSV("./Project1DataSetSmall/Stations_Madeira.csv");
    auto reservoirs = parseCSV("./Project1DataSetSmall/Reservoirs_Madeira.csv");
    auto pipes = parseCSV("./Project1DataSetSmall/Pipes_Madeira.csv");
    
    //add cities to graph
    for(auto cityData: cities){
        cout << "\t- adding city " << cityData[2] << " (" << cityData[0] << ")" << endl;
        City* city = new City(cityData[0], stoi(cityData[1]), cityData[2], stof(cityData[3]), stoi(cityData[4]));
        if(!g.addVertex(city)) cout << "\t- failed to add " << city->getInfo() << endl;
    }
    
    //add stations to graph
    for(auto stationData: stations){
        cout << "\t- adding pumping station " << stationData[1] << endl;
        Station* station = new Station(stoi(stationData[0]), stationData[1]);

        if(!g.addVertex(station)) cout << "\t- failed to add " << station->getInfo() << endl;
    }

    //add reservoirs to graph
    for(auto reservoirData: reservoirs){
        cout << "\t- adding reservoir " << reservoirData[3] << " (" << reservoirData[1] << ")" << endl;
        Reservoir* reservoir = new Reservoir(reservoirData[0], reservoirData[1], stoi(reservoirData[2]), reservoirData[3], stoi(reservoirData[4]));

        if(!g.addVertex(reservoir)) cout << "\t- failed to add " << reservoir->getInfo() << endl;
    }
 
    //add pipes to graph
    for(auto pipeData: pipes){
        
        string source = pipeData[0];
        string destination = pipeData[1];
        int capacity = stoi(pipeData[2]);
        bool bidirectional = !(bool)stoi(pipeData[3]);
        
        cout << "\t -adding " << (bidirectional ? "bi" : "uni") << "directional pipe from " << source << " to " << destination << endl;

        if(bidirectional) g.addBidirectionalEdge(source, destination, capacity);
        else g.addEdge(source, destination, capacity);
    }   
    
    //set flow of edges to zero
    auto verti = g.getVertexSet();
    for(auto v: verti){
        auto edges = v->getAdj();
        for(auto e: edges){
            e->setFlow(0);
        }
    }

}