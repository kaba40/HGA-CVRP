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
#include "DLinkedList.hpp"

using namespace std;
class Solution
{
private:
//	vector<Customer*> encodage;
	DLinkedList* encodage;
	vector<int> tour;
	double solutionCost; //objective_value
	int numberOfRouteInSolution;
	DataAP *tsp_data;
public:
	Solution(DataAP *tsp_dat, DLinkedList* encod);
	virtual ~Solution();

	void setRandomSequence();
  // kairaba : méthode sans argument
  // enc : attribut de la classe. Donc tu y as accès
	bool Decodage();

  // kairaba : méthode sans argument
  // enc : attribut de la classe. Donc tu y as accès
	void CheckSolution();

  // kairaba : méthode sans argument
  // enc : attribut de la classe. Donc tu y as accès
  void PrintSolution();

	vector<int> getTourStructure();
	double getObjVal();
	int getNumberOfRoutesInSolution();
//	vector<Customer*> getSequence();
	DLinkedList* getSequence();
};

#endif /* SOLUTION_HPP_ */
