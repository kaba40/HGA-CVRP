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
	encodage = encod;
}

Solution::Solution(Solution *sol)
{
	this->encodage = sol->encodage;
	this->routeBackwardSeq = sol->routeBackwardSeq;
	this->routeForwardSeq = sol->routeForwardSeq;
	this->tour = sol->tour;
	this->routes = sol->routes;
	this->solutionCost = sol->solutionCost;
	this->numberOfRouteInSolution = sol->numberOfRouteInSolution;
	this->tsp_data = sol->tsp_data;
}

Solution::~Solution()
{
	// TODO Auto-generated destructor stub
}

void Solution::setRandomSequence()
{
	vector<Customer*> encod = tsp_data->getCustomers();
//	srand(unsigned (time(0)) );

	std::random_device rd;
	std::mt19937 g(rd());
	std::random_shuffle(encod.begin(), encod.end()); // random_shuffle(encod.begin(), encod.end());

	encodage->delete_list();

	for(uint i = 0; i < encod.size(); i++)
	{
		Node *node = new Node(encod[i]);
		encodage->push_back(node);
	}

}


bool Solution::Decodage(bool useDecoDirect)
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
        vector<Node*> predNode = splitAlgo->getPredenceNode();

		numberOfRouteInSolution = 0;
		while(nbTour != 0)
		{
			nbTour = pred[nbTour];
			numberOfRouteInSolution++;
		}

		// fill Tour vector containing indices of i where a tour start
		nbTour = tsp_data->getNumberNodes()-1;

		if(useDecoDirect)
		{
	        routes = vector<pair<Node*, uint>>(numberOfRouteInSolution);

	        Node* last = encodage->getTail(); // to record the last element of the route

			for (int i = numberOfRouteInSolution-1 ; i >= 0 ; i --)
			{

	            Customer* depotCustomerFirst = new Customer("0",-1, 0, NULL); // depot customer
	            Customer* depotCustomerlLast = new Customer("0",-1, 0, NULL); // depot customer

	            Node* depotNodeFirst = new Node(depotCustomerFirst); // depot associated node
	            Node* depotNodeLast = new Node(depotCustomerlLast); // depot associated node

	            Node* tmp = predNode[nbTour]->getPrevious();

	            predNode[nbTour]->setPrevious(depotNodeFirst);
	            depotNodeFirst->setNext(predNode[nbTour]);

	            last->setNext(depotNodeLast); // not last->setPrevious(depotNodeLast);
	            depotNodeLast->setPrevious(last); // not depotNodeLast->setNext(last);
#ifdef DEBUG_DecodSol
	            std::cout << predNode[nbTour]->getClient()->getId() << " " << last->getClient()->getId() << std::endl;
#endif
	            routes[i].first = depotNodeFirst;
	            last = tmp;

				nbTour = pred[nbTour] ;

			}

			// add new empty routes. Find first the upper bound of vehicle number
			routes.resize(tsp_data->getNumberNodes() -1);
			for(int i = numberOfRouteInSolution; i < tsp_data->getNumberNodes() -1; i++)
			{
	            Customer* depotCustomerFirst = new Customer("0",-1, 0, NULL); // depot customer
	            Customer* depotCustomerlLast = new Customer("0",-1, 0, NULL); // depot customer

	            Node* depotNodeFirst = new Node(depotCustomerFirst); // depot associated node
	            Node* depotNodeLast = new Node(depotCustomerlLast); // depot associated node

	            depotNodeFirst->setNext(depotNodeLast);
	            depotNodeLast->setPrevious(depotNodeFirst);

	            routes[i].first = depotNodeFirst;
			}
		}
		else
		{
			tour = vector<int>(numberOfRouteInSolution);
			for (int i = numberOfRouteInSolution-1 ; i >= 0 ; i --)
			{
				nbTour = pred[nbTour] ;
				tour[i] = nbTour+1;
			}
		}

#ifdef DEBUG_DecodSol
		if(useDecoDirect)
		{
		    for(uint i = 0; i < routes.size(); i++){
		        cout << "routeSeq[ " << i << "] =" << routes[i].first->getClient()->getId() << endl;
		    }
		}
		else
		{
		    for(uint i = 0; i < tour.size(); i++){
				cout << "tour[ " << i << "] =" << tour[i] << endl;
		    }
		}
