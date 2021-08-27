// https://www.youtube.com/watch?v=eBmU1ONJ-os   watched this video for reference 
#include "SA.h"
#include <vector>
#include <unordered_map>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <cmath> 
#include <iostream> 
#include <utility>
#include <limits> 
#include <algorithm> 
#include <string>

// set the configurations user specified 
void SA::setConfigs(double coolingRateConstant, double initialTemp, const string & scalingTechnique)
{
    if(scalingTechnique == "linear")
    {
        linearScaleAlpa = coolingRateConstant;
    }
    else if (scalingTechnique == "proportional")
    {
        proportionScaleAlpha = coolingRateConstant;
    }
    this->initialTemp = initialTemp;
    tempScaleChoice = scalingTechnique;
}

// start algorithm 
void SA::Search(int numNodes, const unordered_map<int, Pos> & positionMap, const string & mode, double answer, vector<double>&  epochCosts, vector<int> & results, double & distance)
{
    numNodesClass = numNodes; 
    const double EulerConstant = std::exp(1.0); // get e value 
    vector<int> initialPath = generateInitialPath(numNodes);
    double currTemp = initialTemp;
    vector<int> currPath(initialPath);
    double currDistMin = 1000000; 

    int epoch = 0;
    double currDist = calculatePathDistance(currPath,positionMap);
    
        while(currTemp > 0 && epoch < 35000)
        {
            // if mode is timing and distance within 5% of answer, consider answer found 
            if( ( abs(currDist - answer) /answer) < 0.05 && mode == "NodeTiming")
                return; 
           
            double currentEnergy = calculateFitness(currPath, positionMap);
            currDist = calculatePathDistance(currPath,positionMap);
            // keep track of best solution and associated distance over multiple epochs 
            if(currDist < currDistMin)
            {
                currDistMin = currDist;
                results = currPath;
                distance = currDist; 

            }
            if(mode == "LC")
            {
                epochCosts.push_back(currDist); // save cost at each epoch 
            }                
            if(epoch % 5000 == 0 && mode == "LC"){
                cout << "epoch: " << epoch << " current distance: " << currDist<<endl;   // print progress to console 
            }  
            vector<int> nextPath (currPath); 
            getNextPath(nextPath); // modifies nextPath so it is modified from currPath 
            double nextPathEnergy = calculateFitness(nextPath, positionMap);
            double energyDifference = nextPathEnergy - currentEnergy; // calculate energy diff between current and next path 
            
            double rando = ((double) rand() / (RAND_MAX)); //generate number between 0 and 1
            if(energyDifference > 0)
            {
                currPath = nextPath;    // make current path next path if next path is better 
            }

            else if( pow(EulerConstant, (energyDifference / currTemp)) > rando)
            {
                    currPath = nextPath; // with a certain probability, set current path equal to a worse next path 
            } 
            epoch++;
            currTemp = decreaseTemp(currTemp); // decrease the temperature 
        }
}

// decrease the temperature appropriately based on scaling method user selected 
double SA::decreaseTemp(double currTemp)
{
    double nextTemp = 0.0;
    if(tempScaleChoice == "linear")
    {
        nextTemp = decreaseTempLinear(currTemp);
    }

    else if(tempScaleChoice == "proportional")
    {
        nextTemp = decreaseTempProportional(currTemp);
    }
    return nextTemp;
}

// decreases the temperature linearly 
double SA::decreaseTempLinear(double currTemp)
{
    double nextTemp = currTemp - linearScaleAlpa;
    return nextTemp;
}

// decrease the temperature proportionally 
double SA::decreaseTempProportional(double currTemp)
{
    double nextTemp = currTemp * (1-proportionScaleAlpha);
    return nextTemp;
}

//generate a path provided the numNodes in the path. 
vector<int> SA::generateInitialPath(int numNodes)
{
    vector<int> orderedMiddleOfPath;
    orderedMiddleOfPath.push_back(1); //start is always 1 
    for (int i = 2; i <= numNodes; i++)
    {
        orderedMiddleOfPath.push_back(i);  
    } 
 
    // do numNodes swaps on each path 
    // CAN ALTER THE NUMBER OF SWAPS TO VARY PERFORMANCE OF ALGO 
    for(int i = 0; i < numNodes;i++)
    {
        int index1 = (rand() % (orderedMiddleOfPath.size()-1)) +1; 
        int temp = orderedMiddleOfPath.at(index1) ;
        int index2 = (rand() % (orderedMiddleOfPath.size()-1)) +1;
        orderedMiddleOfPath.at(index1) = orderedMiddleOfPath.at(index2);
        orderedMiddleOfPath.at(index2) = temp;       
    }

    orderedMiddleOfPath.push_back(1); //now path is a full path 
    return orderedMiddleOfPath; 

}

// swap two nodes in path for the next path generation 
void SA::getNextPath(vector<int> & nextPath)
{
    int firstIndex = (rand() % (numNodesClass-1))+1;
    int secondIndex = (rand() % (numNodesClass-1)) +1;

    int temp = nextPath.at(firstIndex);
    nextPath.at(firstIndex) = nextPath.at(secondIndex); 
    nextPath.at(secondIndex) = temp;

}

//calculate the fitness of the path passed in  
double SA::calculateFitness(vector<int> path, const unordered_map<int, Pos> & positionMap)
{
    //cout <<"in calc fitness" <<endl;
    double dist = calculatePathDistance(path,positionMap);

    double fitness = (double) 1 / dist;     //use inverse for fitness 
    return fitness; 
}

//calculate the positional distance of a path 
double SA::calculatePathDistance(const vector<int> & v, const unordered_map<int, Pos> & positionMap)
{

    double dist = 0;

    for (int i = 0; i < v.size() - 1; i++)
    {
        //cout <<"i: " << i << endl; 
        int from = v.at(i);
       // cout << from <<endl; 
        double x1 = positionMap.at(from).x;
        double y1 = positionMap.at(from).y;
        double z1 = positionMap.at(from).z;
        int to = v.at(i + 1);
        double x2 = positionMap.at(to).x;
        double y2 = positionMap.at(to).y;
        double z2 = positionMap.at(to).z;

        double d = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
        dist = dist + d;
    }
    return dist;
}