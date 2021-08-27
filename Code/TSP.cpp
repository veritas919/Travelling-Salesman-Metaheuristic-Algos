#include "TSP.h"
#include <iostream>
#include <queue>
#include <stack>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <ratio>
#include <cmath> 

using namespace std;

//receieves positons file and uses an instance of FileLoader to load data into appropriate data structure
void TSP::load(const string & positionFile)
{
    myFileHandler.loadGraph(positionFile, positionMap);
    //printPositionMap(); //just for testing

}

// loads the known cost answers ( known from Lab 3)
void TSP::loadKnownAnswers(const string & pathToKnownAnswers)
{
    myFileHandler.loadKnownAnswers(pathToKnownAnswers, answers); 
}

//select the TSP implementation to use 
void TSP::select(const string & algo)
{
    selectedAlgo = algo;
}

// execute the selected algorithm and obtain timing data 
void TSP::execute(int numNodes)
{
    results.resize(0); 
    epochCosts.resize(0);
    time = 0;
    nodesInPath = numNodes; 
    totalDistance = 0;

    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
 
    if(outputMode == "LC" && selectedAlgo == "GA") 
    {
        t1 = std::chrono::high_resolution_clock::now();
        myGA.startAlgo(numNodes, positionMap, outputMode, 0.0, epochCosts, results, totalDistance);
        t2 = std::chrono::high_resolution_clock::now();
    }
 

    if (outputMode == "LC" && selectedAlgo == "SA") 
    {

        t1 = std::chrono::high_resolution_clock::now();
        mySA.Search(numNodes, positionMap, outputMode, 0.0, epochCosts, results, totalDistance); 
        t2 = std::chrono::high_resolution_clock::now();
    }

    else if(outputMode == "NodeTiming" && selectedAlgo == "GA") // save timing data 
    {
        double answer = answers.at(numNodes - 4);
        t1 = std::chrono::high_resolution_clock::now();
        myGA.startAlgo(numNodes, positionMap, outputMode, answer, epochCosts, results, totalDistance);
        t2 = std::chrono::high_resolution_clock::now();
    }
     
    else if(outputMode == "NodeTiming" && selectedAlgo == "SA") // save timing data 
    {
        double answer = answers.at(numNodes - 4);
        t1 = std::chrono::high_resolution_clock::now();
        mySA.Search(numNodes, positionMap, outputMode, answer, epochCosts, results, totalDistance);
        t2 = std::chrono::high_resolution_clock::now();
    }

    ///////////////////// not using the algos below for this lab, though they could be used if desired /////////////////
    /*
    if (selectedAlgo == "TSP_BRUTE")
    {
        t1 = std::chrono::high_resolution_clock::now();
        totalDistance = myBrute.solve(numNodes, positionMap, results);
        t2 = std::chrono::high_resolution_clock::now();
    }

    else if (selectedAlgo == "TSP_DP")
    {
        t1 = std::chrono::high_resolution_clock::now();
        totalDistance = myDP.solve(numNodes, positionMap, results);
        t2 = std::chrono::high_resolution_clock::now();
    }*/ 

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    time = time_span.count();   // time is saved in TSP class. then call  save in main, so time is saved to file :) 

}


void TSP::selectOutputMode(const string & outputModeChoice)
{
    outputMode = outputModeChoice;
}

// set suer-specified parameters for GA 
void TSP::chooseGAConfigs(int popSize, double mutationRate, const string & selection, const string & breeding, const string & mutation)
{
    myGA.setConfigs(popSize, mutationRate, selection, breeding, mutation);

}

// set user-specified parameters for SA 
void TSP::chooseSAConfigs(double coolingRateConstant, double initialTemp, const string & scalingTechnique)
{
    mySA.setConfigs(coolingRateConstant, initialTemp, scalingTechnique); 
}

//uses an instance of OutputSystem to print statistics for the algorithm to console
void TSP::stats()
{
    myOutputSys.writeToConsoleAlgoInfo(selectedAlgo, time, totalDistance, results);
}


//uses an instance of OutputSystem to write information pertaining to an algorithm execution to the file passed in 
void TSP::save(const string & filePath)
{
    cout <<outputMode<<endl; 

    if(outputMode == "LC")
    {
        myOutputSys.saveLCInfoToFile(filePath, epochCosts);
    }

    else if (outputMode == "NodeTiming")
    {
        myOutputSys.saveNodeTimingInfoToFile(filePath, time, nodesInPath); // append 
    }
    // below is for DP and brute 
    //myOutputSys.saveAlgoInfoToFile(filePath, selectedAlgo, results.size()-1, time, totalDistance);
}


//calculate the positional distance between two nodes
double TSP::calculateDistance()
{

    double dist = 0;

    for (int i = 0; i < results.size() - 1; i++)
    {
        int from = results.at(i);
        double x1 = positionMap[i].x;
        double y1 = positionMap[i].y;
        double z1 = positionMap[i].z;
        int to = results.at(i + 1);
        double x2 = positionMap[i + 1].x;
        double y2 = positionMap[i + 1].y;
        double z2 = positionMap[i + 1].z;

        double d = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2));
        dist = dist + d;
    }
    return dist;
}

// used to test if position map is constructed properly 
void TSP::printPositionMap()
{
    for (auto it = positionMap.cbegin(); it != positionMap.cend(); ++it)
    {
        std::cout << it->first << " " << it->second.x << " " << it->second.y << " " << it->second.z << endl;
    }
}

