//TODO: everything .-.

vector<int> tsp_backtracking(Graph<int> &g, int currentNode, vector<int> bestPath, int& minCost){

    vector<int> path;

    backtrack(g, 0, g.getNumVertex(), 0, path);

    return bestPath;
}

int backtrack(g, int currentNode, int numNodes, int currentCost, vector<int> path){

    cout << "current node: " << currentNode << endl;

    if(currentNode < numNodes - 1)
        backtrack(g, numNodes, currentNode + 1, bestPath, minCost);

}