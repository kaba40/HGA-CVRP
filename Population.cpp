/*
 * Population.cpp
 *
 *  Created on: 26 mars 2019
 *      Author: kkeita
 */

#include "Population.hpp"

Population::Population(DataAP *vrp_dat, int numInds)
{
	this->cvrp_data = vrp_dat;
	this->nbIndivs = numInds;

	// create a new solution (individual)
	Solution *individu = new Solution(cvrp_data, cvrp_data->getCustomerList()) ;
	// generate a random sequence of customers

	for(int i = 0; i < nbIndivs; i++)
	{

		srand(unsigned (time(0)));
		individu->setRandomSequence();
		individu->getSequence()->show();
		// apply a decoding to evaluate the individual
		// true means we use direct encoding to do local search
		if(individu->Decodage(true))
		{
			individu->CheckSolution(true);
			individu->PrintSolution(true);
		}

		cout << "individu[" << i << "] " << "cost = " << individu->getObjVal() << endl;
		this->individus.push_back(individu);
	}

	 delete individu;
}

Population::~Population(){
	// TODO Auto-generated destructor stub
}

// add an individu to the population
void Population::addIndividu(Solution *individu)
{
	this->individus.push_back(individu);
}
vector<Solution*> Population::getIndividus()
{
	return this->individus;
}

