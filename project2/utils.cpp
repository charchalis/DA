/**
 * @brief Analisa um arquivo CSV e retorna os dados em forma de vetor de vetores de strings.
 *
 * Esta função lê um arquivo CSV e retorna os dados em forma de vetor de vetores de strings.
 * Cada vetor interno representa uma linha do arquivo CSV, e cada elemento desse vetor representa
 * uma célula naquela linha.
 *
 * @param filename O nome do arquivo CSV a ser analisado.
 * @return Um vetor de vetores de strings representando os dados do arquivo CSV.
 *
 * @complexity O(N*M), onde N é o número de linhas no arquivo CSV e M é o número máximo de células em uma linha,
 * pois a função percorre cada linha do arquivo e divide as células em cada linha.
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

            cout << "c: " << cell << endl;
            
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
            cout << "d: " << cell << endl;
            cell.erase(remove(cell.begin(), cell.end(), '"'), cell.end());
            cell.erase(remove(cell.begin(), cell.end(), ','), cell.end());
            cout << "e: " << cell << endl;
            row.push_back(cell);
        }

        data.push_back(row);
    }

    file.close();
    return data;
}

/**
 * @brief Popula o grafo com dados de um conjunto de dados específico ou solicita ao usuário para escolher um conjunto de dados.
 *
 * Esta função popula o grafo com dados de um conjunto de dados específico. Se o nome do conjunto de dados não for fornecido,
 * a função solicitará ao usuário que escolha entre os conjuntos de dados "big" ou "small". Os conjuntos de dados são esperados
 * estar em arquivos CSV com os seguintes nomes: Cities.csv, Stations.csv, Reservoirs.csv e Pipes.csv.
 *
 * @param g O grafo a ser populado com os dados.
 * @param dataset O nome do conjunto de dados a ser usado. Se não for fornecido, o usuário será solicitado a escolher.
 * @return O nome do conjunto de dados que foi populado no grafo.
 *
 * @tparam T O tipo de dado dos vértices do grafo.
 *
 * @complexity O(N) em que N é o número de linhas nos arquivos CSV de cidades, estações, reservatórios e tubulações.
 */
// template <class T>
// string populate_graph(Graph<T> &g, string dataset=""){

//     cout << "parsing data..." << endl << endl;

    

//     auto cities = parseCSV("./" + dataset + "/Cities.csv");
//     auto stations = parseCSV("./" + dataset + "/Stations.csv");
//     auto reservoirs = parseCSV("./" + dataset + "/Reservoirs.csv");
//     auto pipes = parseCSV("./" + dataset + "/Pipes.csv");
    
//     //add cities to graph
//     for(auto cityData: cities){
//         cout << "\t- adding city " << cityData[2] << " (" << cityData[0] << ")" << endl;
//         City* city = new City(cityData[0], stoi(cityData[1]), cityData[2], stof(cityData[3]), stoi(cityData[4]));
//         if(!g.addVertex(city)) cout << "\t- failed to add " << city->getInfo() << endl;
//     }
    
//     //add stations to graph
//     for(auto stationData: stations){
//         cout << "\t- adding pumping station " << stationData[1] << endl;
//         Station* station = new Station(stoi(stationData[0]), stationData[1]);

//         if(!g.addVertex(station)) cout << "\t- failed to add " << station->getInfo() << endl;
//     }

//     //add reservoirs to graph
//     for(auto reservoirData: reservoirs){
//         cout << "\t- adding reservoir " << reservoirData[3] << " (" << reservoirData[1] << ")" << endl;
//         Reservoir* reservoir = new Reservoir(reservoirData[0], reservoirData[1], stoi(reservoirData[2]), reservoirData[3], stoi(reservoirData[4]));

//         if(!g.addVertex(reservoir)) cout << "\t- failed to add " << reservoir->getInfo() << endl;
//     }
 
//     //add pipes to graph
//     for(auto pipeData: pipes){
        
//         string source = pipeData[0];
//         string destination = pipeData[1];
//         int capacity = stoi(pipeData[2]);
//         bool bidirectional = !(bool)stoi(pipeData[3]);
        
//         cout << "\t -adding " << (bidirectional ? "bi" : "uni") << "directional pipe from " << source << " to " << destination << endl;

//         if(bidirectional) g.addBidirectionalEdge(source, destination, capacity);
//         else g.addEdge(source, destination, capacity);
//     }   
    
//     //set flow of edges to zero
//     auto verti = g.getVertexSet();
//     for(auto v: verti){
//         auto edges = v->getAdj();
//         for(auto e: edges){
//             e->setFlow(0);
//         }
//     }

//     return dataset; 
    
// }

/**
 * @brief Imprime os vértices e as arestas do grafo junto com o fluxo atual em cada aresta.
 *
 * Esta função imprime os vértices e as arestas do grafo, juntamente com o fluxo atual em cada aresta.
 *
 * @param g O grafo a ser impresso.
 *
 * @tparam T O tipo de dado dos vértices do grafo.
 *
 * @complexity O(V + E), onde V é o número de vértices e E é o número de arestas no grafo.
 */
//print edges and corresponding flow
template <class T>
void printGraph(Graph<T> &g){
    cout << endl << "FINAL RESULT: " << endl << endl;
    auto verti = g.getVertexSet();
    for(auto v: verti){
        auto edges = v->getAdj();
        for(auto e: edges){
            cout << "edge: " << e->getOrig()->getInfo() << "-" << e->getDest()->getInfo() << "  \tflow: " << e->getFlow() << "/" << (e->getWeight() == 1000000000 ? "inf" : to_string((int)e->getWeight())) <<  endl;
        }
    }
    cout << endl;
}

/**
 * @brief Verifica se uma cidade é válida em um grafo.
 *
 * Esta função verifica se uma cidade especificada é válida em um grafo. Se a cidade for "all", ela é considerada válida.
 *
 * @param g O grafo onde a cidade será verificada.
 * @param city A cidade a ser verificada.
 *
 * @return true se a cidade for válida ou se for "all", false caso contrário.
 *
 * @tparam T O tipo de dado dos vértices do grafo.
 *
 * @complexity O(V), onde V é o número de vértices no grafo.
 */
template <class T>
bool validCity(Graph<T> &g, string city){
    if(city == "all") return true;

    return g.findVertex(city);
}