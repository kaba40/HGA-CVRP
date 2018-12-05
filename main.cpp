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
	solution_tsp = new Solution(data_instance);

	vector<Customer*> encoding = vector<Customer*>(data_instance->getNumberNodes()-1);
	encoding = data_instance->getCustomers();

#ifdef SPLIT_ALGORITHM_DEBUG
	// kairaba : avant d'utiliser random_shuffle, il faut exécuter la méthode
	// qui suit d'abord. Autrement, entre deux lancements de ton programme
	// on aura toujours le même vector
	// Enfin, la génération d'une séquence aléatoire doit être une méthode
	// de la classe solution. Cette méthode initialisera l'attribut encoding.
	std::srand ( unsigned ( std::time(0) ) );
	random_shuffle(encoding.begin(), encoding.end());
	for(uint i = 0; i< encoding.size(); i++)
	{
		cout << "enc[" << i << "]= " << encoding[i]->getId() << endl;
		cout << "custo " << encoding[i]->getId() << " index " << encoding[i]->getIndex() << " demand " << encoding[i]->getDemand() << endl;
	}

#endif

	if(solution_tsp->Decodage(encoding))
	{
		solution_tsp->CheckSolution(encoding);
		solution_tsp->PrintSolution(encoding);
	}



	return 0;

}
