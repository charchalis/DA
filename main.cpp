#include <iostream>
#include <string>
#include <limits>
using namespace std;

int NUM_OPTIONS = 5;


void menu_option_1(){
    cout << "city(type 'none' if you want every city): ";

    string userInput;
    cin >> userInput;

    if(userInput == "none") edmonds_karp();
    else edmonds_karp(userInput)
}

void menu_option_2(){return;}
void menu_option_3(){return;}
void menu_option_4(){return;}
void menu_option_5(){return;}



void menu(){

    cout << "menu:" << endl;
    
    cout << "\t1) maximum amount of water to a/each city (T2.1)" << endl;
    cout << "\t2) can all the water reservoirs supply enough water to all its delivery sites? (T2.2 and T2.3)" << endl;
    cout << "\t3) remove a reservoir and list the affected cities (T3.1)" << endl;
    cout << "\t4) Can any pumping station be temporarily taken out of service without affecting the delivery capacity to all the cities?(T3.2)" << endl;
    cout << "\t5) For each city, determine which pipelines, if ruptured i.e. with a null flow capacity, would make it impossible to deliver the desired amount of water to a given city. (T3.3)" << endl;
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
    

int main() {

    menu();

        return 0;
}
