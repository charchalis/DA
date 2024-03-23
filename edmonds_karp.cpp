void edmonds_karp(Graph<string> &g){
    cout << "edmonds karping every city" << endl;

    //TODO
}

bool edmonds_karp(Graph<string> &g, string city){

    if(g.findVertex(city)) cout << "edmonds karping " << city << endl;
    else{
        cout << "city not found" << endl;       
        return false;
    } 
    
    //TODO
    
    return true;
}