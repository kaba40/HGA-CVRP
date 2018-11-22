/*
 * main.cpp
 *
 *  Created on: 21 nov. 2018
 *      Author: kkeita
 */

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include "DataAP.hpp"

using namespace std;

int main()
{

	string name_data = "Data/E-n7.vrp";

	const char* data_vrp = name_data.c_str();

	DataAP * data_instance;
	data_instance = new DataAP(data_vrp);

	cout<< "number of nodes = "<< data_instance->getNodes() << endl;

	cout << " implementation split algorithm" << endl;

	return 0;

}
