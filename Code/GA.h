#ifndef GA_H
#define GA_H

#include "Pos.h"

#include <vector>
#include <stack>
#include <unordered_map> 
#include <cmath> 
#include <utility> 
#include <string> 


using namespace std;

class GA
{

public:
    void setConfigs(int popSize, double mutationRate, const string & selection, const string & breeding, const string & mutation);    
    void startAlgo(int numNodes, const unordered_map<int, Pos> & positionMap, const string & mode, double answer, vector<double>& epochCosts, vector<int> & results, double & distance);
  
private:
    vector<vector<int>> initialPopulation();
    vector<int> generatePath();
    double calculateFitness(vector<int> path, const unordered_map<int, Pos> & positionMap);
    void fitnessAssignments(const unordered_map<int, Pos> & positionMap);
    double determineFittestCurrent(const unordered_map<int, Pos> & positionMap, vector<int> & ); 
    double calculatePathDistance(const vector<int> & v, const unordered_map<int, Pos> & positionMap);
    void selectMatingPoolElitism(); 
    void selectMatingPoolTournament(); 

    void CrossOverTheParents();
    pair<vector<int>, vector<int>> orderedCrossOver(vector<int> p1, vector<int> p2);
    pair<vector<int>, vector<int>> singleCross(vector<int> p1, vector<int> p2);

    void mutateSwap(int indexOfPathToMutate);
    void mutateInvert(int indexOfPathToMutate);
    void mutate(); 

    static bool sortBySecond(const pair<int, double> &a, const pair<int, double> & b);
    
    
    vector< vector<int>> matingPool; 
    vector<double> fitnesses; 
    vector<vector<int>> currGen;
    vector<vector<int>> nextGeneration;
    int numNodesClass;
    int popSizeClass; 
    double mutationRate; 

    string mutationChoice;
    string selectionChoice;
    string breedingChoice; 

};

#endif