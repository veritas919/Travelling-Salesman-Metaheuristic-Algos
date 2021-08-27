#ifndef TSP_H
#define TSP_H

#include <vector>
#include <unordered_map>
#include <string> 

#include "Algorithm.h"
//#include "TSPDP.h"  can include if you want 
//#include "TSPBrute.h"
#include "Pos.h"
#include "FileLoader.h"
#include "OutputSystem.h"
#include "GA.h"
#include "SA.h"

using namespace std;

class TSP : Algorithm
{

public:
    /*
    * Loads the position data into the appropriate storage units 
     * Input: string of file path to positions.txt file 
     * Output: none
     */
    void load(const string &);

    /*
    * Loads known answers into a vector 
    * Input: path to the file with the answers
    * Output: none
    */
    void loadKnownAnswers(const string & pathToKnownAnswers);

    /*
    * Selects the TSP implementation to use 
    * Input: string of the chosen algorithm and implementation  
    * Output: None 
    */
    void select(const string &);


    /*
    *Executes the selected algorithm, visiting the specified number of nodes, and retains timing data
    *Input: integer representing number of nodes to include in circuit 
    *Output: None 
    */
    void execute(int numNodes);

    
    /*
    *Uses an instance of OutputSystem to print statistics for the algorithm to console
    *Input: None
    *Output: None
    */
    void stats();

    /*
    *Uses an instance of OutputSystem to write information pertaining to an algorithm execution to the file passed in 
    * Input: path to text file to save information to 
    * Output: None
    */
    void save(const string& );

    /*
    * Selects the output mode the user wants  
    * Input: user-specified output mode - "LC" or "NodeTiming"
    * Output: None 
    */
    void selectOutputMode(const string &); 

    /*
    * Selects the configurations the user wants for GA
    * Input: population size, mutation rate, selection method, breeding method, and mutation method
    * Output: none 
    */
    void chooseGAConfigs(int popSize, double mutationRate, const string & selection, const string & breeding, const string & mutation);
    
    /*
    * Selects the configurations the user wants for SA
    * Input: alpha, initial temperature, scaling technique - "proportional" or "linear" 
    * Output: None
    * */ 
    void chooseSAConfigs(double coolingRateConstant, double initialTemp, const string & scalingTechnique);


private:
    string selectedAlgo;
    string outputMode;
    int nodesInPath;
    unordered_map<int, Pos> positionMap;

    vector<int> results; 
    vector<double> epochCosts; 
    vector<double> answers;
    double time;
    double totalDistance;
    
    FileLoader myFileHandler;
    OutputSystem myOutputSys;

    GA myGA; 
    SA mySA; 

    //TSPDP myDP;
    //TSPBrute myBrute;

// private functions 
     void printPositionMap(); //used for testing 
    double calculateDistance(); //calculates positional distance of going from one node to next until you reach the destination
};

#endif
