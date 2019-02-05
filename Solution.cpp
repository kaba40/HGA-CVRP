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
}

Solution::~Solution()
{
	// TODO Auto-generated destructor stub
}

void Solution::setRandomSequence()
{
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

		// fill Tour vector containing indices of i where a tour start
		nbTour = tsp_data->getNumberNodes()-1;
		tour = vector<int>(numberOfRouteInSolution);
		for (int i = numberOfRouteInSolution-1 ; i >= 0 ; i --)
		{
			nbTour = pred[nbTour] ;
			tour[i] = nbTour+1;
		}
	}
#ifdef DEBUG_DecodSol
	for(uint i = 0; i < tour.size(); i++)
		cout << "tour[ " << i << "] =" << tour[i] << endl;
#endif

	return returValue;

}

bool Solution::EVAL2(double *cost, SeqData *seq1, SeqData *seq2)
{

#ifdef DEBUG_EvalSol
	cout << "seqFirst_Id = " << seq1->getHead()->getClient()->getId() << endl;
#endif

	// feasibility evaluation for the concatenation of two sub-sequences
	int totalLoad;
	totalLoad = seq1->getDemand() + seq2->getDemand();

	if(totalLoad > tsp_data->getVehicleCap() + 0.0001)
	{
		return false;
	}

	// cost evaluation for the concatenation of two sub-sequences
	double totalCost;
	double partialCostSeq1, partialCostSeq2;
	double joinCost; // distance between last element of seq1 and first element of seq2

	// compute partialCostSeq1
	if(seq1->getHead()->getClient()->getDemand() == 0)
	{
		partialCostSeq1 = seq1->getDistance();
	}
	else
	{
		partialCostSeq1 = seq1->getHead()->getClient()->getDistanceDepot() + seq1->getDistance();
	}

	// compute partialCostSeq2
	if(seq2->getTail()->getClient()->getDemand() == 0)
	{
		partialCostSeq2 = seq2->getDistance();
	}
	else
	{
		partialCostSeq2 = seq2->getTail()->getClient()->getDistanceDepot() + seq2->getDistance();
	}

	// compute joinCost (distance between last element of seq1 and first element of seq2)
	if(seq1->getTail() != seq2->getHead())
	{
		if(seq1->getTail()->getClient()->getDemand() == 0 && seq2->getHead()->getClient()->getDemand() != 0)
		{
			joinCost = seq2->getHead()->getClient()->getDistanceDepot();
		}
		else if(seq1->getTail()->getClient()->getDemand() != 0 && seq2->getHead()->getClient()->getDemand() == 0)
		{
			joinCost = seq1->getTail()->getClient()->getDistanceDepot();
		}
		else
		{
			joinCost = seq1->getTail()->getClient()->getDistance(seq2->getHead()->getClient());
		}

	}
	else
	{
		joinCost = 0;
	}

	// compute totalCost
	totalCost = partialCostSeq1 + joinCost + partialCostSeq2;

	*cost =  totalCost;

	return true;
}