bool Population::Education(Solution *individu)
{
//	cout << "initial individu cost " << individu->getObjVal() << endl;
//	double initObj = individu->getObjVal();

	// return value
	bool retVal = false;
	// r1 is the number of routes including the empty ones
	for(int r1 = 0; r1 < individu->getDataAP()->getNumberNodes()-1; r1++)
	{
		// record each route cost
		double costR1 = individu->getRouteForwSeq()[r1][0].back()->getDistance();
#ifdef DEBUG_EducPop
		cout << "costR1 = " << costR1 << endl;
#endif
		bool breakIfImproved = false; // stop InterRouteInsert move for a route if improved
		// r2 is the number of routes including the empty ones
		for(int r2 = 0; r2 <  individu->getDataAP()->getNumberNodes()-1; r2++)
		{
			if(r1 != r2)
			{
				double costR2 = individu->getRouteForwSeq()[r2][0].back()->getDistance();
#ifdef DEBUG_EducPop
				cout << "costR2 = " << costR2 << endl;
#endif
				Node *insertNodeR1Prev = NULL;
				/* c1 is the index of clients to insert in the route r2, e.g, (0--a--b--c--0) */
				int c1 = 1;
				for(Node *insertNodeR1 = individu->getRoutes()[r1].first->getNext(); !insertNodeR1->isDepot() && breakIfImproved == false; insertNodeR1 = insertNodeR1->getNext(), c1++)
				{
#ifdef DEBUG_EducPop
				cout << "----------- insertNodeR1 " << " " << " = " << insertNodeR1->getClient()->getId() << " ----------" <<  " c1 = " << c1 << endl;
#endif
					insertNodeR1Prev = insertNodeR1->getPrevious();
#ifdef DEBUG_EducPop
						if(insertNodeR1Prev != NULL)
							cout << "insertNodeR1Prev = " << insertNodeR1Prev->getClient()->getId() <<  endl;
#endif

					// find the 2 sub-sequences
					SeqData *seq1 = NULL;
					SeqData *seq2 = NULL;

					seq1 = individu->getRouteForwSeq()[r1][0][c1-1];
#ifdef DEBUG_EducPop
					cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
					int indexLastNodeRouteR1 = individu->getNbClientsForRoute(r1)+1;
					seq2 = individu->getRouteForwSeq()[r1][c1+1][indexLastNodeRouteR1 - (c1+1)];
#ifdef DEBUG_EducPop
					cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
					double newCostR1, reduceCostR1; // double newCostR1;
					if(individu->EVAL2(&newCostR1, seq1, seq2))
					{
#ifdef DEBUG_EducPop
						cout << "newCostR1 = " << newCostR1 << endl;
#endif
						reduceCostR1 = newCostR1 - costR1;
#ifdef DEBUG_EducPop
						cout << "reduceCostR1 = " << reduceCostR1 << endl;
#endif
					}
					// index of moveNode in route r2
					int c2 = 0;
					for(Node *moveNodeR2 = individu->getRoutes()[r2].first; !moveNodeR2->isLastDepot() && breakIfImproved == false; moveNodeR2 = moveNodeR2->getNext(), c2++)
					{
#ifdef DEBUG_EducPop
					cout << "---------- moveNodeR2 " << " " << " = " << moveNodeR2->getClient()->getId() << " ---------- " << " c2 = " << "" << c2 << endl;
#endif
						// find the 3 sub-sequences
						SeqData *seq3 = NULL;
						SeqData *seq4 = NULL;
						SeqData *seq5 = NULL;


						seq3 = individu->getRouteForwSeq()[r2][0][c2];
#ifdef DEBUG_EducPop
						cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
						seq4 = individu->getRouteForwSeq()[r1][c1][0];
#ifdef DEBUG_EducPop
						cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
						int indexLastNodeRouteR2 = individu->getNbClientsForRoute(r2)+1;
						seq5 = individu->getRouteForwSeq()[r2][c2+1][indexLastNodeRouteR2 - (c2+1)];
#ifdef DEBUG_EducPop
						cout << seq5->getHead()->getClient()->getId() << "--" << seq5->getTail()->getClient()->getId() << endl;
#endif
						 double newCostR2, addCostR2, newObjVal; // double newCostR2,  newObjVal;
						if(individu->EVALN(&newCostR2, 3, seq3, seq4, seq5))
						{
#ifdef DEBUG_EducPop
							cout << "newCostR2 = " << newCostR2 << endl;
#endif
							addCostR2 = newCostR2 - costR2;
#ifdef DEBUG_EducPop
							cout << "addCostR2 = " << addCostR2 << endl;
							cout << "oldObjVal = " << individu->getObjVal() << endl;
#endif
							newObjVal = individu->getObjVal() + reduceCostR1 + addCostR2;
#ifdef DEBUG_EducPop
							cout << "newObjVal = " << newObjVal << endl;
#endif
							if(newObjVal < individu->getObjVal() - 0.0001 || newObjVal > individu->getObjVal() + 0.0001)
							{
								// remove insertNodeR1
								insertNodeR1->removeNode();

								// insert insertNodeR1 after moveNodeR2
								moveNodeR2->insertAfter(insertNodeR1);
								// update R1 number of nodes
#ifdef DEBUG_EducPop
								cout << " r1 = " << r1 << endl;
								cout << "this->initSol->getRoutes()[ " << r1 << " ].second1 = " << individu->getRoutes()[r1].second << endl;
#endif
								int numR1 = individu->getRoutes()[r1].second -1;
								individu->updateRouteNbNodes(r1,numR1);
#ifdef DEBUG_EducPop
								cout << "this->initSol->getRoutes()[ " << r1 << " ].second2 = " << individu->getRoutes()[r1].second << endl;
#endif
								// update R2 number of nodes
#ifdef DEBUG_EducPop
								cout << " r2 = " << r2 << endl;
								cout << "this->initSol->getRoutes()[ " << r2 << " ].second1 = " << individu->getRoutes()[r2].second << endl;
#endif
								int numR2 = individu->getRoutes()[r2].second +1;
								individu->updateRouteNbNodes(r2,numR2);
#ifdef DEBUG_EducPop
								cout << "this->initSol->getRoutes()[ " << r2 << " ].second2 = " << individu->getRoutes()[r2].second << endl;
#endif

								// update R1 and R2 route sub-sequences
								individu->updateOneRouteSetSubSeq(r1);
								individu->updateOneRouteSetSubSeq(r2);

								// update objective value
								individu->setObjVal(newObjVal);

								breakIfImproved = true;
								retVal = true;
							}
						}
					}
				}
			}
		}
	}
	return retVal;
}
