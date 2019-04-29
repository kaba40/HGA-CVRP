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

#include "DLinkedList.hpp"

//#include "Customer.hpp"
using namespace std;

class Customer;

class DataAP
{

private:

	vector<Customer*> clients; // vector of customer
	vector<vector<double>> distNodes; // distances between nodes LOWER_DIAG_ROW

	int numberNodes; // number of nodes including depot
	int vehicleCap; // vehicle capacity

 void readData( const char* data_vrp); // method to read data

public:

 	DataAP();
	DataAP( const char* data_vrp);
	int getNumberNodes();
	int getVehicleCap();
	double getDistances(int index1, int index2);
	Customer* getCustomerByIndex( int index);
	vector<Customer*> getCustomers();
	DLinkedList* getCustomerList();
	virtual ~DataAP();
};

#endif /* DATAAP_HPP_ */
