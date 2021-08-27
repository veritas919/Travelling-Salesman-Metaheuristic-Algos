// https://www.youtube.com/watch?v=cY4HiiFHO1o William Fiset's Dynamic Programming TSP video 
// https://github.com/williamfiset/Algorithms/blob/master/src/main/java/com/williamfiset/algorithms/graphtheory/TspDynamicProgrammingIterative.java William Fiset's Java Implementation 

#include "TSPDP.h"

#include <cmath> 
#include <iostream> 

// creates and sets up memo table before calling other functions
double TSPDP::solve(int numNodes, const unordered_map<int,Pos> & positionMap, vector<int> & results){
 
    int start = 1;
    int startIndex = start - 1;

    // create a numNodes x 2^(numNodes) 2D memo table 
    double ** memoTable = new double*[numNodes];
    for(int i =0; i < numNodes;i++){
        memoTable[i] = new double[1 << numNodes]; // 1 << numNodes equals 2^numNodes 
    }

    for(int i = 0; i < numNodes;i++)
    {
        for(int j = 0; j < (1<<numNodes);j++)
        {
            memoTable[i][j] = 1000000; // HUGE 
        }
    }

    // add 2-node path costs to memo table. 
    // Note column is the state represented in binary. eg 11 indicates node 1 and 2 are visited. 
    for(int i = 0; i < numNodes; i++){
        
        if(i == startIndex)
        {
            continue;
        }
        
        memoTable[i][(1<<startIndex) | (1<<i)] = calculateDistance(positionMap, startIndex+1, i+1);
        
    }

    //call other functions to find optimal path 
    generatePartialTourCosts(startIndex, numNodes, memoTable, positionMap);
    double lowestCost = findMinCost(startIndex,numNodes,memoTable, positionMap);
    findOptimalTour(results, startIndex, numNodes, memoTable, positionMap);
    return lowestCost; 
    //free allocated memory for memoTable 
    for(int i = 0; i < numNodes; i++){
        delete [] memoTable[i];
    }
    delete [] memoTable; 

} //end setUp

// fills memo table with min distances 
void TSPDP::generatePartialTourCosts(int startIndex, int numNodes, double ** memoTable, const unordered_map<int, Pos>&  positionMap)
{

    // r is number of nodes in a partial tour
    for(int r = 3; r <= numNodes; r++)
    {
        //vector<int> subsetVec; // will hold {0111, 1011, 1101, 1110} if combinations(3,4)
        vector<int> subsetVec = combinations(r,numNodes); 
        for (int binarySubset:subsetVec)
        {
            if (notIn(startIndex, binarySubset))
            {
                continue;
            }
            //next represents index of the next node 
            //next node must be part of the current subset
            for(int next = 0; next < numNodes; next++)
            {
                if(next == startIndex || notIn(next, binarySubset))
                {
                    continue;
                }
                int subsetWithoutNext  = binarySubset ^ (1 << next);   //the subset excluding the next node. 
                                                        //so you can find best tour when next node is not yet in the subset
                double minDistance = 100000;   //HUGE BASICALLY
                //see which end node best optimizes this partial tour. end node can not be start, next, or not be in current subset we are considering. 
                for(int endNode = 0; endNode < numNodes; endNode++)
                {
                    if(endNode == startIndex || endNode == next || notIn (endNode, binarySubset))
                    {
                        continue;
                    }
                    double newDistance = memoTable[endNode][subsetWithoutNext] + calculateDistance(positionMap, endNode +1, next+1);
                    if(newDistance < minDistance) //new best distance -> save! 
                    {
                        minDistance = newDistance;
                    } 
                }
                memoTable[next][binarySubset] = minDistance; // store best possible tour from start to next in memoTable
            }
        }

    }
}
//calls function to generate combos of partial tours given number of nodes visited in partial tour (r) and total number of nodes to visit in full tour
//combos of partial tours would be {0111, 1011, 1101, 1110} if combinations(3,4)
vector<int> TSPDP::combinations(int r, int numNodes)
{
    vector<int> subsets;
    combinations(0,0,r,numNodes,subsets);
    return subsets;
}

// recursively generates combos of partial tours given number of nodes visited in partial tour (r) and total number of nodes to visit in full tour
void TSPDP::combinations(int set, int at, int r, int numNodes, vector<int> & subsetVec)
{
    int elementsLeft = numNodes-at;
    if(elementsLeft < r)
    {
        return;
    }
    if(r==0){
        subsetVec.push_back(set);
    } 
    else{
        for(int i = at; i < numNodes; i=i+1)
        {
            set = set ^ (1<<i); // turn on i'th bit
            combinations(set, i+1, r-1, numNodes, subsetVec);
            set = set ^(1<<i); //backtrack 
        }
    }
}

//checks if i'th bit in subset is 0 
bool TSPDP::notIn(int i, int subset)
{
    return ((1<<i) & subset) ==0;
}

// finds the minimum cost of path that visits every node exactly once and starts and ends at the start node 
double TSPDP::findMinCost(int startIndex, int numNodes,double ** memoTable, const unordered_map<int, Pos>&  positionMap)
{
    int end_state = (1<<numNodes)-1; // numNodes bits set to 1 means path is a Hamiltonian circuit
    double minCost = 100000; // HUGE

    //look at each possible end node and minimize tour cost 
    for(int endNode = 0; endNode < numNodes; endNode++)
    {
        if(endNode == startIndex)
        {
            continue;
        }
        double tourCost = memoTable[endNode][end_state] + calculateDistance(positionMap, endNode +1, startIndex+1); //add cost to go from last node back to start
        if (tourCost < minCost)
        {
            minCost = tourCost; // save minimal cost 
        }
    }
    return minCost;     
}


// work backwards from end state. use memoTable to find optimal path  
void TSPDP::findOptimalTour(vector<int> & results, int startIndex, int numNodes, double** memoTable, const unordered_map<int, Pos>&  positionMap)
{

    int lastIndex = startIndex;
    int state = (1<<numNodes)-1;    //the end state. all nodes visited (1's)
    results.push_back(startIndex+1);

    for(int i = 1; i < numNodes;i++)
    {
        int index = -1;
        for(int j = 0; j < numNodes;j++)
        {
            if(j == startIndex || notIn(j, state))
            {
                continue;
            }
            if(index == -1)
            {
                index = j;
            }
            double previousDistance = memoTable[index][state] + calculateDistance(positionMap, index + 1, lastIndex+1);
            double newDistance = memoTable[j][state] + calculateDistance(positionMap, j+1, lastIndex+1);
            if(newDistance < previousDistance)
            {
                index = j;
            }
        }
        results.push_back(index+1); 
        state = state ^ (1 <<index);
        lastIndex = index;
    }
    results.push_back(startIndex+1); // add start to end to complete Hamiltonian circuit 
}


//calculate the positional distance between two nodes
double TSPDP::calculateDistance(const unordered_map<int, Pos> & positionMap, int node1, int node2)
{

        double x1 = positionMap.at(node1).x;
        double y1 = positionMap.at(node1).y;
        double z1 = positionMap.at(node1).z;

        double x2 = positionMap.at(node2).x;
        double y2 = positionMap.at(node2).y;
        double z2 = positionMap.at(node2).z;

        double d = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
        
    
    return d;
}