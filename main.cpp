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

using namespace std;

int main()
{

	DLinkedList list;

	for(int i = 0; i < 20 ; i+=2)
	{
		Node *node;
		node = new Node(i+1);
		list.push_front(node);
	}

	list.show();
//	Node *tmp;
//	tmp = new Node(1001);
	list.push_front(new Node(1001));

	cout << "montre l'ajout de 1001 au debut" << endl;

	list.show();

	list.push_back(new Node(673));

	cout << "montre l'ajout de 673 à la fin" << endl;

	list.show();


	exit(-1);

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
	solution_tsp = new Solution(data_instance);

	solution_tsp->setRandomSequence();


#ifdef SPLIT_ALGORITHM_DEBUG

	for(uint i = 0; i< solution_tsp->getSequence().size(); i++)
	{
		cout << "custo " << solution_tsp->getSequence()[i]->getId() << " index " << solution_tsp->getSequence()[i]->getIndex() << " demand " << solution_tsp->getSequence()[i]->getDemand() << endl;
	}

#endif

	if(solution_tsp->Decodage())
	{
		solution_tsp->CheckSolution();
		solution_tsp->PrintSolution();
	}



	delete data_instance;
	delete solution_tsp;


	return 0;

}
