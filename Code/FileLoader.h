#ifndef FILELOADER_H
#define FILELOADER_H

#include <unordered_map>
#include <iostream> 
#include <vector> 

#include "Pos.h" 


using namespace std;

class FileLoader
{

public:

    /*
    * Loads position data into a map
     * Input: string of file path to positions.txt file 
     * Output: none
     */
    void loadGraph(string, unordered_map<int, Pos> &);

    /*
    * loads cost for each number of nodes in path into the vector passed in 
    */
    void loadKnownAnswers(const string & pathToKnownAnswers, vector<double> & answers);

};

#endif
