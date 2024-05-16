#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include <filesystem>
#include <cstdio>
#include <chrono>
#include <functional>
#include <iostream>
#include "./data_structures/Graph.h"
#include "./utils.cpp"
#include "./algorithms.cpp"
using namespace std;

int NUM_OPTIONS = 6;

void menu_option_1(){
    cout << "chosen option 1" << endl;

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

void menu_option_3(){
    cout << "chosen option 3" << endl;
}

void menu_option_4(){

    cout << "chosen option 4" << endl;
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


        int userInput;

        while (true) {
        cout << "Input choice: ";
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
        
        switch(userInput){
            case 1: menu_option_1(); break;
            case 2: menu_option_2(g); break;
            case 3: menu_option_3(); break;
            case 4: menu_option_4(); break;
            case 5: menu_option_5(); break;
            case 6: return;
            default: break;
        }
    }
}

//measures the execution time of a function.
//it passes a function and its arguments as arguments (mind blown)
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
   
void exampleFunction(int a, int b) {
    // Simulate a time-consuming task
    for (volatile int i = 0; i < a * b; ++i);
}

int main() {

    Graph<int> g;
    
    //populate_graph(g, "datasets/Extra_Fully_Connected_Graphs/edges_25.csv");
    populate_graph(g, "datasets/Toy-Graphs/shipping.csv");
    printGraph(g);

    double result;

    vector<int> best_path;
    double min_cost;
    double executionTime = measureExecutionTime(tsp_backtracking, min_cost,g,0,best_path);

    cout << "best path: ";
    for(int path_point: best_path){
        cout << path_point << "-";
    }
    cout << "\tprecise distance: " << min_cost << endl;
    cout << "execution time: " << executionTime << "s" << endl;

    vector<Edge<int>*> mstEdges;
    
    executionTime = measureExecutionTime(primUsingEdges, result, g, 0, mstEdges);

    for(auto edge: mstEdges){
        cout << edge->getOrig()->getInfo() << "-" << edge->getDest()->getInfo() << "\tcost: " << edge->getWeight() << endl;
    }

    cout << "approximate distance: " << result << endl;
    cout << "execution time: " << executionTime << "s" << endl;



    //menu(g);

    return 0;
}