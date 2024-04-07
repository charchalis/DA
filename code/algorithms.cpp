/**
 * @brief Executa o algoritmo de Edmonds-Karp para encontrar o fluxo máximo no grafo.
 *
 * Este algoritmo implementa o método de Edmonds-Karp para encontrar o fluxo máximo de uma fonte para um
 * sorvedouro em um grafo. Ele encontra caminhos aumentadores usando uma busca em largura (BFS) e atualiza
 * o fluxo ao longo desses caminhos até que não seja possível encontrar mais caminhos aumentadores.
 *
 * @param graph O grafo no qual o algoritmo será executado. O grafo será modificado para refletir o fluxo máximo.
 * @param source O vértice de origem do fluxo.
 * @param sink O vértice de destino do fluxo.
 *
 * @complexity O(V * E^2), onde V é o número de vértices e E é o número de arestas do grafo, pois o algoritmo
 * executa uma busca em largura (BFS) para encontrar caminhos aumentadores e atualiza o fluxo ao longo desses caminhos.
 */
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

/**
 * @brief Configura o grafo para uso geral sem excluir nenhuma cidade específica.
 *
 * Esta função configura o grafo para uso geral, identificando os reservatórios como fontes e as cidades como destinos.
 * Em seguida, cria um superorigem que se conecta às fontes do grafo.
 *
 * @param g O grafo original a ser configurado.
 * @param sources Um vetor que será preenchido com os códigos das fontes identificadas no grafo.
 * @param destinations Um vetor que será preenchido com os códigos dos destinos identificados no grafo.
 * @return Um novo grafo configurado para uso geral.
 *
 * @complexity O(V + E), onde V é o número de vértices e E é o número de arestas do grafo original, pois a função
 * percorre todos os vértices do grafo original para identificar as fontes e os destinos, adiciona uma superorigem
 * e as arestas correspondentes, e inicializa o fluxo das novas arestas.
 */
