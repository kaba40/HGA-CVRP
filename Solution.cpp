/*
 * Solution.cpp
 *
 *  Created on: 27 nov. 2018
 *      Author: kkeita
 */

#include "Solution.hpp"

Solution::Solution(DataAP *tsp_dat)
{
	solutionCost =0;
	numberOfRouteInSolution = 0;
	tsp_data = tsp_dat;
}

Solution::~Solution()
{
	for(int i = 0; i < tsp_data->getNumberNodes()-1; i++)
	{
		delete encodage[i];
	}
	// TODO Auto-generated destructor stub
}


bool Solution::Decodage(vector<Customer*> enc)
{
	bool returValue = false;
	SplitBellman *splitAlgo;
	splitAlgo = new SplitBellman(tsp_data, enc);

	if(splitAlgo->solve())
	{
		returValue =true;

		// fill objVal

		solutionCost = splitAlgo->getPathCost()[tsp_data->getNumberNodes() -1];

		// count the number of routes -- numberOfRoutes
		int nbTour = tsp_data->getNumberNodes()-1;
		vector<int> pred = splitAlgo->getPredence();
		while(nbTour != 0)
		{
			nbTour = pred[nbTour];
			numberOfRouteInSolution++;
		}

		// fill Tour vector containg indices of i where a tour start
		nbTour = tsp_data->getNumberNodes()-1;
		tour = vector<int>(numberOfRouteInSolution);
		for (int i = numberOfRouteInSolution-1 ; i >= 0 ; i --)
		{
			nbTour = pred[nbTour] ;
			tour[i] = nbTour+1;
		}
	}
#ifdef SPLIT_ALGORITHM_DEBUG
	for(uint i = 0; i < tour.size(); i++)
		cout << "tour[ " << i << "] =" << tour[i] << endl;
#endif

	return returValue;

}

void Solution::CheckSolution(vector<Customer*> enc)
{
	int start, end;
	double load;
	double distance = 0;

	if(tour[0] != 1)
		cout << " Error : the first route should start with the first customer" << endl;

	for(int i = 0; i < numberOfRouteInSolution; i++)
	{
		load = 0;
		start = tour[i];
		if( i < numberOfRouteInSolution-1)
			end = tour[i+1];
		else
			end = enc.size()+1;


		for(int j = start; j < end; j++)
			load += enc[j-1]->getDemand();

		distance += enc[start-1]->getDistanceDepot() + enc[end-2]->getDistanceDepot();
		for(int j = start; j < end-1; j++)
		{
			int k = j-1;
			int ksuiv = k+1;
			distance += enc[k]->getDistance(enc[ksuiv]);
		}

		if(load > tsp_data->getVehicleCap() + 0.0001)
		{
			cout << "ERROR : One route is exceeding the capacity limit" << endl ;
			throw string("ERROR : One route is exceeding the capacity limit");
		}
	}

	if( distance > solutionCost + 0.0001 || distance < solutionCost - 0.0001)
	{
		cout << "ERROR : Solution checker does not find the same solution cost" << endl ;
		cout << "Solution checker cost is = " << distance << endl;
		cout << "Solution cost is = " << solutionCost << endl;
	}
}

void Solution::PrintSolution(vector<Customer*> enc)
{

	cout << endl ;
	cout << "------------------------------------" << endl ;
	cout << "SOLUTION COST : " << std::setprecision(12) << solutionCost << endl ;
	cout << "NB ROUTES : " << numberOfRouteInSolution << endl ;
	int start, end;
	for(int i = 0; i < numberOfRouteInSolution; i++)
	{
		start = tour[i];
		if( i < numberOfRouteInSolution-1)
			end = tour[i+1];
		else
			end = enc.size()+1;

		cout << "TOUR[" << i << "] = {" ;
		for(int j = start; j < end; j++)
			cout << enc[j-1]->getId() << " " ;
		cout << "}" << endl;

	}
	cout << "------------------------------------" << endl ;
	cout << endl ;
}

vector<int> Solution::getTourStructure()
{
	return tour;
}

double Solution::getObjVal()
{
	return solutionCost;
}

int Solution::getNumberOfRoutesInSolution()
{
	return numberOfRouteInSolution;
}
