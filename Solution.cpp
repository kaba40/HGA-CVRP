/*
 * Solution.cpp
 *
 *  Created on: 27 nov. 2018
 *      Author: kkeita
 */

#include "Solution.hpp"



Solution::Solution(DataAP *tsp_dat, DLinkedList* encod)
{
	solutionCost = 0;
	numberOfRouteInSolution = 0;
	tsp_data = tsp_dat;
//	encodage = tsp_dat->getCustomers();
	encodage = encod;

	/* // kairaba: tu dois initialiser ici l'encodage
	   / ainsi la méthode Decodage ne prendra aucun argument
	   encoding = data_instance->getCustomers();
	//*/
}

Solution::~Solution()
{

  /* // kairaba : ce n'est pas ici qu'il faut détruire la liste des customers
     // si tu as plusieurs solutions, comment feras tu si tu détruis tous les customers ?
     // tu dois détruire les customers dans le destructeur de la classe DataAP
	for(int i = 0; i < tsp_data->getNumberNodes()-1; i++)
	{
		delete encodage[i];
	}
  //*/
	// TODO Auto-generated destructor stub
}

void Solution::setRandomSequence()
{
	// kairaba : avant d'utiliser random_shuffle, il faut exécuter la méthode
	// qui suit d'abord. Autrement, entre deux lancements de ton programme
	// on aura toujours le même vector
	// Enfin, la génération d'une séquence aléatoire doit être une méthode
	// de la classe solution. Cette méthode initialisera l'attribut encoding.
	vector<Customer*> encod = tsp_data->getCustomers();
	srand(unsigned (time(0)) );
	random_shuffle(encod.begin(), encod.end());

	encodage->delete_list();

	for(uint i = 0; i < encod.size(); i++)
	{
		Node *node = new Node(encod[i]);
		encodage->push_back(node);
	}

}

