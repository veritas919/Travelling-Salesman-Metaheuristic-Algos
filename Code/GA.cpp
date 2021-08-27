// https://towardsdatascience.com/evolution-of-a-salesman-a-complete-genetic-algorithm-tutorial-for-python-6fe5d2b3ca35 
// https://www.hindawi.com/journals/cin/2017/7430125/  referenced for crossover techniques for TSP 


#include "GA.h"
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
    
// set user-specified configurations 
void GA:: setConfigs(int popSize, double mutationRate, const string & selection, const string & breeding, const string & mutation)
{
    popSizeClass = popSize;
    this->mutationRate = mutationRate;

    mutationChoice = mutation;
    selectionChoice = selection;
    breedingChoice = breeding;    
}
    
// run the algorithm 
void GA::startAlgo(int numNodes, const unordered_map<int, Pos> & positionMap, const string & mode, double answer, vector<double>& epochCosts, vector<int> & results, double & distance)
{
    numNodesClass = numNodes; 
    currGen = initialPopulation();
    double minDist = 200000; 

    vector<int> holder; 
       
    // run for i steps/generations of the population. Can alter value i goes to 
    for(int i = 0; i <= 10000;i++)
    {
        fitnessAssignments(positionMap);
        double currBestCost = determineFittestCurrent(positionMap, holder);
        // keep track of best solution and associated distance over multiple epochs 
        if(currBestCost < minDist)
        {
            minDist = currBestCost;
            results = holder;
            distance = currBestCost;  
        }
        // if mode is timing and distance within 5% of answer, consider answer found 
        if( ( abs(currBestCost - answer) /answer) < 0.05 && mode == "NodeTiming")
            return; 
        if(mode == "LC")
        {
            epochCosts.push_back(currBestCost); // save cost at each epoch 
        }      
        if(i % 500 == 0 && mode == "LC")
        {
            cout << "epoch: " << i << " current distance: " << currBestCost <<endl; // print progress to console                
        }
        if(selectionChoice == "Elitism")
        {
            selectMatingPoolElitism();
        }
        else if(selectionChoice == "Tournament") 
        {
            selectMatingPoolTournament(); 
        }   
        else{
            cout <<" you specified an invalid selection technique!!! Choose Elitism or Tournament" <<endl;
            exit(1);  
        }         
        CrossOverTheParents();  // Breed. (OX or SingleCross) 
        mutate();               // mutate (Swap or Inversion)
        // clear class vars 
        currGen.clear();
        fitnesses.clear();
        currGen = nextGeneration;
        nextGeneration.clear(); 
        matingPool.clear(); 
        // cout << "size: " << matingPool.size() <<endl;
    }
}



// calculate the fitness of the path passed in  
double GA::calculateFitness(vector<int> path, const unordered_map<int, Pos> & positionMap)
{
    double dist = calculatePathDistance(path,positionMap);

    double fitness = (double) 1 / dist;     //use inverse for fitness 
    return fitness; 
}

