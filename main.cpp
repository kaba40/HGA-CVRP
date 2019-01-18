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

	for(int i = 0; i< solution_tsp->getSequence()->getSize(); i++)
	{
		cout << "custo " << solution_tsp->getSequence()->find(i)->getClient()->getId() << " index " << solution_tsp->getSequence()->find(i)->getClient()->getIndex() << " demand " << solution_tsp->getSequence()->find(i)->getClient()->getDemand() << endl;
	}

#endif


	if(solution_tsp->Decodage())
	{
		solution_tsp->CheckSolution();
		solution_tsp->PrintSolution();
	}

//	vector<vector<Node*>> routeSeq = solution_tsp->getRouteSequence();
//
//	for(uint i = 0; i < routeSeq.size(); i++)
//	{
//		cout << "ROUTE[" << i << "] = {" ;
//		for(uint j = 0; j < routeSeq[i].size(); j++)
//		{
//			cout << routeSeq[i][j]->getClient()->getId() << " ";
//		}
//		cout << "}" << endl;
//	}

	solution_tsp->initRouteSetSubSeq();

	vector<vector<vector<SeqData*>>> routeSetSubSeq = solution_tsp->getRouteSetSubSeq();

	for(uint i = 0; i < routeSetSubSeq.size(); i++)
	{
		cout << "route[" << i << "] " << "contain " << routeSetSubSeq[i].size() << " element" << endl;
		for(uint j = 0; j < routeSetSubSeq[i].size(); j++)
		{

			for(uint k = 0; k < routeSetSubSeq[i][j].size(); k++)
			{
				cout << routeSetSubSeq[i][j][k]->getHead()->getClient()->getId() << "--" <<routeSetSubSeq[i][j][k]->getTail()->getClient()->getId() << " ";
				cout << "---->" << " dde = " << routeSetSubSeq[i][j][k]->getDemand() << " dist = " << routeSetSubSeq[i][j][k]->getDistance() << endl;
			}
		}
	}

	exit(-1);

//	LocalSearch* localAlgo = new LocalSearch(solution_tsp);
//	if(localAlgo->Insert())
//	{
//		cout << "Insert" << endl;
//		solution_tsp->CheckSolution();
//		solution_tsp->PrintSolution();
//		solution_tsp->getSequence()->show();
//	}
//
//	if(localAlgo->ArcInsert())
//	{
//		cout << "ArcInsert" << endl;
//		solution_tsp->CheckSolution();
//		solution_tsp->PrintSolution();
//		solution_tsp->getSequence()->show();
//	}
//
//	if(localAlgo->Swap())
//	{
//		cout << "Swap" << endl;
//		solution_tsp->CheckSolution();
//		solution_tsp->PrintSolution();
//		solution_tsp->getSequence()->show();
//	}
//
//	if(localAlgo->SwapArcs())
//	{
//		cout << "SwapArcs" << endl;
//		solution_tsp->CheckSolution();
//		solution_tsp->PrintSolution();
//		solution_tsp->getSequence()->show();
//	}
//
//
//	if(localAlgo->SwapTwoArcs())
//	{
//		cout << "SwapTwoArcs" << endl;
//		solution_tsp->CheckSolution();
//		solution_tsp->PrintSolution();
//		solution_tsp->getSequence()->show();
//	}
//
//	delete localAlgo;
	delete data_instance;
	delete solution_tsp;


	return 0;

}
