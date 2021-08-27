#ifndef SA_H
#define SA_H

#include "Pos.h"

#include <vector>
#include <stack>
#include <unordered_map> 
#include <cmath> 
#include <utility> 
#include <string> 


using namespace std;

class SA
{

public:
    void Search(int numNodes, const unordered_map<int, Pos> & positionMap, const string & mode, double answer, vector<double>&  epochCosts, vector<int> & results, double & distance);
    void setConfigs(double coolingRateConstant, double initialTemp, const string & scalingTechnique);
    
  
private:
    double calculateFitness(vector<int> path, const unordered_map<int, Pos> & positionMap); 
    vector<int> generateInitialPath(int numNodes); 
    double calculatePathDistance(const vector<int> & v, const unordered_map<int, Pos> & positionMap); 
    void getNextPath(vector<int> & nextPath);  
    double decreaseTemp(double currTemp);
    double decreaseTempLinear(double currTemp);
    double decreaseTempProportional(double currTemp);
    
    int numNodesClass;
    double initialTemp;
    string tempScaleChoice;     //eg "linear" or "proportional" 
    double proportionScaleAlpha;
    double linearScaleAlpa; 

};

#endif