//Setup the graph for general use (Without excluding any city)
Graph<string> default_graph_setup_general(Graph<string> g,vector<string> sources, vector<string> &destinations){
   
    //General Graph Setup
    auto verti = g.getVertexSet();

    for(auto vertex: verti){

        //add reservoirs to sources and add cities to destinations
        string vertexCode = vertex->getInfo();
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


/**
 * @brief Configura o grafo adicionando um super-sorvedouro ao qual os destinos se conectam.
 *
 * Esta função cria um super-sorvedouro no grafo fornecido e conecta todos os destinos a ele.
 * Em seguida, inicializa o fluxo das novas arestas que conectam aos destinos ao super-sorvedouro.
 *
 * @param graph O grafo ao qual o super-sorvedouro será adicionado e ao qual os destinos serão conectados.
 * @param destinations Um vetor contendo os códigos dos destinos a serem conectados ao super-sorvedouro.
 * @return O nome do super-sorvedouro adicionado ao grafo.
 *
 * @complexity O(D), onde D é o número de destinos, pois a função percorre os destinos para criar as arestas
 * entre eles e o super-sorvedouro e inicializa o fluxo dessas novas arestas.
 */
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


/**
 * @brief Lista as cidades afetadas pelo déficit de abastecimento de água.
 *
 * Esta função verifica se todas as cidades têm arestas de entrada completas de água.
 * Se a cidade especificada for "all", verifica todas as cidades; caso contrário, verifica apenas a cidade especificada.
 * Em seguida, para cada cidade verificada, calcula o déficit entre a demanda e o fluxo de água recebido.
 * Se o déficit for positivo, indica que a cidade está com falta de água.
 * Além disso, se um mapa de bombeamento for fornecido, ajusta o déficit com base no bombeamento disponível.
 *
 * @param graph O grafo representando a rede de abastecimento de água.
 * @param city A cidade específica a ser verificada, ou "all" para verificar todas as cidades.
 * @param pumpingMap Um ponteiro para um mapa que contém informações sobre o bombeamento de água disponível para cada cidade.
 * Se nenhum mapa for fornecido, isso será tratado como nulo.
 *
 * @complexity O(V + E), onde V é o número de vértices e E é o número de arestas do grafo, pois a função percorre
 * todos os vértices e arestas relevantes para calcular o déficit de cada cidade.
 */
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

/**
 * @brief Executa o algoritmo de Edmonds-Karp em um grafo de rede de abastecimento de água.
 *
 * Esta função executa o algoritmo de Edmonds-Karp no grafo fornecido, configurando os reservatórios como fontes
 * e a cidade especificada (ou todas as cidades, se "all" for fornecido) como o sorvedouro. Em seguida, imprime
 * as arestas do grafo com o fluxo correspondente e lista as cidades afetadas pelo déficit de abastecimento de água.
 *
 * @param g O grafo representando a rede de abastecimento de água.
 * @param city A cidade específica a ser considerada como sorvedouro, ou "all" para considerar todas as cidades.
 *
 * @complexity O(V + E), onde V é o número de vértices e E é o número de arestas do grafo, pois a função chama
 * outras funções como `default_graph_setup_general`, `default_graph_setup_sink`, `edmonds_karp` e `list_affected_cities`,
 * que têm complexidades lineares ou sub-lineares em relação ao número de vértices e arestas do grafo.
 */
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

/**
 * @brief Remove um reservatório de água do grafo e executa o algoritmo de Edmonds-Karp no grafo resultante.
 *
 * Esta função remove o vértice representando o reservatório de água especificado do grafo fornecido.
 * Em seguida, configura o grafo para execução do algoritmo de Edmonds-Karp, considerando os novos vértices
 * fonte e sorvedouro após a remoção do reservatório.
 * Após a execução do algoritmo de Edmonds-Karp, imprime as arestas do grafo com o fluxo correspondente e lista
 * as cidades afetadas pelo déficit de abastecimento de água.
 *
 * @param g O grafo representando a rede de abastecimento de água.
 * @param waterReservoir O nome do reservatório de água a ser removido do grafo.
 * @return Verdadeiro se o reservatório foi removido com sucesso e falso caso contrário.
 *
 * @complexity O(V + E), onde V é o número de vértices e E é o número de arestas do grafo, pois a função executa
 * operações de remoção de vértices que têm complexidade O(V + E), além de chamar outras funções como
 * `default_graph_setup_general`, `default_graph_setup_sink`, `edmonds_karp` e `list_affected_cities`,
 * que têm complexidades lineares ou sub-lineares em relação ao número de vértices e arestas do grafo.
 */
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


/**
 * @brief Mede os déficits de estoque das cidades na rede de abastecimento de água.
 *
 * Esta função configura o grafo fornecido para execução do algoritmo de Edmonds-Karp e, em seguida,
 * executa o algoritmo para calcular o fluxo de água na rede.
 * Em seguida, mede os déficits de estoque de cada cidade com base na demanda e no fluxo de água recebido.
 * Os déficits são armazenados em um mapa que mapeia o nome de cada cidade para seu déficit de estoque.
 *
 * @param g O grafo representando a rede de abastecimento de água.
 * @return Um mapa que mapeia o nome de cada cidade para seu déficit de estoque.
 *
 * @complexity O(V + E), onde V é o número de vértices e E é o número de arestas do grafo, pois a função executa
 * operações de configuração do grafo e executa o algoritmo de Edmonds-Karp, que tem complexidade O(V + E).
 */
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

/**
 * @brief Verifica se a remoção de uma estação de bombeamento afeta o abastecimento de água das cidades na rede.
 *
 * Esta função configura o grafo fornecido para execução do algoritmo de Edmonds-Karp e, em seguida,
 * executa o algoritmo para calcular o fluxo de água na rede.
 * Em seguida, verifica como a remoção da estação de bombeamento especificada afeta o abastecimento de água
 * das cidades na rede.
 * Se a remoção da estação de bombeamento resultar em um aumento do déficit de estoque de qualquer cidade,
 * a função imprime o novo déficit de estoque e retorna verdadeiro. Caso contrário, retorna falso.
 *
 * @param g O grafo representando a rede de abastecimento de água.
 * @param pumpingStation O nome da estação de bombeamento a ser removida da rede.
 * @param pumping_map Um mapa que mapeia o nome de cada cidade para seu déficit de estoque antes da remoção da estação de bombeamento.
 * @return Verdadeiro se a remoção da estação de bombeamento resultar em um aumento do déficit de estoque de qualquer cidade, caso contrário, falso.
 *
 * @complexity O(V + E), onde V é o número de vértices e E é o número de arestas do grafo, pois a função executa
 * operações de configuração do grafo e executa o algoritmo de Edmonds-Karp, que tem complexidade O(V + E).
 */
bool T3_2(Graph<string> g, string pumpingStation, map<string, double> pumping_map){

    bool found_deficit = false; 

    //Makes sure all the flows are set to 0                                            
    for(auto v: g.getVertexSet()){
        for(auto e: v->getAdj()){
            e->setFlow(0);
        }
        for(auto e: v->getIncoming()){
            e->setFlow(0);
        }
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
                        found_deficit = true; 
                    }
                }
                // Element was not found, therefore a deficit did not exist
                else {
                    std::cout << name << ": " << "new deficit after removing " << pumpingStation << " :" << deficit << endl; 
                    found_deficit = true; 
                }
            } 
        }
    }

    return found_deficit; 

}

