#include "OutputSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// writes information pertaining to an algorithm execution to console 
void OutputSystem::writeToConsoleAlgoInfo(const string & algoName, double time, double distance, const vector<int> & path){

    cout <<"selected algo: " << algoName <<endl; 
    cout <<"number of nodes in path (not counting start twice): " << path.size()-1 <<endl; 
    cout << "time: " << time <<endl;
    cout <<"total distance: " << distance<<endl; 
    for(auto i:path){
        cout << i << " ";
    }
    cout << endl << endl; 
}

//writes information pertaining to an algorithm execution to the file passed in 
void OutputSystem::saveAlgoInfoToFile(const string & filePath, const string & selectedAlgo, int nodeNumber, double time, double distance)
{
    ofstream outFile;
    outFile.open(filePath.c_str(), std::ios_base::app);

    if (!outFile.is_open())
    {
        cout << "problem" << endl;
    }
    else
    {
        cout << "output file opened" << endl;
    }
    outFile << selectedAlgo << " " << nodeNumber << " " << time << " " << distance << "\n";

    outFile.close();
}

// save learning curve data to specified file path passed in 
void OutputSystem::saveLCInfoToFile(const string & filePath, const vector<double> & epochCosts)
{
    cout << "SIZE: " << epochCosts.size() <<endl;
    cout << epochCosts.at(epochCosts.size()-1); 
    ofstream outFile;
    outFile.open(filePath.c_str());

    if (!outFile.is_open())
    {
        cout << "problem" << endl;
    }
    else
    {
        cout << "output file opened" << endl;
    }
    outFile<< "epoch" << "," << "cost" <<"\n";
    for(int i = 0; i < epochCosts.size();i++)
    {
        outFile << i + 1 << "," << epochCosts.at(i) << "\n";
    }

    outFile.close();
}

// save node timing data to file passed in 
void OutputSystem::saveNodeTimingInfoToFile(const string & filePath, double time, int nodesInPath)
{
    ofstream outFile;
    outFile.open(filePath.c_str(), std::ios_base::app);

    if (!outFile.is_open())
    {
        cout << "problem" << endl;
    }
    else
    {
        cout << "output file opened" << endl;
    }
    outFile << nodesInPath << " " << time << "\n";

    outFile.close();
}

