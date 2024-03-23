#include <string>
#include <iostream>
using namespace std;
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