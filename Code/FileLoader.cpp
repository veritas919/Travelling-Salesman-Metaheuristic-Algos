#include "FileLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Loads position data into a map
void FileLoader::loadGraph(string pathToFile,unordered_map<int, Pos> & positionMap){

    ifstream file;
    file.open(pathToFile.c_str());
    int count = 0;
    string dummy;
    while (getline(file, dummy))
    {
        std::stringstream ss;
        ss.str(dummy);
        int vertex;
        double x;
        double y;
        double z;
        char c;
        ss >> vertex >> c >> x >> c >> y >> c >> z;
        Pos myPos(vertex, x, y, z);
        positionMap[vertex] = myPos;
    }

}

// loads answers in file into vector passed in  
void FileLoader::loadKnownAnswers(const string & pathToKnownAnswers, vector<double> & answers)
{
    cout <<"in load answers" <<endl; 
    ifstream file;
    file.open(pathToKnownAnswers.c_str());
    int count = 0;
    string dummy;
    while (getline(file, dummy))
    {
        std::stringstream ss;
        ss.str(dummy);
        string  algoName;
        int nodesInPath;
        double time;
        double cost;
        ss >> algoName >> nodesInPath >>time >> cost;
        answers.push_back(cost);
    }
    for (auto i: answers){
        cout << i << " " ; 
    }
}



