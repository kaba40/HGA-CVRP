/*
 * DataAP.hpp
 *
 *  Created on: 22 nov. 2018
 *      Author: kkeita
 */

#ifndef DATAAP_HPP_
#define DATAAP_HPP_

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
#include <utility>      // std::pair

using namespace std;
//using namespace boost;


struct customer
{
	int index;
	double demand;
//	double distReturn;
//	double distNext;
};

class DataAP
{

private:

	vector<customer> clients; // vector of customer
	vector<double> arcCost; // pas precis

	int nbNodes;
	int vehicleCap;
	int depotIndex;

 void readData( const char* data_vrp);


public:

	DataAP( const char* data_vrp);
	int getNodes();

	virtual ~DataAP();
};

#endif /* DATAAP_HPP_ */
