#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include <filesystem>
#include <cstdio>
#include <random>
#include <chrono>
#include <functional>
#include <iostream>
#include "./data_structures/Graph.h"
#include "./utils.cpp"
#include "./algorithms.cpp"
using namespace std;

int NUM_OPTIONS = 6;

int userInput(){
    int userInput;
    while (true) {
        cout << endl << "Input choice: ";
        if (!(cin >> userInput)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (userInput < 1 || userInput > NUM_OPTIONS) {
            cout << "Invalid input. Please enter a number between 1 and " << NUM_OPTIONS << "." << endl;
        } else {
            break;
        }
    }
    return userInput;
}

void toy_menu(Graph<int> &g){
    cout << "1) Shipping dataset" << endl;
    cout << "2) Stadiums dataset" << endl;
    cout << "3) Tourism dataset" << endl;
    cout << endl << "4) Back" << endl;

    string dataset = "datasets/Toy-Graphs/";

    int input = userInput();   
    switch(input){
        case 1: dataset += "shipping.csv"; break;
        case 2: dataset += "stadiums.csv"; break;
        case 3: dataset += "tourism.csv"; break;
        case 4: return;
        default: break;
    }

    if(g.getNumVertex() != 0) g = Graph<int>();
    populate_graph(g, dataset);

}

void extra_fully_menu(Graph<int> &g){
    cout << "1) edges_25.csv" << endl;
    cout << "2) edges_50.csv" << endl;
    cout << "3) edges_75.csv" << endl;
    cout << "4) edges_100.csv" << endl;
    cout << "5) edges_200.csv" << endl;
    cout << "6) edges_300.csv" << endl;
    cout << "7) edges_400.csv" << endl;
    cout << "8) edges_500.csv" << endl;
    cout << "9) edges_600.csv" << endl;
    cout << "10) edges_700.csv" << endl;
    cout << "11) edges_800.csv" << endl;
    cout << "12) edges_900.csv" << endl;
    
    cout << endl << "13) Back" << endl;

    string datasetRoot = "datasets/Extra_Fully_Connected_Graphs/";
    string edges_csv;

    int input = userInput();   

    switch(input){
        case 1: edges_csv = "edges_25.csv"; break;
        case 2: edges_csv = "edges_50.csv"; break;
        case 3: edges_csv = "edges_75.csv"; break;
        case 4: edges_csv = "edges_100.csv"; break;
        case 5: edges_csv = "edges_200.csv"; break;
        case 6: edges_csv = "edges_300.csv"; break;
        case 7: edges_csv = "edges_400.csv"; break;
        case 8: edges_csv = "edges_500.csv"; break;
        case 9: edges_csv = "edges_600.csv"; break;
        case 10: edges_csv = "edges_700.csv"; break;
        case 11: edges_csv = "edges_800.csv"; break;
        case 12: edges_csv = "edges_900.csv"; break;
        case 13: return;
        default: break;
    }
    if(g.getNumVertex() != 0) g = Graph<int>();
    populate_graph_real_world(g, datasetRoot + "nodes.csv", datasetRoot + edges_csv);

}

void real_world_menu(Graph<int> &g){
    cout << "1) graph1" << endl;
    cout << "2) graph2" << endl;
    cout << "3) graph3" << endl;
    cout << endl << "4) Back" << endl;

    int input = userInput();

    string datasetRoot = "datasets/Real-world Graphs/graph";
        
    switch(input){
        case 1: datasetRoot += "1/"; break;
        case 2: datasetRoot += "2/"; break;
        case 3: datasetRoot += "3/"; break;
        case 4: return;
        default: break;
    }

    populate_graph_real_world(g, datasetRoot + "nodes.csv", datasetRoot + "edges.csv");

}

void menu_option_1(Graph<int> &g){
    cout << "1) Toy Graphs" << endl;
    cout << "2) Extra Fully Connected Graphs" << endl;
    cout << "3) Real World Graphs" << endl;
    cout << endl << "4) Back" << endl;

    int input = userInput();
        
    switch(input){
        case 1: toy_menu(g); break;
        case 2: extra_fully_menu(g); break;
        case 3: real_world_menu(g); break;
        case 4: return;
        default: break;
    }
}

void menu_option_2(Graph<int> &g){

    cout << "tsp backtracking" << endl;

    vector<int> best_path;

    auto min_cost = tsp_backtracking(g, 0, best_path);

    cout << "best path: ";
    for(int path_point: best_path){
        cout << path_point;
    }
    cout << "\tcost: " << min_cost << endl;
}

void menu_option_3(Graph<int> &g){
    T2_2(g);
}

void menu_option_4(Graph<int> &g){

    cout << "chosen option 4" << endl;

    T2_3(g);
}

void menu_option_5(){

    cout << "chosen option 5" << endl;
}


template <class T>
void menu(Graph<T> &g){

    while(true){

        cout << endl << endl << "menu:" << endl;
        
        cout << "\t1) Select a dataset (T1.2)" << endl;
        cout << "\t2) Backtracking Algorithm (T2.1)" << endl;
        cout << "\t3) Triangular Approximation Heuristic (T2.2)" << endl;
        cout << "\t4) Other Heuristics??????????? (T2.3)" << endl;
        cout << "\t5) TSP in the Real World (T2.4)"<< endl;
        cout << "\t6) exit" << endl;
        cout << endl;


        int input = userInput();
        
        switch(input){
            case 1: menu_option_1(g); break;
            case 2: menu_option_2(g); break;
            case 3: menu_option_3(g); break;
            case 4: menu_option_4(g); break;
            case 5: menu_option_5(); break;
            case 6: return;
            default: break;
        }
    }
}




int main() {

    Graph<int> g;

    //populate_graph_real_world(g,"datasets/Real-world\ Graphs/graph1");

    menu(g);

    return 0;
}