// kairaba : méthode sans argument
// enc : attribut de la classe. Donc tu y as accès
bool Solution::Decodage()
{
	bool returValue = false;
	SplitBellman *splitAlgo;
	splitAlgo = new SplitBellman(tsp_data, encodage);

	if(splitAlgo->solve())
	{
		returValue = true;

		// fill objVal

		solutionCost = splitAlgo->getPathCost()[tsp_data->getNumberNodes() -1];

		// count the number of routes -- numberOfRoutes
		int nbTour = tsp_data->getNumberNodes()-1;
		vector<int> pred = splitAlgo->getPredence();
		numberOfRouteInSolution = 0; // to remove
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

void Solution::CheckSolution()
{
	int start, end;
	double load;
	double distance = 0;

	if(tour[0] != 1)
		cout << " Error : the first route should start with the first customer" << endl;

//	cout << "numberOfRouteInSolution = " << numberOfRouteInSolution << endl;

	for(int i = 0; i < numberOfRouteInSolution; i++)
	{
		load = 0;
		start = tour[i];
		if( i < numberOfRouteInSolution-1)
			end = tour[i+1];
		else
			end = encodage->getSize() +1; //encodage.size()+1;

//		cout << "encodage->getSize() in Solution =" << encodage->getSize() << endl;
		for(int j = start; j < end; j++)
		{
//			cout << "numberOfRouteInSolution= " << numberOfRouteInSolution << endl;
//			cout << "j= " << j << endl;
			load += encodage->find(j-1)->getClient()->getDemand(); //encodage[j-1]->getDemand();
		}


		distance +=  encodage->find(start-1)->getClient()->getDistanceDepot() + encodage->find(end-2)->getClient()->getDistanceDepot();//encodage[start-1]->getDistanceDepot() + encodage[end-2]->getDistanceDepot();
		for(int j = start; j < end-1; j++)
		{
			int k = j-1;
			int ksuiv = k+1;
			distance += encodage->find(k)->getClient()->getDistance(encodage->find(ksuiv)->getClient());//encodage[k]->getDistance(encodage[ksuiv]);
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


void Solution::PrintSolution()
{

	cout << endl ;
	cout << "------------------------------------" << endl ;
	cout << "SOLUTION COST : " << std::setprecision(12) << solutionCost << endl ;
	cout << "NB ROUTES : " << numberOfRouteInSolution << endl ;
	int start, end;
	routeSeq.resize(numberOfRouteInSolution);
	for(int i = 0; i < numberOfRouteInSolution; i++)
	{

		start = tour[i];
		if( i < numberOfRouteInSolution-1)
			end = tour[i+1];
		else
			end = encodage->getSize() +1; //encodage.size()+1;

	//	routeSeq[i].push_back(dummyNode) tomorrow
		// create depot customer and node
		Customer* depotCustomer = new Customer("0",-1, 0, NULL); // depot customer
		Node* depotNode = new Node(depotCustomer); // depot associated node
		routeSeq[i].push_back(depotNode);
		cout << "TOUR[" << i << "] = {" ;
		for(int j = start; j < end; j++)
		{
			cout << encodage->find(j-1)->getClient()->getId() << " " ;//cout << encodage[j-1]->getId() << " " ;
			routeSeq[i].push_back(encodage->find(j-1)); // add dummy depot node at the beginning and end of routeSeq
		}
		routeSeq[i].push_back(depotNode);
		cout << "}" << endl;
		//	routeSeq[i].push_back(dummyNode)
	}
	cout << "------------------------------------" << endl ;
	cout << endl ;
}

DLinkedList* Solution::getSequence()
{
	return encodage;
}

vector<vector<Node*>> Solution::getRouteSequence()
{
	return routeSeq;
}

void Solution::updateRoute(int numRoute, vector<Node*> rteSeq)
{
	routeSeq[numRoute] = rteSeq;
}

void Solution::initRouteSetSubSeq()
{
	sequenceTab.resize(numberOfRouteInSolution);
	for(int i = 0; i < numberOfRouteInSolution; i++) // number of route
	{
		sequenceTab[i].resize(routeSeq[i].size()); //each route contains 2 dummy nodes
		for(uint j = 0; j < sequenceTab[i].size()-1; j++) // number of nodes in a route
		{
#ifdef DEBUG_Sol
			cout << "this[" << i << "][" << j << "]= " << routeSeq[i][j]->getClient()->getId() << endl;
#endif
			SeqData* seq = new SeqData(routeSeq[i][j]); // first one visit sub-sequence
			SeqData* ret = NULL;
			sequenceTab[i][j].push_back(seq);
			for(uint k = j+1; k < sequenceTab[i].size(); k++)
			{
#ifdef DEBUG_Sol
			cout << "seq[" << i << "][" << k << "]= " << routeSeq[i][k]->getClient()->getId() << " ";
#endif
			SeqData* seqNext = new SeqData(routeSeq[i][k]); // all visits after seq

			if(k == j+1)
				 ret = seq->concatForWard(seqNext); // concatenate seq and seqNext
			else
				ret = ret->concatForWard(seqNext); // concatenate ret and seqNext
			sequenceTab[i][j].push_back(ret);
			}
			cout << endl;
		}
		cout << endl;
	}
}

void Solution::updateOneRouteSetSubSeq(int numRoute)
{
	for(uint j = 0; j < sequenceTab[numRoute].size()-1; j++) // number of nodes in a route
	{
		sequenceTab[numRoute][j].clear(); // clear vector containing route nodes
#ifdef DEBUG_Sol
		// routeSeq[numRoute][j] is the updated route
		cout << "this[" << i << "][" << j << "]= " << routeSeq[numRoute][j]->getClient()->getId() << endl;
#endif
		SeqData* seq = new SeqData(routeSeq[numRoute][j]); // first one visit sub-sequence
		SeqData* ret = NULL;
		sequenceTab[numRoute][j].push_back(seq);
		for(uint k = j+1; k < sequenceTab[numRoute].size(); k++)
		{
#ifdef DEBUG_Sol
		cout << "seq[" << i << "][" << k << "]= " << routeSeq[numRoute][k]->getClient()->getId() << " ";
#endif
		SeqData* seqNext = new SeqData(routeSeq[numRoute][k]); // all visits after seq

		if(k == j+1)
			 ret = seq->concatForWard(seqNext); // concatenate seq and seqNext
		else
			ret = ret->concatForWard(seqNext); // concatenate ret and seqNext don't work
		sequenceTab[numRoute][j].push_back(ret);
		}
		cout << endl;
	}
}

vector<vector<vector<SeqData*>>> Solution::getRouteSetSubSeq() // to test sequenceTab in the main method
{
	return sequenceTab;
}

vector<int> Solution::getTourStructure()
{
	return tour;
}

double Solution::getObjVal()
{
	return solutionCost;
}

int Solution::getRoutesNumber()
{
	return numberOfRouteInSolution;
}

void Solution::setObjVal(double objVal)
{
	this->solutionCost = objVal;
}

void Solution::setTourStructure(vector<int> route)
{
	this->tour = route;
}

void Solution::setRouteNumber(int numRoute)
{
	this->numberOfRouteInSolution = numRoute;
}
