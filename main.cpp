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

        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    file.close();
    return data;
}

void parse_data(Graph<int> g){

    cout << "parsing data..." << endl;


    auto cities = parseCSV("./Project1DataSetSmall/Cities_Madeira.csv");
    auto stations = parseCSV("./Project1DataSetSmall/Stations_Madeira.csv");
    auto reservoirs = parseCSV("./Project1DataSetSmall/Reservoirs_Madeira.csv");
    auto pipes = parseCSV("./Project1DataSetSmall/Pipes_Madeira.csv");
    
    cout << cities[0][0] << endl;

    //TODO:populate graph

}
    

int main() {

    Graph<int> g;

    parse_data(g);

    menu();

    return 0;
}