/**
 * @brief Calcula métricas relacionadas ao grafo, como diferenças entre pesos e fluxos.
 *
 * Esta função calcula métricas relacionadas ao grafo, como a diferença média entre o peso
 * das arestas e o fluxo nelas, bem como a maior diferença encontrada e a aresta correspondente.
 *
 * @param g O grafo para o qual as métricas serão calculadas.
 * @param iternum O número da iteração de Edmonds-Karp (padrão é 0).
 * @return A diferença média entre peso e fluxo das arestas.
 *
 * @complexity O(V * E), onde V é o número de vértices e E é o número de arestas do grafo, pois
 * percorre todas as arestas do grafo uma vez.
 */
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

/**
 * @brief Obtém as arestas críticas do grafo, considerando vértices críticos.
 *
 * Esta função calcula as arestas críticas do grafo com base nos vértices críticos fornecidos.
 * Uma aresta é considerada crítica se sua capacidade for significativamente maior do que a média
 * das capacidades das arestas vizinhas, e se sua capacidade for maior do que a segunda maior
 * capacidade entre as arestas vizinhas.
 *
 * @param g O grafo para o qual as arestas críticas serão obtidas.
 * @param critical_vertexes Os vértices críticos já identificados no grafo.
 * @return Um mapa contendo as arestas críticas e suas capacidades correspondentes.
 *
 * @complexity O(V * E), onde V é o número de vértices e E é o número de arestas do grafo, pois
 * percorre todos os vértices e, para cada vértice, percorre todas as suas arestas adjacentes uma vez.
 */
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

/**
 * @brief Realiza a execução do algoritmo T2_3 para otimização do fluxo no grafo.
 *
 * Este algoritmo utiliza uma abordagem baseada em heurísticas para otimizar o fluxo em um grafo.
 * Ele identifica arestas críticas com base nas diferenças entre capacidade e fluxo, e ajusta
 * as capacidades dessas arestas para melhorar o balanceamento do fluxo.
 *
 * @param g O grafo no qual o algoritmo será executado.
 * @param dataset O conjunto de dados associado ao grafo.
 *
 * @complexity O(V^2 * E * n), onde V é o número de vértices, E é o número de arestas, e n é o número
 * de iterações necessárias até que todas as arestas críticas sejam identificadas e otimizações aplicadas,
 * pois a função executa Edmonds-Karp e computa métricas em cada iteração, além de realizar diversas operações
 * em grafos e vetores.
 */