bool Solution::EVALN(double *cost, int numSeq, SeqData *seq, ...) // add a number of parameter
{

	va_list args;
	va_start(args, seq);

#ifdef DEBUG_EvalSol
	cout << "seqFirst_Id = " << seq->getHead()->getClient()->getId() << endl;
#endif

	// feasibility evaluation for the concatenation of two sub-sequences
	int totalLoad = 0 ;
	totalLoad += seq->getDemand();

	// cost evaluation for the concatenation of two sub-sequences (compute totalDistance resulting from the concatenation of seq and seqNext)

	double totalDistance = 0;

	// add first sequence distance to totalDistance
	if(seq->getHead()->getClient()->getDemand() == 0)
	{
		totalDistance += seq->getDistance();
	}
	else
	{
		totalDistance += seq->getHead()->getClient()->getDistanceDepot() + seq->getDistance();
	}

	bool lastSeq1 = false;
	SeqData* seqCopy = new SeqData(seq); // make a copy of the first sub-sequence

	for(int i = 0; i < numSeq-1; i++)
	{
		SeqData *seq1 = va_arg(args, SeqData*);
		totalLoad += seq1->getDemand();

		if(totalLoad > tsp_data->getVehicleCap() + 0.0001)
		{
			return false;
		}

		// add second sequence distance to totalDistance
		if(seq1->getTail()->getClient()->getDemand() == 0)
		{
			totalDistance += seq1->getDistance();
			lastSeq1 = true;
		}
		else
		{
			totalDistance +=  seq1->getDistance();
		}

		// compute joinCost between seqCopy and seq1 and add it to totalDistance
		if(lastSeq1 == true)
		{
			// compute distance between seq tail and seq1 head then add it to totalDistance
			if(seqCopy->getTail() != seq1->getHead())
			{
				if(seqCopy->getTail()->getClient()->getDemand() == 0 && seq1->getHead()->getClient()->getDemand() != 0)
				{
					totalDistance += seq1->getHead()->getClient()->getDistanceDepot();
				}
				else if(seqCopy->getTail()->getClient()->getDemand() != 0 && seq1->getHead()->getClient()->getDemand() == 0)
				{
					totalDistance += seqCopy->getTail()->getClient()->getDistanceDepot();
				}
				else
				{
					totalDistance += seqCopy->getTail()->getClient()->getDistance(seq1->getHead()->getClient());
				}
			}
		}
		else
		{
			// compute distance between seq tail and seq1 head then add it to totalDistance
			if(seqCopy->getTail() != seq1->getHead())
			{
				if(seqCopy->getTail()->getClient()->getDemand() == 0 && seq1->getHead()->getClient()->getDemand() != 0)
				{
					totalDistance += seq1->getHead()->getClient()->getDistanceDepot();
				}
				else if(seqCopy->getTail()->getClient()->getDemand() != 0 && seq1->getHead()->getClient()->getDemand() == 0)
				{
					totalDistance += seqCopy->getTail()->getClient()->getDistanceDepot();
				}
				else
				{
					totalDistance += seqCopy->getTail()->getClient()->getDistance(seq1->getHead()->getClient());
				}
			}
			// move seqCopy tail to seq1 tail
			seqCopy->setTail(seq1->getTail());
		}
	}

	*cost = totalDistance;

	va_end(args);

	return true;
}