#endif

	}

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
		else if(seq1->getTail()->getClient()->getDemand() == 0 && seq2->getHead()->getClient()->getDemand() == 0)
		{
			joinCost = 0;
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


void Solution::CheckSolution(bool useDecoDirect)
{

	double load ;
	double distance = 0;

	if(useDecoDirect)
	{
#ifdef DEBUG_CheckSol
		cout << "routeSeq.size() = " << routes.size() << endl;
#endif
	    for(uint i = 0; i < routes.size(); i++)
	    {
	    	load = 0;

	    	int j = 0;
	        for( Node* routeNode = routes[i].first; routeNode != NULL; routeNode = routeNode->getNext())
	        {
	        	j++;
	        	if(routeNode->getClient()->getDemand() != 0 && routeNode->getNext()->getClient()->getDemand() != 0 )
	        	{
#ifdef DEBUG_CheckSol
	        		cout << "routeNode = " << routeNode->getClient()->getId() << endl;
	        		cout << "routeNode->getNext() = " << routeNode->getNext()->getClient()->getId() << endl;
#endif
	        		distance += routeNode->getClient()->getDistance(routeNode->getNext()->getClient());
	        	}
	        	else
	        	{
	        		if(routeNode->getNext() != NULL)
	        		{
		        		if(routeNode->getClient()->getDemand() == 0 && routeNode->getNext()->getClient()->getDemand() != 0 )
		        		{
		        			distance += routeNode->getNext()->getClient()->getDistanceDepot();
		        		}
		        		else if(routeNode->getClient()->getDemand() != 0 && routeNode->getNext()->getClient()->getDemand() == 0 )
		        		{
		        			distance += routeNode->getClient()->getDistanceDepot();
		        		}
	        		}
	        	}
	        	load += routeNode->getClient()->getDemand();
	        }
	        routes[i].second = j;

			if(load > tsp_data->getVehicleCap() + 0.0001)
			{
				cerr << "ERROR : One route is exceeding the capacity limit" << endl ;
				throw string("ERROR : One route is exceeding the capacity limit");
//				exit(-1); //return;
			}
	    }
	}
	else
	{
		int start, end;

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
				if(routeStartNodeFirst != NULL) // to remove
					cout << "routeStartNodeFirst = " << routeStartNodeFirst->getClient()->getId() << endl;
	#endif
				if(routeStartNodeFirst != NULL) // to remove
					load += routeStartNodeFirst->getClient()->getDemand();
				routeStartNodeFirst = routeStartNodeFirst->getNext();
			}

			if(load > tsp_data->getVehicleCap() + 0.0001)
			{
				cerr << "ERROR : One route is exceeding the capacity limit" << endl ;
				throw string("ERROR : One route is exceeding the capacity limit");
//				exit(-1); //return;
			}

			Node *routeStartNodeSecond = encodage->find(start-1);
			distance +=  routeStartNodeSecond->getClient()->getDistanceDepot() + encodage->find(end-2)->getClient()->getDistanceDepot();//encodage[start-1]->getDistanceDepot() + encodage[end-2]->getDistanceDepot();
			for(int j = start; j < end-1; j++)
			{
				distance += routeStartNodeSecond->getClient()->getDistance(routeStartNodeSecond->getNext()->getClient());//encodage->find(k)->getClient()->getDistance(encodage->find(ksuiv)->getClient());
				routeStartNodeSecond = routeStartNodeSecond->getNext();
			}
		}
	}

	if( distance > solutionCost + 0.0001 || distance < solutionCost - 0.0001)
	{
		cerr << "ERROR : Solution checker does not find the same solution cost" << endl ;
		cerr << "Solution checker cost is = " << distance << endl;
		cerr << "Solution cost is = " << solutionCost << endl;
		throw string("Check solutionCost");//return;
	}
}

