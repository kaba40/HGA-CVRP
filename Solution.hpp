/*
 * Solution.hpp
 *
 *  Created on: 27 nov. 2018
 *      Author: kkeita
 */

#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

#include<iostream>
#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <time.h>

#include <set>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <utility>
#include<algorithm>
#include<random>

#include "Customer.hpp"
#include "DataAP.hpp"
#include "SplitBellman.hpp"
#include "Node.hpp"
#include "DLinkedList.hpp"
#include "SeqData.hpp"

using namespace std;
class Solution
{
private:

	DLinkedList* encodage;
	vector<vector<vector<SeqData*>>> sequenceTab;
	vector<int> tour;
	vector<vector<Node*>> routeSeq;
	double solutionCost; //objective_value
	int numberOfRouteInSolution;
	DataAP *tsp_data;
public:
	Solution(DataAP *tsp_dat, DLinkedList* encod);
	virtual ~Solution();


	bool Decodage();
	void CheckSolution();
	void PrintSolution();

	// getter methods
	vector<int> getTourStructure();
	double getObjVal();
	int getRoutesNumber();
	DLinkedList* getSequence(); // to delete may be
	vector<vector<Node*>> getRouteSequence(); // to delete may be
	vector<vector<vector<SeqData*>>> getRouteSetSubSeq();

	//initialization methods
	void initRouteSetSubSeq();

	//updater methods
	void updateRoute(int numRoute, vector<Node*> rteSeq); // update each route modify by a move
	void updateOneRouteSetSubSeq(int numRoute);

	//setter methods
	void setRandomSequence();
	void setObjVal(double objVal);
	void setTourStructure(vector<int> route);
	void setRouteNumber(int numRoute);
};

#endif /* SOLUTION_HPP_ */
