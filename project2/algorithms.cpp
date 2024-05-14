//TODO: everything .-.

int tsp_backtracking(Graph<int> &g, int numNodes, int currentNode, vector<int> bestPath, int& minCost){

    cout << "current node: " << currentNode << endl;

    

    if(currentNode < numNodes - 1)
        tsp_backtracking(g, numNodes, currentNode + 1, bestPath, minCost);

    return 0;
}