void Solution::PrintSolution(bool useDecoDirect)
{

	cout << endl ;
	cout << "------------------------------------" << endl ;
	cout << "SOLUTION COST : " << std::setprecision(12) << solutionCost << endl ;

	if(useDecoDirect)
	{
		int numRoute = 0;
	    for(uint i = 0; i < routes.size(); i++)
	    {
	    	if(routes[i].second > 2)
	    	{
	    		numRoute++;
		        cout << "Route[" << i << "]: ";
		        Node* routeNode;

		        for( routeNode = routes[i].first; routeNode->getNext() != NULL; routeNode = routeNode->getNext())
		        {
		            cout << routeNode->getClient()->getId() << "--" ;
		        }
		        cout << routeNode->getClient()->getId() << " contains " <<  routes[i].second  << " nodes" << endl;
	    	}
	    }
	    cout << "NB ROUTES : " << numRoute << endl;
	}
	else
	{
		cout << "NB ROUTES : " << numberOfRouteInSolution << endl;
		int start, end;
		for(int i = 0; i < numberOfRouteInSolution; i++)
		{
			start = tour[i];
			if( i < numberOfRouteInSolution-1)
			{
				end = tour[i+1];
			}
			else
			{
				end = encodage->getSize() +1;
			}
			cout << "TOUR[" << i << "] = {" ;
			Node *routeStartNode = encodage->find(start-1);
			for(int j = start; j < end-1; j++)
			{
				cout << routeStartNode->getClient()->getId() << "--";

				routeStartNode = routeStartNode->getNext();
			}
			cout << routeStartNode->getClient()->getId() ;
			cout << "}"  << endl;
		}
	}
	cout << "------------------------------------" << endl ;
	cout << endl ;
}

// getter methods

DLinkedList* Solution::getSequence()
{
	return encodage;
}

DataAP* Solution::getDataAP()
{
	return tsp_data;
}

vector<pair<Node*,uint>> Solution::getRoutes()
{
	return routes;
}

vector<vector<vector<SeqData*>>> Solution::getRouteBackSeq() // to test sequenceTab in the main method
{
	return routeBackwardSeq;
}

vector<vector<vector<SeqData*>>> Solution::getRouteForwSeq()
{
	return this->routeForwardSeq;
}

vector<int> Solution::getTourStructure()
{
	return tour;
}

double Solution::getObjVal() const
{
	return solutionCost;
}

int Solution::getRoutesNumber()
{
	return numberOfRouteInSolution;
}

int Solution::getNbClientsForRoute(int r)
{
	return routes[r].second-2;
}

void Solution::initRouteSetSubSeq()
{
	routeBackwardSeq.resize(routes.size());
	routeForwardSeq.resize(routes.size());
	for(uint i = 0; i < routes.size(); i++) // number of routes
	{
		routeBackwardSeq[i].resize(routes[i].second); //each route contains 2 dummy nodes depot-depot
		routeForwardSeq[i].resize(routes[i].second);

		int j = 0;
		for(Node *seqNode = routes[i].first; seqNode != NULL; seqNode = seqNode->getNext(), j++)
		{
#ifdef DEBUG_Sol
			cout << "this[" << i << "][" << j << "]= " << seqNode->getClient()->getId() << endl;
#endif
			SeqData* seq = new SeqData(seqNode);
			SeqData* retBack = NULL;
			SeqData* retFor = NULL;

			routeBackwardSeq[i][j].push_back(seq);
			routeForwardSeq[i][j].push_back(seq);

			for(Node *seqNodeNext = seqNode->getNext(); seqNodeNext != NULL; seqNodeNext = seqNodeNext->getNext())
			{
#ifdef DEBUG_Sol
				cout << "seq[" << i << "][" << j+1 << "]= " << seqNodeNext->getClient()->getId() << " ";
#endif
				SeqData* seqNext = new SeqData(seqNodeNext); // all visits after seq

				if(seqNodeNext == seqNode->getNext())
				{
					retBack = seq->concatBackWard(seqNext); // concatenate forward seq and seqNext
					retFor = seq->concatForWard(seqNext); // concatenate backward seq and seqNext
				}
				else
				{
					retBack = retBack->concatBackWard(seqNext); // concatenate forward retFor and seqNext
					retFor = retFor->concatForWard(seqNext); // concatenate backward retFor and seqNext
				}
				routeBackwardSeq[i][j].push_back(retBack);
				routeForwardSeq[i][j].push_back(retFor);
			}
#ifdef DEBUG_Sol
			cout << endl;
#endif
		}
#ifdef DEBUG_Sol
			cout << endl;
#endif
	}
}

