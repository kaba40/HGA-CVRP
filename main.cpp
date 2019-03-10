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
//	string name_data = "Data/E-n13-k4.vrp";
	const char* data_vrp = name_data.c_str();

	int numVeh = 1;

	DataAP * data_instance;
	data_instance = new DataAP(data_vrp, numVeh);
#ifdef DEBUG_Main
	cout << " affichage d'information d'une instance " << endl;

	cout<< " number of nodes = "<< data_instance->getNumberNodes() << endl;
	cout << " number of vehicles = " << data_instance->getNumberVehicle() << endl;
	cout << " vehicle capacity = " << data_instance->getVehicleCap() << endl;

	cout <<  endl;

	cout << " implementation split algorithm" << endl;
	cout << endl;
#endif
	Solution *solution_tsp;
	solution_tsp = new Solution(data_instance, data_instance->getCustomerList());


	solution_tsp->setRandomSequence();

#ifdef DEBUG_Main

	for(Node *custoNode = solution_tsp->getSequence()->getHead(); custoNode != NULL; custoNode = custoNode->getNext())
	{
		cout << "custo " << custoNode->getClient()->getId() << " index " << custoNode->getClient()->getIndex() << " demand " << custoNode->getClient()->getDemand() << endl;
	}

#endif

	// true means we use direct encoding to do local search
	if(solution_tsp->Decodage(true))
	{
        solution_tsp->CheckSolution(true);
		solution_tsp->PrintSolution(true);
	}

	LocalSearch* localAlgo = new LocalSearch(solution_tsp);

//	if(localAlgo->IntraRouteInsert())
//	{
//		cout << "IntraRouteInsert" << endl;
//		solution_tsp->CheckSolution(true);
//		solution_tsp->PrintSolution(true);
//	}
//
//	if(localAlgo->IntraRouteArcInsert())
//	{
//		cout << "IntraRouteArcInsert" << endl;
//		solution_tsp->CheckSolution(true);
//		solution_tsp->PrintSolution(true);
//	}
//
//	if(localAlgo->IntraRouteSwap())
//	{
//		cout << "IntraRouteSwap" << endl;
//		solution_tsp->CheckSolution(true);
//		solution_tsp->PrintSolution(true);
//	}

//	if(localAlgo->IntraRouteArcSwap())
//	{
//		cout << "IntraRouteArcSwap" << endl;
//		solution_tsp->CheckSolution(true);
//		solution_tsp->PrintSolution(true);
//	}

//	if(localAlgo->IntraRoute2ArcSwap())
//	{
//		cout << "IntraRoute2ArcSwap" << endl;
//		solution_tsp->CheckSolution(true);
//		solution_tsp->PrintSolution(true);
//	}

	if(localAlgo->InterRouteInsert())
	{
		cout << "InterRouteInsert" << endl;
		solution_tsp->CheckSolution(true);
		solution_tsp->PrintSolution(true);
	}

//	if(localAlgo->Insert())
//	{
//		cout << "Insert" << endl;
//		solution_tsp->PrintSolution(false);
//		solution_tsp->getSequence()->show();
//	}
//
//	if(localAlgo->ArcInsert())
//	{
//		cout << "ArcInsert" << endl;
//		solution_tsp->PrintSolution(false);
//		solution_tsp->getSequence()->show();
//	}
//
//	if(localAlgo->Swap())
//	{
//		cout << "Swap" << endl;
//		solution_tsp->PrintSolution(false);
//		solution_tsp->getSequence()->show();
//	}
//
//	if(localAlgo->SwapArcs())
//	{
//		cout << "SwapArcs" << endl;
//		solution_tsp->PrintSolution(false);
//		solution_tsp->getSequence()->show();
//	}
//
//	if(localAlgo->SwapTwoArcs())
//	{
//		cout << "SwapTwoArcs" << endl;
//		solution_tsp->CheckSolution(false);
//		solution_tsp->PrintSolution(false);
//		solution_tsp->getSequence()->show();
//	}

	delete localAlgo;
	delete data_instance;
	delete solution_tsp;


	return 0;
}
