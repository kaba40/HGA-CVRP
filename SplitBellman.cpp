/*
 * SplitBellman.cpp
 *
 *  Created on: 27 nov. 2018
 *      Author: kkeita
 */

#include "SplitBellman.hpp"
#include "Customer.hpp"

SplitBellman::SplitBellman(DataAP* tspDat,  DLinkedList* enc)
{
	tspData = tspDat;
	encod = enc;
	// TODO Auto-generated constructor stub
}

SplitBellman::~SplitBellman() {
	// TODO Auto-generated destructor stub
}


int SplitBellman::solve()
{
	double load, distance, cost;
#ifdef DEBUG
	for(int i = 0; i < tspData->getNumberNodes()-1; i++)
	{
		cout << "encod[" << i << "]= " << encod->find(i)->getClient()->getId() << endl;
	}
#endif

	//initialization of vectors
	pathCost = vector<double>(tspData->getNumberNodes(), 1.e30);
	pred = vector<int>(tspData->getNumberNodes(), -1);

	pathCost[0] = 0.0; //depot

	//split algorithm
	for( int t = 0; t < tspData->getNumberNodes()-1; t++)
	{
		load = 0;
		distance = 0;
		int i = t+1;
#ifdef SPLIT_ALGORITHM_DEBUG
		cout << "t = " << t << endl;
#endif

//		while((i < tspData->getNumberNodes()) && ((load + tspData->getCustomerByIndex(encod[i-1])->getDemand()) <= tspData->getVehicleCap()))
		while((i < tspData->getNumberNodes()) && ((load + encod->find(i-1)->getClient()->getDemand()) <= tspData->getVehicleCap()))
		{
			Customer *clientI, *clientIav;
			clientI =  encod->find(i-1)->getClient() ; //encod[i-1]; // clientI = tspData->getCustomerByIndex(encod[i-1]);
			load += clientI->getDemand();

			if( i == t+1)
			{
				distance += clientI->getDistanceDepot();
#ifdef SPLIT_ALGORITHM_DEBUG
				cout << "distance [" << 0 << "][" << clientI->getId() << "]= " << distance << endl;
#endif
			}
			else
			{
				clientIav = encod->find(i-2)->getClient() ; // encod[i-2];
				distance += clientIav->getDistance(clientI);
#ifdef SPLIT_ALGORITHM_DEBUG
				cout << "distance [" << clientIav->getId() << "][" << clientI->getId() << "]= " << distance << endl;
#endif
			}

//			exit(-1);
			cost = distance + clientI->getDistanceDepot() ;// distance from depot
			if(pathCost[t] + cost < pathCost[i] && load <= tspData->getVehicleCap())
			{
				pathCost[i] = pathCost[t] + cost; // modifier pathCost[encod[i-1]] par pathCost[]
				pred[i] = t;
#ifdef SPLIT_ALGORITHM_DEBUG
				cout << "pathCost[" << i << "]= " << pathCost[t] + cost << endl;
				cout << "pred[" << i << "]= " << t << endl;
#endif
			}
			 i++;
		}
	}

	if(pathCost[tspData->getNumberNodes()-1] > 1.e29)
	{
		cout << "ERROR : no Split solution has been propagated until the last node" << endl ;
		throw string ("ERROR : no Split solution has been propagated until the last node");
	}
#ifdef SPLIT_ALGORITHM_DEBUG
	for(int i = 0; i < tspData->getNumberNodes(); i++)
		cout<< "pred[" << i << "]" << pred[i] << endl;
	cout << "pathCost[" << tspData->getNumberNodes()-1 << "]= " << pathCost[tspData->getNumberNodes()-1] << endl;
#endif
	return 1;
}

vector<double>& SplitBellman::getPathCost(void)
{

	return pathCost;
}

vector<int>& SplitBellman::getPredence(void)
{
	return pred;
}