void Solution::CheckSolution()
{
	int start, end;
	double load;
	double distance = 0;

	if(tour[0] != 1)
		cout << " Error : the first route should start with the first customer" << endl;
#ifdef DEBUG_CheckSol
	cout << "numberOfRouteInSolution = " << numberOfRouteInSolution << endl;
#endif
	for(int i = 0; i < numberOfRouteInSolution; i++)
	{
		load = 0;
		start = tour[i];
		if( i < numberOfRouteInSolution-1)
			end = tour[i+1];
		else
			end = encodage->getSize() +1; //encodage.size()+1;
#ifdef DEBUG_CheckSol
		cout << "encodage->getSize() in Solution =" << encodage->getSize() << endl;
#endif

		Node *routeStartNodeFirst = encodage->find(start-1);
		for(int j = start; j < end; j++)
		{
#ifdef DEBUG_CheckSol
			cout << "numberOfRouteInSolution= " << numberOfRouteInSolution << endl;
			cout << "j= " << j << endl;
#endif
			load += routeStartNodeFirst->getClient()->getDemand(); //encodage->find(j-1);
			routeStartNodeFirst = routeStartNodeFirst->getNext();
		}


		Node *routeStartNodeSecond = encodage->find(start-1);
		distance +=  routeStartNodeSecond->getClient()->getDistanceDepot() + encodage->find(end-2)->getClient()->getDistanceDepot();//encodage[start-1]->getDistanceDepot() + encodage[end-2]->getDistanceDepot();
		for(int j = start; j < end-1; j++)
		{
//			int k = j-1;
//			int ksuiv = k+1;
			distance += routeStartNodeSecond->getClient()->getDistance(routeStartNodeSecond->getNext()->getClient());//encodage->find(k)->getClient()->getDistance(encodage->find(ksuiv)->getClient());
			routeStartNodeSecond = routeStartNodeSecond->getNext();
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

		// create depot customer and node
		Customer* depotCustomer = new Customer("0",-1, 0, NULL); // depot customer
		Node* depotNode = new Node(depotCustomer); // depot associated node
		Node* dNodeFirst = depotNode;
		Node* dNodeLast = depotNode;

		routeSeq[i] = new Node();
//		routeSeq[i]->push_back(depotNode);

		cout << "TOUR[" << i << "] = {" ;
		cout << "encodage.size = " << encodage->getSize() << endl;
		Node *routeStartNode = encodage->find(start-1);
		cout << "routeStartNode1 = " << routeStartNode->getClient()->getId() << endl;
		Node *routeNodeFirst = routeStartNode;
		cout << "routeSNode1 = " << routeNodeFirst->getClient()->getId() << endl;
		dNodeFirst->setNext(routeNodeFirst);
		routeNodeFirst->setPrevious(dNodeFirst);
		for(int j = start; j < end; j++)
		{

	//		cout << encodage->find(j-1)->getClient()->getId() << " " ;//cout << encodage[j-1]->getId() << " " ;
			cout << routeStartNode->getClient()->getId() << " " << endl;

//			routeSeq[i]->push_back(copyNode); // add dummy depot node at the beginning and end of routeSeq

			routeStartNode = routeStartNode->getNext();
		}
		Node *routeNodeLast = routeStartNode->getPrevious();
//		routeNodeLast->setNext(dNodeLast);
//		dNodeLast->setPrevious(routeNodeFirst);
		//cut the link
		cout << "routeStartNode2 = " << routeStartNode->getClient()->getId() << endl;
		cout << "routeStartNode2Next = " << routeStartNode->getNext()->getClient()->getId() << endl;
		routeNodeLast->setNext(NULL);
		routeStartNode->setPrevious(NULL);

		routeNodeLast->setNext(dNodeLast);
		dNodeLast->setPrevious(routeNodeLast);


		routeSeq[i] = dNodeFirst;

		cout << "}"  << endl;

	}
	cout << "------------------------------------" << endl ;
	cout << endl ;
}

DLinkedList* Solution::getSequence()
{
	return encodage;
}

vector<Node*> Solution::getRouteSequence()
{
	return routeSeq;
}

void Solution::updateRoute(int numRoute, Node* rteSeq)
{
	routeSeq[numRoute] = rteSeq;
}

//void Solution::initRouteSetSubSeq() // to modify when routeSeq modified
//{
//	routeForwardSeq.resize(numberOfRouteInSolution);
//	routeBackwardSeq.resize(numberOfRouteInSolution);
//	for(int i = 0; i < numberOfRouteInSolution; i++) // number of route
//	{
//		routeForwardSeq[i].resize(routeSeq[i]->getSize()); //each route contains 2 dummy nodes depot-depot
//		routeBackwardSeq[i].resize(routeSeq[i]->getSize());
//
//		int j = 0;
//		for(Node *seqNode = routeSeq[i]->getHead(); seqNode != NULL; seqNode = seqNode->getNext())
////		for(uint j = 0; j < routeForwardSeq[i].size(); j++) // number of nodes in a route
//		{
//
//#ifdef DEBUG_Sol
//			cout << "this[" << i << "][" << j << "]= " << seqNode->getClient()->getId() << endl;
//#endif
////			SeqData* seq = new SeqData(routeSeq[i][j]); // first one visit sub-sequence
//			SeqData* seq = new SeqData(seqNode);
//			SeqData* retFor = NULL;
//			SeqData* retBack = NULL;
//
//			routeForwardSeq[i][j].push_back(seq);
//			routeBackwardSeq[i][j].push_back(seq);
//
////			int k = j+1;
//			for(Node *seqNodeNext = seqNode->getNext(); seqNodeNext != NULL; seqNodeNext = seqNodeNext->getNext())
////			for(uint k = j+1; k < routeForwardSeq[i].size(); k++)
//			{
//#ifdef DEBUG_Sol
//				cout << "seq[" << i << "][" << j+1 << "]= " << seqNodeNext->getClient()->getId() << " ";
//#endif
//				SeqData* seqNext = new SeqData(seqNodeNext); // all visits after seq
//
//				if(seqNodeNext == seqNode->getNext())//if(k == j+1)
//				{
//					retFor = seq->concatForWard(seqNext); // concatenate forward seq and seqNext
//					retBack = seq->concatBackWard(seqNext); // concatenate backward seq and seqNext
//
//				}
//				else
//				{
//					retFor = retFor->concatForWard(seqNext); // concatenate forward retFor and seqNext
//					retBack = retBack->concatBackWard(seqNext); // concatenate backward retFor and seqNext
//				}
//
//				routeForwardSeq[i][j].push_back(retFor);
//				routeBackwardSeq[i][j].push_back(retBack);
//
////				k++;
//			}
//#ifdef DEBUG_Sol
//			cout << endl;
//#endif
//			j++;
//		}
//#ifdef DEBUG_Sol
//			cout << endl;
//#endif
//	}
//}
//
//void Solution::updateOneRouteSetSubSeq(int numRoute) // modify to update routeForwardSeq
//{
//	int j = 0;
//	for(Node *seqNode = routeSeq[numRoute]; seqNode != NULL; seqNode = seqNode->getNext())
////	for(uint j = 0; j < routeForwardSeq[numRoute].size()-1; j++) // number of nodes in a route
//	{
//		routeForwardSeq[numRoute][j].clear(); // clear vector containing route nodes
//		routeBackwardSeq[numRoute][j].clear();
//#ifdef DEBUG_Sol
//		// routeSeq[numRoute][j] is the updated route
//		cout << "this[" << numRoute << "][" << j << "]= " << seqNode->getClient()->getId() << endl;
//#endif
//		SeqData* seq = new SeqData(seqNode); // first one visit sub-sequence
//		SeqData* retFor = NULL;
//		SeqData* retBack = NULL;
//
//		routeForwardSeq[numRoute][j].push_back(seq);
//		routeBackwardSeq[numRoute][j].push_back(seq);
//
//		for(Node *seqNodeNext = seqNode->getNext(); seqNodeNext != NULL; seqNodeNext = seqNodeNext->getNext())
////		for(uint k = j+1; k < routeForwardSeq[numRoute].size(); k++)
//		{
//#ifdef DEBUG_Sol
//		cout << "seq[" << numRoute << "][" << j+1 << "]= " << seqNodeNext->getClient()->getId() << " ";
//#endif
//		SeqData* seqNext = new SeqData(seqNodeNext); // all visits after seq
//
//		if(seqNodeNext == seqNode->getNext())//if(k == j+1)
//		{
//			retFor = seq->concatForWard(seqNext); // concatenate forward seq and seqNext
//			retBack = seq->concatBackWard(seqNext); // concatenate backward seq and seqNext
//
//		}
//		else
//		{
//			retFor = retFor->concatForWard(seqNext); // concatenate forward retFor and seqNext
//			retBack = retBack->concatBackWard(seqNext); // concatenate backward retFor and seqNext
//		}
//
//		routeForwardSeq[numRoute][j].push_back(retFor);
//		routeBackwardSeq[numRoute][j].push_back(retBack);
//
//		}
//		cout << endl;
//		j++;
//	}
//}

vector<vector<vector<SeqData*>>> Solution::getRouteForwSeq() // to test sequenceTab in the main method
{
	return routeForwardSeq;
}

vector<vector<vector<SeqData*>>> Solution::getRouteBackSeq()
{
	return this->routeBackwardSeq;
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
