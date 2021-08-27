#include <iostream>
#include <string> 
#include <fstream>
#include <sstream>

#include "TSP.h"

using namespace std;

// NOTES when running... 

// whichever output mode you choose, calling stats() will output to the console the algo name, number of nodes, time, distance, and path 
// with output mode of "LC", epoch steps and distance will be output to the console so you know how far in the algo is when running 

// Output selection choices are "LC" or "NodeTiming"
// GA selection choices are: "Elitism" or "Tournament"
// GA crossover choices are: "OX" or "SingleCross"
// GA mutation choices are: "Swap" or "Invert"

// SA scaling technique choices are: "proportional" or "linear" 

int main(int argc, char * argv[])
{
    TSP myAlgo;
    string pathToPositionFile = "Data/Graph/positions.txt";

    myAlgo.load(pathToPositionFile);  // load graph

 //////////// uncomment below to display and save learning curve data for algorithm with specified configurations /////////////
// GA 
/*   
    myAlgo.select("GA");
    myAlgo.chooseGAConfigs(80, 0.10, "Elitism", "OX", "Swap"); // parameters are population size, mutation rate, selection choice, crossover choice, mutation chocie 
    myAlgo.selectOutputMode("LC"); 
    myAlgo.execute(40); // 40 node path
    myAlgo.stats();  // displays stats to console 
    //myAlgo.save("Data/FileToSaveTo1.txt");  //uncomment this line to save data to file 
*/        
//SA
/*  
    myAlgo.select("SA");
    myAlgo.chooseSAConfigs(0.01, 10000.1, "proportional"); // parameters are alpha value, initial temperature value, scaling choice 
    myAlgo.selectOutputMode("LC");
    myAlgo.execute(30);     //30 node path 
    myAlgo.stats();     //displays stats to console 
    //myAlgo.save("Data/FileToSaveTo2.txt");  //uncomment this line to save data to file 
*/      

///////////// uncomment below to display and save the timing it takes for the algorithm to get within 5% of the answer ////////////////

    string pathToKnownAnswers = "Data/KnownResults.txt";
    myAlgo.loadKnownAnswers(pathToKnownAnswers); // known answers from DP and Brute 
    myAlgo.select("GA"); // choose "SA" or "GA" 
    //myAlgo.chooseSAConfigs(0.1, 10000.1, "proportional");   // if you uncomment this, change myAlgo.select("SA"); and comment out chooseGAConfigs line
    myAlgo.chooseGAConfigs(80, 0.10, "Elitism", "OX", "Swap"); 
    myAlgo.selectOutputMode("NodeTiming");
    for(int i = 4; i <= 21;i++)
    {
        myAlgo.execute(i); 
        myAlgo.stats(); 
        //myAlgo.save("Data/TimingDataDummyFile.txt");
    }
        

} //end main