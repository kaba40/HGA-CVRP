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
#include "Customer.hpp"

using namespace std;

int main()
{

	string name_data = "Data/E-n7.vrp";
	const char* data_vrp = name_data.c_str();

	int numVeh = 1;

	DataAP * data_instance;
	data_instance = new DataAP(data_vrp, numVeh);

	cout << " affichage d'information d'une instance " << endl;

	cout<< " number of nodes = "<< data_instance->getNumberNodes() << endl;
	cout << " number of vehicles = " << data_instance->getNumberVehicle() << endl;
	cout << " vehicle capacity = " << data_instance->getVehicleCap() << endl;
	cout << " distance entre 1 et 2 = " << data_instance->getDistances(1,2) << endl;
	cout << " distance entre 2 et 1 = " << data_instance->getDistances(2,1) << endl;

	Customer* custo1 = data_instance->getCustomerByIndex(6);

	Customer* custo2 = data_instance->getCustomerByIndex(7);
//	custo1 = new Customer(1,0, data_instance);
//	custo2 = new Customer(2,1, data_instance);

	cout << " distance entre 6 et 7 = " << custo1->getDistance(custo2) << endl;

	cout << " implementation split algorithm" << endl;

	return 0;

}