//generate a path  
vector<int> GA::generatePath()
{
    vector<int> orderedMiddleOfPath;
    orderedMiddleOfPath.push_back(1); //start is always 1 
    for (int i = 2; i <= numNodesClass; i++)
    {
        orderedMiddleOfPath.push_back(i);  
    } 

    // CAN ALTER THE NUMBER OF SWAPS TO VARY PERFORMANCE OF ALGO 
    for(int i = 0; i < numNodesClass;i++)
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

// called to generate and return the initial, random paths for the population 
vector<vector<int>> GA::initialPopulation()
{
    srand (time(0)); // have this so we get diff paths with generatePath function 
 
    vector< vector<int> > population;
    // generate random initial population
    for(int i = 0; i < popSizeClass;i++)
    {
        population.push_back(generatePath());
    }
    return population; 
}


//  calculate fitness values for each individual in population 
void GA::fitnessAssignments(const unordered_map<int, Pos> & positionMap)
{
    vector<vector<int>> selectionResults;
    fitnesses.clear();  //ensure fitnesses is empty 
    // calculate fitness score for each individual path in population 
    for(int i =0; i < currGen.size();i++)
    {
        double fitness = calculateFitness(currGen.at(i), positionMap);
        fitnesses.push_back(fitness); 
    }   
}

// find the current best(cheapest) cost 
double GA::determineFittestCurrent(const unordered_map<int, Pos> & positionMap, vector<int> & bestPathCurrent)
{
    double currFitnessMax = -1.1;
    int bestCurrPathIndex = -1;
    for(int i = 0; i < fitnesses.size(); i++)
    {
        if(fitnesses.at(i) > currFitnessMax )
        {
            currFitnessMax = fitnesses.at(i);
            bestCurrPathIndex = i;
        }
    }
    bestPathCurrent = currGen.at(bestCurrPathIndex); 
    double cost = calculatePathDistance(bestPathCurrent, positionMap);
    return cost; 
}


//calculate the positional distance of a path 
double GA::calculatePathDistance(const vector<int> & v, const unordered_map<int, Pos> & positionMap)
{

    double dist = 0;

    for (int i = 0; i < v.size() - 1; i++)
    {
        int from = v.at(i);
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

// Tournament selection 
// select the parents via tournament selection
void GA::selectMatingPoolTournament() 
{
    srand (time(0));  
    for(int i = 0; i < popSizeClass; i++) //choose popSize parents 
    {
        int competitor1index = rand() % popSizeClass;
        int competitor2index = rand() % popSizeClass; 

        double fitness1 = fitnesses.at(competitor1index);
        double fitness2 = fitnesses.at(competitor2index);

        if(fitness1 > fitness2)
        {
            matingPool.push_back(currGen.at(competitor1index));
        }
        else{
            matingPool.push_back(currGen.at(competitor2index));
        }
    }
}


// referenced https://www.geeksforgeeks.org/sorting-vector-of-pairs-in-c-set-1-sort-by-first-and-second/
bool GA::sortBySecond(const pair<int, double> &a, const pair<int, double> & b)
{
    return (a.second > b.second);
}

// Elitism selection 
// select the parents via Elitism selection
void GA::selectMatingPoolElitism() 
{
    vector<pair<int, double>> indexInPopulationToFitness;
    for(int i = 0; i < currGen.size(); i++)
    {
        indexInPopulationToFitness.push_back(make_pair(i, fitnesses.at(i)));
    }

    sort(indexInPopulationToFitness.begin(), indexInPopulationToFitness.end(), sortBySecond);
    vector<int> bestIndexesInPop;
    for(int i = 0; i < popSizeClass/2;i++) //choose popSize/2 best solutions to breed for next generation
    {
        bestIndexesInPop.push_back(indexInPopulationToFitness.at(i).first);
    }

    srand (time(0));  
    for(int i = 0; i < popSizeClass; i++) //need to choose popSize parents 
    {
        int randomNum = rand() % (popSizeClass/2);  
        matingPool.push_back(currGen.at(bestIndexesInPop.at(randomNum)));
    }
}


// Calls the user-specified crossover technique and saves the children into the next generation 
void GA::CrossOverTheParents()
{
    nextGeneration.clear();     // clear the nextGeneration vector 
    if(breedingChoice == "OX")
    {
        for(int i = 0; i < matingPool.size(); i=i+2)
        {
            pair<vector<int>, vector<int>> children = orderedCrossOver(matingPool.at(i), matingPool.at(i+1));
            nextGeneration.push_back(children.first);
            nextGeneration.push_back(children.second); 
        }
    }

    else if (breedingChoice == "SingleCross")
    {
        for(int i = 0; i < matingPool.size(); i=i+2)
        {
            pair<vector<int>, vector<int>> children = singleCross(matingPool.at(i), matingPool.at(i+1));
            nextGeneration.push_back(children.first);
            nextGeneration.push_back(children.second); 
        }
    }
}

// Single Cross 
// Each child replicates a parent and then one gene from the other parent is taken by child 
pair<vector<int>, vector<int>> GA::singleCross(vector<int> p1, vector<int> p2)
{
    vector<int> c1(p1);
    vector<int>c2 (p2);

    // generate c1 
    int chosenIndex = (rand() % (numNodesClass - 1)) +1;
    
    int valueInP2 = p2.at(chosenIndex);
    int valueInC1 = c1.at(chosenIndex);

    if(valueInP2 == valueInC1)
    {
        //do nothing
    }
    else
    {
       int savedVal = c1.at(chosenIndex); 
       c1.at(chosenIndex) = valueInP2;
       for(int i = 1; i < c1.size()-1;i++) // beginning and end index are 1 so dont have to worry about those indices
       {
           if(c1.at(i) == valueInP2)
           {
               c1.at(i) = savedVal;
               break;
           }
       } 
    }

    // generate c2 
    int chosenIndex2 = (rand() % (numNodesClass - 1)) +1;
    int valueInP1 = p1.at(chosenIndex2);
    int valueInC2 = c2.at(chosenIndex2);

    if(valueInP1 == valueInC2)
    {
        //do nothing
    }
    else
    {
       int savedVal2 = c2.at(chosenIndex2);
       c2.at(chosenIndex2) = valueInP1;
       for(int i = 1; i < c2.size()-1;i++) // beginning and end index are 1 so dont have to worry about those indices
       {
           if(c2.at(i) == valueInP1)
           {
               c2.at(i) = savedVal2;
               break;
           }
       } 
    }

    return pair<vector<int>, vector<int>>(c1, c2);
}



// Ordered Crossover (OX) 
pair<vector<int>, vector<int>> GA:: orderedCrossOver(vector<int> p1, vector<int> p2) 
{
    vector<int> p1Shorter;
    //get vector with just middle nodes in path
    for (int i =1; i < p1.size()-1; i++)
    {
        p1Shorter.push_back(p1.at(i));
    }

    vector<int> p2Shorter;
    //get vector with just middle nodes in path
    for (int i =1; i < p2.size()-1; i++)
    {
        p2Shorter.push_back(p2.at(i));
    }

    int firstIndex = rand() % p1Shorter.size();
    int secondIndex = rand() % p2Shorter.size();

    if(secondIndex < firstIndex)
    {
        int temp = firstIndex;
        firstIndex = secondIndex;
        secondIndex = temp;
    }
    /// First Child ///
    unordered_map<int,int> blacklistP2;

    // forming c1;
    vector<int>c1Shorter (p1Shorter.size(), 0);// make c1 0 0 0 0 0
    for(int i = firstIndex; i <= secondIndex;i++) // copy inclusive [index1, index2]
    {
        c1Shorter.at(i) = p1Shorter.at(i); // copy over elements from p1  
        blacklistP2[c1Shorter.at(i)] = 100; //100 is blacklisted 
    }

    //now have to copy over ordered items from p2Shorter;
    vector<int> p2Clean; //no blacklisted items
    // check thru end of p2SHorter
    for(int i = secondIndex+1; i < p2Shorter.size(); i++)
    {
        if (blacklistP2.find(p2Shorter.at(i)) == blacklistP2.end()) 
        {
            p2Clean.push_back(p2Shorter.at(i));
        }       
    }

    // need to loop around to front of p2Shorter
    for(int i = 0; i <= secondIndex;i++)
    {
       if (blacklistP2.find(p2Shorter.at(i)) == blacklistP2.end()) 
        {
            p2Clean.push_back(p2Shorter.at(i));
        }   
    }
    int p2CleanIndex = 0;
    for(int i = secondIndex +1; i < c1Shorter.size();i++)
    {
        c1Shorter.at(i) = p2Clean.at(p2CleanIndex);
        p2CleanIndex++;
    } 

    for(int i = 0; i < firstIndex; i++)
    {
        c1Shorter.at(i) = p2Clean.at(p2CleanIndex);
        p2CleanIndex++;
    }

    /// Second Child ///

    unordered_map<int,int> blacklistP1;

    // forming c2;
    vector<int>c2Shorter (p2Shorter.size(), 0);// make c2 0 0 0 0 0
    for(int i = firstIndex; i <= secondIndex;i++) // copy inclusive [index1, index2]
    {
        c2Shorter.at(i) = p2Shorter.at(i); // copy over elements from p2   
        blacklistP1[c2Shorter.at(i)] = 100; //100 is blacklisted  
    }

    //now have to copy over ordered items from p1Shorter;
    vector<int> p1Clean; //no blacklisted items
    // check thru end of p1SHorter
    for(int i = secondIndex+1; i < p1Shorter.size(); i++)
    {
        if (blacklistP1.find(p1Shorter.at(i)) == blacklistP1.end()) 
        {
            p1Clean.push_back(p1Shorter.at(i));
        }       
    }
    // need to loop around to front of p1Shorter
    for(int i = 0; i <= secondIndex;i++)
    {
       if (blacklistP1.find(p1Shorter.at(i)) == blacklistP1.end()) 
        {
            p1Clean.push_back(p1Shorter.at(i));
        }   
    }
    int p1CleanIndex = 0;
    for(int i = secondIndex +1; i < c2Shorter.size();i++)
    {
        c2Shorter.at(i) = p1Clean.at(p1CleanIndex);
        p1CleanIndex++;
    } 

    for(int i = 0; i < firstIndex; i++)
    {
        c2Shorter.at(i) = p1Clean.at(p1CleanIndex);
        p1CleanIndex++;
    }

    // add 1 to start and end of paths 
    c1Shorter.push_back(1);
    c2Shorter.push_back(1);

    auto it = c1Shorter.insert(c1Shorter.begin(), 1); 
    auto it2 = c2Shorter.insert(c2Shorter.begin(), 1);

    return pair<vector<int>, vector<int>>(c1Shorter, c2Shorter);
}

// Swap Mutation 
void GA::mutateSwap(int indexOfPathToMutate)
{ 
    int firstIndex = (rand() % (numNodesClass-1))+1;
    if(nextGeneration.at(indexOfPathToMutate).at(firstIndex) == 1)
    {
        //cout <<"problem..swapping node 1!" <<endl; 
    }
    int secondIndex = (rand() % (numNodesClass-1)) +1;

    int temp = nextGeneration.at(indexOfPathToMutate).at(firstIndex);
    nextGeneration.at(indexOfPathToMutate).at(firstIndex) = nextGeneration.at(indexOfPathToMutate).at(secondIndex); 
    nextGeneration.at(indexOfPathToMutate).at(secondIndex) = temp;
}

// Inversion Mutation 
void GA::mutateInvert(int indexOfPathToMutate)
{ 
    int firstIndex = (rand() % (numNodesClass-1)) +1;

    int secondIndex = (rand() % (numNodesClass-1))+1;
    // make it so firstIndex is smaller than or equal to secondIndex 
    if(secondIndex < firstIndex)
    {
        int temp = firstIndex;
        firstIndex = secondIndex;
        secondIndex = temp;
    }

    stack<int> nodeVals;
    for(int i = firstIndex; i <=secondIndex;i++)
    {
        nodeVals.push(nextGeneration.at(indexOfPathToMutate).at(i));
    }

    for(int i = firstIndex; i <=secondIndex;i++)
    {
        nextGeneration.at(indexOfPathToMutate).at(i) = nodeVals.top();
        nodeVals.pop(); 
    }
}

// determines if a mutation should occur and calls appropriate mutation function 
void GA::mutate()
{
    for(int i = 0; i < popSizeClass; i++)
    {
        double rando = ((double) rand() / (RAND_MAX)); //generate number between 0 and 1 
        if (rando < mutationRate && mutationChoice == "Swap")
        {
            //mutate that individual!
            mutateSwap(i);
        }

        else if (rando < mutationRate && mutationChoice == "Invert")
        {
            //mutate that individual!
            mutateInvert(i);
        }
        else
        {
            //cout <<"No mutation" <<endl; 
        }
        
    }
}
