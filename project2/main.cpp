#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include <filesystem>
#include <cstdio>
#include "./data_structures/Graph.h"
#include "./utils.cpp"
#include "./algorithms.cpp"
using namespace std;

int NUM_OPTIONS = 5;

void menu_option_1(){
    cout << "chosen option 1" << endl;

}

void menu_option_2(){

   cout << "chosen option 2" << endl;

    return;
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



void menu(){

    while(true){

        cout << endl << endl << "menu:" << endl;
        
        cout << "\t1) Backtracking Algorithm (T2.1)" << endl;
        cout << "\t2) Triangular Approximation Heuristic (T2.2)" << endl;
        cout << "\t3) Other Heuristics??????????? (T2.3)" << endl;
        cout << "\t4) TSP in the Real World (T2.4)"<< endl;
        cout << "\t5) exit" << endl;
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
            case 2: menu_option_2(); break;
            case 3: menu_option_3(); break;
            case 4: menu_option_4(); break;
            case 5: return;
            default: break;
        }
    }
}
   

int main() {
    

    menu();

    return 0;
}