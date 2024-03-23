#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include <filesystem>
#include "./data_structures/Graph.h"
using namespace std;

int NUM_OPTIONS = 5;

void edmonds_karp(){
    cout << "edmonds karping every city" << endl;

    //TODO
}

void edmonds_karp(string city){
    cout << "edmonds karping " << city << endl;

    //TODO
}

void menu_option_1(){
    cout << "city(type 'none' if you want every city): ";

    string userInput;
    cin >> userInput;

    if(userInput == "none") edmonds_karp();
    else edmonds_karp(userInput);
}

void menu_option_2(){
    return;
}

void menu_option_3(){
    cout << "reservoir: ";

    string userInput;
    cin >> userInput;

    cout << "removing reservoir "<< userInput << endl;

    //TODO: call function
}

void menu_option_4(){
    
    //TODO: call function

    //can_be_out_of_service() ???

    return;
}

void menu_option_5(){

    //TODO: call function

    //fulcral_pipelines() ???

    return;
}



void menu(){

    cout << "menu:" << endl;
    
    cout << "\t1) maximum amount of water to a/each city (T2.1)" << endl;
    cout << "\t2) can all the water reservoirs supply enough water to all its delivery sites? (T2.2 and T2.3)" << endl;
    cout << "\t3) remove a reservoir and list the affected cities (T3.1)" << endl;
    cout << "\t4) Can any pumping station be temporarily taken out of service without affecting the delivery capacity to all the cities?(T3.2)" << endl;
    cout << "\t5) For each city, determine which pipelines, if ruptured i.e. with a null flow capacity, would make it impossible to deliver the desired amount of water to a given city (T3.3)" << endl;
    cout << endl;


    int userInput;

    while(!userInput || userInput > NUM_OPTIONS){
	
	    cout << "Input choice: ";
	    cin >> userInput;

	    if(!userInput || userInput > NUM_OPTIONS) cout << "invalid input, try again (valid options: 1-" << NUM_OPTIONS << ")" << endl;

	    cin.clear(); // Clear the fail state
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
    }
    
    switch(userInput){
        case 1: menu_option_1(); break;
        case 2: menu_option_2(); break;
        case 3: menu_option_3(); break;
        case 4: menu_option_4(); break;
        case 5: menu_option_5(); break;
    }
}

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

class City: public Vertex<string> {
    private:
        string city;
        int id;
        string code;
        float demand;
        int population;
    public:
	    City(string city, int id, string code, float demand, int population)
            : Vertex(code), city(city), id(id), code(code), demand(demand), population(population){}
    
};

class Station: public Vertex<string> {
    private:
	    int id;
	    string code;
    public:
	    Station(int id, string code)
        : Vertex(code), id(id), code(code) {}
    
};

class Reservoir: public Vertex<string> {
    private:
        string reservoir;
        string city;
        int id;
        string code;
        int maxDelivery;
    public:
	    Reservoir(string reservoir, string city, int id, string code, int maxDelivery)
            : Vertex(code), reservoir(reservoir), city(city), id(id), code(code), maxDelivery(maxDelivery){}
};



template <class T>
void parse_data(Graph<T> &g){

    cout << "parsing data..." << endl;


    auto cities = parseCSV("./Project1DataSetSmall/Cities_Madeira.csv");
    auto stations = parseCSV("./Project1DataSetSmall/Stations_Madeira.csv");
    auto reservoirs = parseCSV("./Project1DataSetSmall/Reservoirs_Madeira.csv");
    auto pipes = parseCSV("./Project1DataSetSmall/Pipes_Madeira.csv");
    
    for(auto cityData: cities){
        City* city = new City(cityData[0], stoi(cityData[1]), cityData[2], stof(cityData[3]), stoi(cityData[4]));
        cout << city->getInfo() << (city->isVisited() ? " " : " not ") << "visited!" << endl;

        if(!g.addVertex(city)) cout << "failed to add " << city->getInfo() << endl;
    }
    
    for(auto stationData: stations){
        Station* station = new Station(stoi(stationData[0]), stationData[1]);
        cout << station->getInfo() << (station->isVisited() ? " " : " not ") << "visited!" << endl;

        if(!g.addVertex(station)) cout << "failed to add " << station->getInfo() << endl;
    }

    for(auto reservoirData: reservoirs){
        Reservoir* reservoir = new Reservoir(reservoirData[0], reservoirData[1], stoi(reservoirData[2]), reservoirData[3], stoi(reservoirData[4]));
        cout << reservoir->getInfo() << (reservoir->isVisited() ? " " : " not ") << "visited!" << endl;

        if(!g.addVertex(reservoir)) cout << "failed to add " << reservoir->getInfo() << endl;
    }
 
    for(auto pipeData: pipes){
        
        string source = pipeData[0];
        string destination = pipeData[1];
        int capacity = stoi(pipeData[2]);
        bool bidirectional = !(bool)stoi(pipeData[3]);

        if(bidirectional) g.addBidirectionalEdge(source, destination, capacity);
        else g.addEdge(source, destination, capacity);
    }   

}
    

int main() {

    Graph<string> g;

    parse_data(g);
    
    auto test = g.getVertexSet();
    
    cout << "test.size: " << test.size() << endl;

    for(auto v: test){
        cout << "vertex " << v->getInfo() << endl;
    }

    //menu();

    return 0;
}
