/*
 * main.cpp
 *
 *  Created on: 21 nov. 2018
 *      Author: kkeita
 */

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
#include<random>

#include "DataAP.hpp"
#include "Customer.hpp"
#include "SplitBellman.hpp"
#include "Solution.hpp"
#include "Node.hpp"
#include "DLinkedList.hpp"
#include "LocalSearch.hpp"

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

	cout <<  endl;

	cout << " implementation split algorithm" << endl;

	Solution *solution_tsp;
	solution_tsp = new Solution(data_instance, data_instance->getCustomerList());


//	solution_tsp->setRandomSequence();

#ifdef SPLIT_ALGORITHM_DEBUG

	for(uint i = 0; i< solution_tsp->getSequence()->getSize(); i++)
	{
		cout << "custo " << solution_tsp->getSequence()->find(i)->getClient()->getId() << " index " << solution_tsp->getSequence()->find(i)->getClient()->getIndex() << " demand " << solution_tsp->getSequence()->find(i)->getClient()->getDemand() << endl;
	}

#endif


	if(solution_tsp->Decodage())
	{
		solution_tsp->CheckSolution();
		solution_tsp->PrintSolution();
	}


	LocalSearch* localAlgo = new LocalSearch(solution_tsp);
	localAlgo->Insert();
//
//	DLinkedList *localSol = localAlgo->Insert(data_instance->getCustomerList());
//
//	Solution *sol = new Solution(data_instance, localSol);
//
//	if(sol->Decodage())
//	{
//		sol->CheckSolution();
//		sol->PrintSolution();
//	}
//
//
	delete localAlgo;
//	delete localSol;
//	delete sol;
	delete data_instance;
	delete solution_tsp;


	return 0;

}
