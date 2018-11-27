/*
 * SplitBellman.cpp
 *
 *  Created on: 27 nov. 2018
 *      Author: kkeita
 */

#include "SplitBellman.hpp"
#include "Customer.hpp"

SplitBellman::SplitBellman(DataAP* tspData)
{
	_tspData = tspData;
	// TODO Auto-generated constructor stub

}

SplitBellman::~SplitBellman() {
	// TODO Auto-generated destructor stub
}

int SplitBellman::solve()
{
	double load, distance, cost;

	//initialization of vectors
	_pathCost = vector<double>(_tspData->getNumberNodes());
	_pred = vector<int>(_tspData->getNumberNodes());

	for(int i = 0; i < _tspData->getNumberNodes(); i++)
	{
		_pathCost[i] = 1.e30;
		_pred[i] = -1;
	}

	_pathCost[0] = 0.0;

	//split algorithm
	for( int t = 0; t < _tspData->getNumberNodes(); t++)
	{
		load = 0;
		distance = 0;
		int i = t+2 ;//t+1;

		cout << "t = " << t << endl;

		while((i < _tspData->getNumberNodes()) && ((load + _tspData->getCustomerByIndex(i)->getDemand()) <= _tspData->getVehicleCap()))
		{
			load += _tspData->getCustomerByIndex(i)->getDemand();
			Customer *depot, *clientI, *clientIav;

			depot = _tspData->getCustomerByIndex(_tspData->getDepotIndex());

			cout << "depotIndex = " << depot->getIndex() << endl;

			cout << " i = " << i << endl;
			if( i == t+2) //if( i == t+1)
			{
				clientI = _tspData->getCustomerByIndex(i);
				cout << " clientI index = " << clientI->getIndex() << endl;
				distance += depot->getDistance(clientI); // exception index1 and index2 must not be equal
			}
			else
			{
				clientIav = _tspData->getCustomerByIndex(i-1);
				clientI = _tspData->getCustomerByIndex(i);
				distance += clientIav->getDistance(clientI);
			}


			cost = distance + depot->getDistance(clientI);
			if(_pathCost[t] + cost < _pathCost[i] && load <= _tspData->getVehicleCap())
			{
				_pathCost[i] = _pathCost[t] + cost;
				_pred[i] = t;
			}
			 i++;
		}
	}

	return 0;
}
