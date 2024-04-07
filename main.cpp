#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include <filesystem>
#include "./data_structures/Graph.h"
#include "./data_structures/Vertex_children.h"
#include "./utils.cpp"
#include "./algorithms.cpp"
using namespace std;

int NUM_OPTIONS = 5;

Graph<string> g;

void menu_option_1(){
    cout << "city code(type 'all' if you want every city): ";

    string userInput;
    cin >> userInput;

    while(!validCity(g, userInput)){
        cout << "invalid city. Try again" << endl;
        cout << "city code(type 'all' if you want every city): ";
        cin >> userInput;
    }
    T2_1(g,userInput);
}

void menu_option_2(string dataset){

   T2_3(g, dataset); 

    return;
}

void menu_option_3(){
    std::cout << "reservoir: ";

    std::string userInput;
    std::cin >> userInput;

    std::cout << "removing reservoir "<< userInput << std::endl;

    if(!T3_1(g, userInput)){
        menu_option_3(); //retry
    }
}

void menu_option_4(string dataset){

    //Computes the defices of the stock graph 
    //And inserts them into the map
    map<string, double> pumping_map = getStockDeficits(g); 

    vector<string> critical_pumping_stations; 
    vector<string> non_critical_pumping_stations;
                                                        
    for(auto v : g.getVertexSet()){
        //Filter the pumping stations
        if(v->getInfo()[0] == 'P' && v->getInfo()[1]=='S'){
            //Creates a new graph (no copies of the original graph are made, so we make sure we are not modifying the original graph
            //as we are going to remove the pumping station mid iteration)
            Graph<string> g_new; 
            populate_graph(g_new, dataset);
            //Removes the pumping station
            if(g_new.removeVertex(v->getInfo()))
                std::cout << endl << "Pumping Station " << v->getInfo() << " Removed Succesfully" << std::endl; 
            else{
                std::cout << "There was an error processing the given Pumping Station ... Skipping" << std::endl; 
                continue;   
            }
            //Runs the algorithm in order to check for deficits
            if(T3_2(g_new, v->getInfo(), pumping_map))
                critical_pumping_stations.push_back(v->getInfo());
            else
                non_critical_pumping_stations.push_back(v->getInfo());
        }
    }

    std::cout << "Critical Pumping Station (Cannot be Removed): " << std::endl;
    for(auto v : critical_pumping_stations){
        std::cout << "Pumping Station: " << v << std::endl; 
    }

    std::cout << "Non-Critical Pumping Station (Can be Removed): " << std::endl; 
    for(auto v : non_critical_pumping_stations){
        std::cout << "Pumping Station: " << v << std::endl; 
    }
    
    return;
}

void menu_option_5(string dataset){

    T3_3(g, dataset);

    return;
}



void menu(string dataset){

    cout << "menu:" << endl;
    
    cout << "\t1) maximum amount of water to a/each city (T2.1) and cities deficit (T2.2)" << endl;
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
        case 2: menu_option_2(dataset); break;
        case 3: menu_option_3(); break;
        case 4: menu_option_4(dataset); break;
        case 5: menu_option_5(dataset); break;
    }
}
   

int main() {

    string dataset = populate_graph(g); //populate graph
    
    menu(dataset);

    return 0;
}