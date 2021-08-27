#ifndef OUTPUTSYSTEM_H
#define OUTPUTSYSTEM_H

#include <iostream> 
#include<string>
#include<vector> 

using namespace std;

class OutputSystem
{

public:

    void writeToConsoleAlgoInfo(const string & algoName, double time, double distance, const vector<int> & path);
    void saveAlgoInfoToFile(const string & , const string & , int , double, double );
    void saveLCInfoToFile(const string & filePath, const vector<double> & epochCosts);
    void saveNodeTimingInfoToFile(const string & filePath, double time, int nodesInPath); // append 
};

#endif
