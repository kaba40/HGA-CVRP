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


//	solution_tsp->setRandomSequence();

#ifdef DEBUG_Main

	for(Node *custoNode = solution_tsp->getSequence()->getHead(); custoNode != NULL; custoNode = custoNode->getNext())
	{
		cout << "custo " << custoNode->getClient()->getId() << " index " << custoNode->getClient()->getIndex() << " demand " << custoNode->getClient()->getDemand() << endl;
	}

#endif


	if(solution_tsp->Decodage())
	{
        //solution_tsp->CheckSolution();
		solution_tsp->PrintSolution();
	}
    exit(0);
	vector<Node*> rteSeq = solution_tsp->getRouteSequence();
	int numRoute = solution_tsp->getRoutesNumber();

	for(int i = 0; i < numRoute; i++)
	{
		cout << "route[" << i << "]= " ;
		for( Node *tmp = rteSeq[i]; tmp != NULL; tmp = tmp->getNext())
		{
			cout <<  tmp->getClient()->getId() << "--";
		}
		cout << endl;
	}


	exit(-1);

//	solution_tsp->initRouteSetSubSeq();
//
//	vector<vector<vector<SeqData*>>> routeForSubSeq = solution_tsp->getRouteForwSeq();
//
//	for(uint i = 0; i < routeForSubSeq.size(); i++)
//	{
//		cout << "route[" << i << "] " << "contain " << routeForSubSeq[i].size() << " element" << endl;
//		//determine the cost of a route, last element of the vector routeSetSubSeq[i][0]
//	//	cout << "route[" << i << "] " << "cost " << routeSetSubSeq[i][0].back()->getDistance() << " " << endl;
//		for(uint j = 0; j < routeForSubSeq[i].size(); j++)
//		{
//			if(j == 0)
//				cout << "route[" << i << "] " << "cost " << routeForSubSeq[i][j].back()->getDistance() << " " << endl;
//			for(uint k = 0; k < routeForSubSeq[i][j].size(); k++)
//			{
//				cout << routeForSubSeq[i][j][k]->getHead()->getClient()->getId() << "--" <<routeForSubSeq[i][j][k]->getTail()->getClient()->getId() << " ";
//				cout << "---->" << " dde = " << routeForSubSeq[i][j][k]->getDemand() << " dist = " << routeForSubSeq[i][j][k]->getDistance() << endl;
//			}
//		}
//	}
//
//	exit(-1);
//
//	vector<vector<vector<SeqData*>>> routebackSubSeq = solution_tsp->getRouteBackSeq();
//
//	for(uint i = 0; i < routebackSubSeq.size(); i++)
//	{
//		cout << "route[" << i << "] " << "contain " << routebackSubSeq[i].size() << " element" << endl;
//		//determine the cost of a route, last element of the vector routeSetSubSeq[i][0]
//	//	cout << "route[" << i << "] " << "cost " << routeSetSubSeq[i][0].back()->getDistance() << " " << endl;
//		for(uint j = 0; j < routebackSubSeq[i].size(); j++)
//		{
//			if(j == 0)
//				cout << "routeInvert[" << i << "] " << "cost " << routebackSubSeq[i][j].back()->getDistance() << " " << endl;
//			for(uint k = 0; k < routebackSubSeq[i][j].size(); k++)
//			{
//				cout << routebackSubSeq[i][j][k]->getHead()->getClient()->getId() << "--" <<routebackSubSeq[i][j][k]->getTail()->getClient()->getId() << " ";
//				cout << "---->" << " dde = " << routebackSubSeq[i][j][k]->getDemand() << " dist = " << routebackSubSeq[i][j][k]->getDistance() << endl;
//			}
//		}
//	}

//	LocalSearch* localAlgo = new LocalSearch(solution_tsp);
//
//	if(localAlgo->IntraRouteInsert())
//	{
//		cout << "Insert" << endl;
//	}
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
