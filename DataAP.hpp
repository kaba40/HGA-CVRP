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

//#include "Customer.hpp"
using namespace std;
//using namespace boost;


//struct customer
//{
//	int index;
//	double demand;
//};

class Customer;

class DataAP
{

private:

	vector<Customer*> clients; // vector of customer
	vector<vector<double>> distNodes; // distances between nodes LOWER_DIAG_ROW

	int numberNodes;
	int numberVehicle;
	int vehicleCap;
	int depotIndex;

 void readData( const char* data_vrp, int nbVeh);


public:

 	DataAP();
	DataAP( const char* data_vrp, int nbVeh);
	int getNumberNodes();
	int getDepotIndex();
	int getNumberVehicle();
	int getVehicleCap();
	double getDistances(int index1, int index2);
	Customer* getCustomerByIndex( int index);
	virtual ~DataAP();
};

#endif /* DATAAP_HPP_ */