// update methods

void Solution::updateOneRouteSetSubSeq(int numRoute)
{
	//resize the second dimension of vectors to take into account the variation of the number of client in route numRoute
	routeBackwardSeq[numRoute].resize(routes[numRoute].second);
	routeForwardSeq[numRoute].resize(routes[numRoute].second);

	int j = 0;
	for(Node *seqNode = routes[numRoute].first; seqNode != NULL; seqNode = seqNode->getNext(), j++)// number of nodes in a route
	{
		// clear vector containing route nodes
		routeBackwardSeq[numRoute][j].clear();
		routeForwardSeq[numRoute][j].clear();
#ifdef DEBUG_Sol
		// routeSeq[numRoute][j] is the updated route
		cout << "this[" << numRoute << "][" << j << "]= " << seqNode->getClient()->getId() << endl;
#endif
		SeqData* seq = new SeqData(seqNode); // first single visit
		SeqData* retBack = NULL;
		SeqData* retFor = NULL;

		routeBackwardSeq[numRoute][j].push_back(seq);
		routeForwardSeq[numRoute][j].push_back(seq);

		for(Node *seqNodeNext = seqNode->getNext(); seqNodeNext != NULL; seqNodeNext = seqNodeNext->getNext())
		{
#ifdef DEBUG_Sol
			cout << "seq[" << numRoute << "][" << j+1 << "]= " << seqNodeNext->getClient()->getId() << " ";
#endif
			SeqData* seqNext = new SeqData(seqNodeNext); // all visits after seq

			if(seqNodeNext == seqNode->getNext())
			{
				retBack = seq->concatBackWard(seqNext); // concatenate backward seq and seqNext
				retFor = seq->concatForWard(seqNext); // concatenate forward seq and seqNext
			}
			else
			{
				retBack = retBack->concatBackWard(seqNext); // concatenate backward retFor and seqNext
				retFor = retFor->concatForWard(seqNext); // concatenate forward  retFor and seqNext
			}
			routeBackwardSeq[numRoute][j].push_back(retBack);
			routeForwardSeq[numRoute][j].push_back(retFor);
		}
#ifdef DEBUG_Sol
			cout << endl;
#endif
	}
}

void Solution::updateRouteNbNodes(int numRoute, int numNodes)
{
	this->routes[numRoute].second = numNodes;
}

void Solution::restoreSequence()
{

//	cout << "print solution before concat = " ; PrintSolution(true); cout << endl;

	Node* tail = NULL;
	bool tailInit = false;
	for(uint r = 0; r < routes.size() && tailInit == false; r++)
	{
		if(routes[r].second > 2) // if route has at least one client
		{
			tail = routes[r].first->getNext();
			tailInit = true;
		}
	}

//	cout << "tail = " << tail->getClient()->getId() << endl; //	cout << std::flush;

	encodage->setSize(0);
	encodage->setHead(NULL);
	encodage->setTail(NULL);

	for(uint r = 0; r < routes.size(); r++)
	{
		if(routes[r].second > 2) // if route has at least one client
		{
			Node *tmp = tail->getNext();
//			cout << "tmp = " << tmp->getClient()->getId() << endl;
			encodage->push_back(tail);
			for(Node *routeNode = tmp; !tmp->isDepot();)
			{
				routeNode = tmp;
				tmp = routeNode->getNext();
				encodage->push_back(routeNode);
			}
			bool tailUpdate = false;
			for(uint j = r+1; j < routes.size() && tailUpdate == false; j++)
			{
				if(routes[j].second > 2)
				{
					tail = routes[j].first->getNext();
					tailUpdate = true;
				}
			}
		}
	}

	routes.clear();
	tour.clear();
	solutionCost = 0;
	numberOfRouteInSolution = 0;
}

// setter methods

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

// overloading operator<
bool operator<( const Solution &sol1, const Solution &sol2)
{
	return sol1.getObjVal() < sol2.getObjVal();
}
