#ifndef TSPDP_H
#define TSPDP_H

#include "Pos.h"

#include <unordered_map>
#include <vector> 

using namespace std;

class TSPDP
{

public:

  double solve(int numNodes, const unordered_map<int,Pos> & positionMap, vector<int> &);

private: 
  void generatePartialTourCosts(int startIndex, int numNodes, double ** memoTable, const unordered_map<int, Pos>&  positionMap);
  vector<int> combinations(int r, int numNodes);
  void combinations(int set, int at, int r, int numNodes, vector<int> & subsetVec);
  bool notIn(int i, int subset);
  double calculateDistance(const unordered_map<int, Pos> & positionMap, int node1, int node2); 
  double findMinCost(int startIndex, int numNodes, double ** memoTable, const unordered_map<int, Pos>&  positionMap);
  void findOptimalTour(vector<int> &, int startIndex, int numNodes, double** memoTable, const unordered_map<int, Pos>&  positionMap);


};

#endif