void T2_3(Graph<string> g, string dataset){


    vector<string> sources;
    vector<string> destinations;
    
    Graph<string> best_graph; 
    double best_avg = 9999999999; 
    int best_iter_num = 0; 

    int iter_num = 0; 

    //General Graph Setup
    Graph<string> graph = default_graph_setup_general(g, sources, destinations);
    string sink = default_graph_setup_sink(graph, destinations); 

    //Without using heuristics
    edmonds_karp(graph, "source", sink); 
    compute_metrics(graph, iter_num); 

    //gets the critical edges, as well as the critical vertexes of the graph after running Edmonds Karp
    map<Edge <string> *, int> critical_edges; 
    vector<string> critical_vertexes;
    critical_edges = get_critical_edges(graph, critical_vertexes);
 
    while(true){

        iter_num++; 
        int start_critical_vertexes = critical_vertexes.size(); 

        //Resets the graph by reseting the flow to its original state (0)
        for(auto v : graph.getVertexSet()){
            for(auto e : v->getAdj()){
                e->setFlow(0);
            }
        }
        

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

        //Runs Edmonds Karp again and computes the metrics on the graph with the updated critical edges
        edmonds_karp(graph, "source", sink);
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


/**
 * @brief Realiza a execução do algoritmo T3_3 para análise do impacto de falhas em pipelines.
 *
 * Este algoritmo analisa o impacto de falhas em pipelines em um grafo de fluxo. Ele identifica cidades
 * afetadas por uma falha em um pipeline específico e calcula o déficit de abastecimento de água resultante.
 *
 * @param g O grafo representando a rede de pipelines.
 * @param dataset O conjunto de dados associado ao grafo.
 *
 * @complexity O(V^3 * E^2), onde V é o número de vértices e E é o número de arestas do grafo, pois
 * realiza uma série de operações em grafos, incluindo a execução de Edmonds-Karp várias vezes com diferentes
 * conjuntos de arestas desativadas.
 */
void T3_3(Graph<string> g, string dataset){

    //Setups the Edmonds Karp of the default graph
    vector<string> sources; 
    vector<string> destinations; 
    Graph<string> graph = default_graph_setup_general(g, sources, destinations);
    string sink = default_graph_setup_sink(graph, destinations);

    //Performs Edmonds Karp without removing any vertexes for metrics purposes
    edmonds_karp(graph, "source", sink);

    vector<string> not_meet_demand;

    //Check which cities do not meet their demand by default
    for(auto v : graph.getVertexSet()){
        if(v->getInfo()[0] == 'C'){
            auto incoming = v->getIncoming();
            float demand = v->getDemand();
            double flow = 0;
            for(auto e: incoming){
                flow += e->getFlow();
            }
            if(flow < demand){
                not_meet_demand.push_back(v->getInfo());
            }
        }
    }


    map<string, string> affected_cities; //Cities Affected by a given pipeline 
    map<string, double> pipe_deficits; //The defices provoked by a pipeline

    //Iterate through every edge 
    for(auto v : graph.getVertexSet()){
        for(auto e: v->getAdj()){

            //Ignore the edges that are connected to the super source or to the super sink
            if(e->getDest()->getInfo() == "sink" || e->getOrig()->getInfo() == "sink" || e->getOrig()->getInfo() == "source" || e->getDest()->getInfo() == "source")
                continue;

            //Reset All Edges Flow 
            for(auto v: graph.getVertexSet()){
                for(auto e: v->getAdj()){
                    e->setFlow(0);
                }
            }
            
            //Temporarily set its capacity to zero (simulate the rupture/failure)
            int original_capacity = e->getWeight();
            e->setWeight(0); 

            //Runs Edmonds Karp for the given "ruptured" edge 
            edmonds_karp(graph, "source", sink);

            //Check what cities got their demand affected (can no longer supply the ammount of water they need)
            for (auto v : graph.getVertexSet()) {

                if (v->getInfo()[0] == 'C') {

                    //If this city already has a deficit, we can skip the next computations
                    //Since this city already has a deficit by default 
                    int occurences = count(not_meet_demand.begin(), not_meet_demand.end(), v->getInfo());
                    if(occurences > 0)
                        continue;   

                    auto incoming = v->getIncoming();
                    float demand = v->getDemand();

                    //If there are no incoming edges, the we can ignore the next computations and jump right into 
                    // saying that deficit = demand
                    if(incoming.empty()){
                        affected_cities.insert({e->getOrig()->getInfo()+"->"+e->getDest()->getInfo(), v->getInfo()});
                        pipe_deficits.insert({e->getOrig()->getInfo()+"->"+e->getDest()->getInfo(), demand}); 
                        continue; 
                    }

                    //Get the city's flow 
                    double flow = 0;
                    for (auto e : incoming) {
                        flow += e->getFlow();
                    }

                    double deficit = demand - flow; 
                    if(deficit > 0){
                        affected_cities.insert({e->getOrig()->getInfo()+"->"+e->getDest()->getInfo(), v->getInfo()});
                        pipe_deficits.insert({e->getOrig()->getInfo()+"->"+e->getDest()->getInfo(), deficit}); 
                    } 
                }
            }

            //Set the edge's capacity back to its original value
            e->setWeight(original_capacity);
        }
    }

    /* 
        SOLUTION OUTPUT
    */

    std::cout << "---------------------- SORTED BY PIPELINE ----------------------" << std::endl;
    //Print the cities a pipeline affects as well as the deficit provoked by the pipeline
    for(auto it = affected_cities.begin(); it != affected_cities.end(); ++it){
        std::cout << "Pipeline: " << it->first << " Affected City: " << it->second << " Deficit: " << pipe_deficits[it->first] << std::endl; 
    }   

    std::cout << "---------------------- SORTED BY CITY ----------------------" << std::endl;
    //Print the pipelins that affect a city 
    for(auto v : graph.getVertexSet()){
        if(v->getInfo()[0] == 'C'){
            string city = v->getInfo();
            for(auto it = affected_cities.begin(); it != affected_cities.end(); ++it){
                if(it->second == city){
                    std::cout << "City: " << city << " Affected by pipeline: " << it->first << std::endl;
                }
            }  
        }
    }
    
}