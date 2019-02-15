/*
 * LocalSearch.cpp
 *
 *  Created on: 19 déc. 2018
 *      Author: kkeita
 */

#include "LocalSearch.hpp"

LocalSearch::LocalSearch(Solution *initSol)
{
	this->initSol = initSol;
}

LocalSearch::~LocalSearch() {
	// TODO Auto-generated destructor stub
}

// movements for indirect encoding, i.e, giant tour
bool LocalSearch::Insert()
{
#ifdef DEBUG_Insert
	cout << endl;
	cout << "local search insert initial sequence = " ; this->initSol->getSequence()->show() ; cout << endl;
#endif
	double initialObjVal = this->initSol->getObjVal();
	int numberRoute = this->initSol->getRoutesNumber();
	vector<int> tour = this->initSol->getTourStructure();
#ifdef DEBUG_Insert
	cout << "initialObjVal = " << initialObjVal << endl;
#endif
	double newObjVal;

	Node *insertNodePrev = NULL;
	Node *insertNodeNext = NULL;

	for(Node *insertNode = this->initSol->getSequence()->getHead(); insertNode != NULL; insertNode = insertNode->getNext())
	{
#ifdef DEBUG_Insert
		cout << "--------------------insertNode " << " " << " = " << insertNode->getClient()->getId() << " -----------------------" << endl;
#endif
		insertNodePrev = insertNode->getPrevious();
		insertNodeNext = insertNode->getNext();
#ifdef DEBUG_Insert
		if(insertNodePrev != NULL)
			cout << "insertNodePrev = " << insertNodePrev->getClient()->getId() << " ;" ;
		if(insertNodeNext != NULL)
			cout << " insertNodeNext = " << insertNodeNext->getClient()->getId() << endl;
#endif
		bool addAfter = false;

		for(Node *moveNode = this->initSol->getSequence()->getHead(); moveNode != NULL; moveNode = moveNode->getNext())
		{
#ifdef DEBUG_Insert
			cout << "-------------------------- moveNode " << " " << " = " << moveNode->getClient()->getId() << " ------------------- " << endl;
#endif
			// if insertNode == moveNode do nothing and addAfter = true
			if(insertNode == moveNode)
			{
				addAfter = true;
			}
			else
			{
				// remove insertNode from the list
				this->initSol->getSequence()->removeNode(insertNode);
#ifdef DEBUG_Insert
				cout << " show list after removing  " << insertNode->getClient()->getId() << " "  ;  this->initSol->getSequence()->show() ;
#endif
				// insert insertNode after moveNode if addAfter = true
				if(addAfter == true)
				{
					if(moveNode->getNext() == NULL)
					{
						//push back insertNode
						this->initSol->getSequence()->addTail(insertNode);
					}
					else
					{
						//insert insertNode after moveNode
						moveNode->insertAfter(insertNode);
					}
#ifdef DEBUG_Insert
					cout << " show list after inserting " << insertNode->getClient()->getId() << " after " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;
#endif
				}
				else // insert insertNode before moveNode if addAfter = false
				{
					if(moveNode->getPrevious() == NULL)
					{
						//push front insertNode
						this->initSol->getSequence()->addHead(insertNode);
					}
					else
					{
						//insert insertNode before moveNode
						moveNode->insertBefore(insertNode);
					}
#ifdef DEBUG_Insert
					cout << " show list after inserting " << insertNode->getClient()->getId() << " before " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;
#endif
				}

			}

			// decode the list
			if(this->initSol->Decodage(false))
			{
#ifdef DEBUG_Insert
				this->initSol->CheckSolution(false);
#endif
				newObjVal = this->initSol->getObjVal();
			}
#ifdef DEBUG_Insert
			cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();
#endif

			// if the new solution is better than the initial one then keep the new solution
			if(newObjVal < initialObjVal - 0.0001)
			{
#ifdef DEBUG_Insert
				this->initSol->getSequence()->show();
#endif
				return true;
			}
			else // otherwise restore the list
			{
				// reset initialObjVal, tour and number of routes
				this->initSol->setObjVal(initialObjVal);
				this->initSol->setRouteNumber(numberRoute);
				this->initSol->setTourStructure(tour);

				// reset the initial sequence

				// remove insertNode in its current place
				this->initSol->getSequence()->removeNode(insertNode);

#ifdef DEBUG_Insert
				cout << " show list after removing " << insertNode->getClient()->getId() << " in its current place " ;  this->initSol->getSequence()->show() ;
#endif

				// insert insertNode in its initial place
				if(insertNodePrev == NULL)
				{
					// push front insertNode
					this->initSol->getSequence()->addHead(insertNode);
				}
				else if( insertNodeNext == NULL)
				{
					// push back insertNode
					this->initSol->getSequence()->addTail(insertNode);
				}
				else
				{
					// insert insertNode between insertNodePrev and insertNodeNext
					insertNode->insertBetween(insertNodePrev, insertNodeNext);
				}
#ifdef DEBUG_Insert
				cout << " show list after inserting " << insertNode->getClient()->getId() << " in its initial place " ;  this->initSol->getSequence()->show() ;
#endif
			}
		}
#ifdef DEBUG_Insert
		cout << "-------------------------------------------------------------------------------------------------" << endl;
#endif
	}

	return false;
}

bool LocalSearch::ArcInsert()
{
#ifdef DEBUG_ArcInsert
	cout << endl;
	cout << "local search ArcInsert initial sequence = " ; this->initSol->getSequence()->show() ;
#endif
	double initialObjVal = this->initSol->getObjVal();
	int numberRoute = this->initSol->getRoutesNumber();
	vector<int> tour = this->initSol->getTourStructure();
#ifdef DEBUG_ArcInsert
	cout << "initialObjVal = " << initialObjVal << endl;
#endif
	double newObjVal;

	Node *insertNodeFirstPrev = NULL;
	Node *insertNodeLastNext = NULL;

	Node *insertNodeFirst = NULL;
	Node *insertNodeLast = NULL;

	for(Node *insertNode = this->initSol->getSequence()->getHead(); insertNode != NULL; insertNode = insertNode->getNext())
	{
		insertNodeFirst = insertNode;
		insertNodeLast = insertNodeFirst->getNext();

		if(insertNodeLast != NULL)
		{
#ifdef DEBUG_ArcInsert
			cout << " -------------------- insert arc = " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << " -----------------------" << endl;
#endif
			insertNodeFirstPrev = insertNode->getPrevious();
			insertNodeLastNext = insertNodeLast->getNext();
#ifdef DEBUG_ArcInsert
			if(insertNodeFirstPrev != NULL)
				cout << "insertNodeFirstPrev = " << insertNodeFirstPrev->getClient()->getId() << " ;" ;
			if(insertNodeLastNext != NULL)
				cout << " insertNodeLastNext = " << insertNodeLastNext->getClient()->getId() << endl;
			cout << endl;
#endif
			bool addAfter = false;

			for(Node *moveNode = this->initSol->getSequence()->getHead(); moveNode != NULL; moveNode = moveNode->getNext())
			{
#ifdef DEBUG_ArcInsert
				cout << "-------------------------- moveNode " << " " << " = " << moveNode->getClient()->getId() << " ------------------- " << endl;
#endif
				// if insertNodeFirst == moveNode or insertNodeLast == moveNode do nothing and addAfter = true
				if(insertNodeFirst == moveNode || insertNodeLast == moveNode)
				{
					addAfter = true;
				}
				else
				{
					//remove the two arcs from the list
					this->initSol->getSequence()->removeNode(insertNodeFirst);
					this->initSol->getSequence()->removeNode(insertNodeLast);
#ifdef DEBUG_ArcInsert
					cout << " show list after removing  " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() ;  this->initSol->getSequence()->show() ;
#endif
					// insert insertNodeFirst and insertNodeLast after moveNode if addAfter = true
					if(addAfter == true)
					{
						if(moveNode->getNext() == NULL)
						{
							// add back insertNodeFirst
							this->initSol->getSequence()->addTail(insertNodeFirst);

							// add back insertNodeLast
							this->initSol->getSequence()->addTail(insertNodeLast);
						}
						else
						{
							// insert insertNodeFirst after moveNode
							moveNode->insertAfter(insertNodeFirst);

							// insert insertNodeLast after insertNodeFirst
							insertNodeFirst->insertAfter(insertNodeLast);
						}
#ifdef DEBUG_ArcInsert
						cout << " show list after inserting " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << " after " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;
#endif
					}
					else // insert insertNodeFirst and insertNodeLast before moveNode if addAfter = false
					{
						if(moveNode->getPrevious() == NULL)
						{
							// add front insertNodeLast
							this->initSol->getSequence()->addHead(insertNodeLast);

							// add front insertNodeFirst
							this->initSol->getSequence()->addHead(insertNodeFirst);
						}
						else
						{
							//insert insertNodeLast before moveNode
							moveNode->insertBefore(insertNodeLast);

							//insert insertNodeFirst before insertNodeLast
							insertNodeLast->insertBefore(insertNodeFirst);
						}
#ifdef DEBUG_ArcInsert
						cout << " show list after inserting " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId()  << " before " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show();
#endif
					}

				}

				// decode the list
				if(this->initSol->Decodage(false))
				{
#ifdef DEBUG_ArcInsert
					this->initSol->CheckSolution(false);
#endif
					newObjVal = this->initSol->getObjVal();
				}
#ifdef DEBUG_ArcInsert
				cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();
#endif
				// if the new solution is better than the initial one then keep the new solution
				if(newObjVal < initialObjVal - 0.0001)
				{
#ifdef DEBUG_ArcInsert
					this->initSol->getSequence()->show();
#endif
					return true;
				}
				else // otherwise restore the list
				{
					// reset initialObjVal, tour and number of routes
					this->initSol->setObjVal(initialObjVal);
					this->initSol->setRouteNumber(numberRoute);
					this->initSol->setTourStructure(tour);

					// reset the initial sequence
					// remove insertNodeFirst and insertNodeLast in their current place from the list
					this->initSol->getSequence()->removeNode(insertNodeFirst);
					this->initSol->getSequence()->removeNode(insertNodeLast);
#ifdef DEBUG_ArcInsert
					cout << " show list after removing " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << " in their current place " ;  this->initSol->getSequence()->show() ;
#endif
					// insert insertNode in its initial place
					if(insertNodeFirstPrev == NULL)
					{
						// add insertNodeLast to the front
						this->initSol->getSequence()->addHead(insertNodeLast);

						// add insertNodeFirst to the front
						this->initSol->getSequence()->addHead(insertNodeFirst);
					}
					else if( insertNodeLastNext == NULL)
					{
						// push back insertNodeFirst
						this->initSol->getSequence()->addTail(insertNodeFirst);

						// push back insertNodeLast
						this->initSol->getSequence()->addTail(insertNodeLast);
					}
					else if(insertNodeFirstPrev != NULL && insertNodeLastNext != NULL)
					{
						// insert insertNodeFirst after insertNodeFirstPrev and before insertNodeLastNext
						insertNodeFirst->insertBetween(insertNodeFirstPrev,insertNodeLastNext);

						//insert insertNodeLast after insertNodeFirst and before insertNodeLastNext
						insertNodeLast->insertBetween(insertNodeFirst,insertNodeLastNext);
					}
#ifdef DEBUG_ArcInsert
					cout << " show list after inserting " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << " in their initial place " ; this->initSol->getSequence()->show();
#endif
				}
			}
		}
#ifdef DEBUG_ArcInsert
		cout << "-------------------------------------------------------------------------------------------------" << endl;
#endif
	}

	return false;
}


bool LocalSearch::Swap()
{
#ifdef DEBUG_Swap
	cout << endl;
	cout << "local search Swap initial sequence = " ; this->initSol->getSequence()->show() ;
#endif
	double initialObjVal = this->initSol->getObjVal();
	int numberRoute = this->initSol->getRoutesNumber();
	vector<int> tour = this->initSol->getTourStructure();
#ifdef DEBUG_Swap
	cout << "initialObjVal = " << initialObjVal << endl;
#endif
	double newObjVal;

	Node *swapNodeFirstPrev = NULL;
	Node *swapNodeFirstNext = NULL;

	for(Node *swapNodeFirst = this->initSol->getSequence()->getHead(); swapNodeFirst != NULL; swapNodeFirst = swapNodeFirst->getNext())
	{
#ifdef DEBUG_Swap
		cout << "--------------------swapNodeFirst " << " " << " = " << swapNodeFirst->getClient()->getId() << " -----------------------" << endl;
#endif
		swapNodeFirstPrev = swapNodeFirst->getPrevious();
		swapNodeFirstNext = swapNodeFirst->getNext();
#ifdef DEBUG_Swap
		if(swapNodeFirstPrev != NULL)
			cout << "swapNodeFirstPrev = " << swapNodeFirstPrev->getClient()->getId() << " ;" ;
		if(swapNodeFirstNext != NULL)
			cout << " swapNodeFirstNext = " << swapNodeFirstNext->getClient()->getId() << endl;
		cout << endl;
#endif
		Node *swapNodeSecondPrev = NULL;
		Node *swapNodeSecondNext = NULL;

		for(Node *swapNodeSecond = swapNodeFirst->getNext(); swapNodeSecond != NULL; swapNodeSecond = swapNodeSecond->getNext())
		{
#ifdef DEBUG_Swap
			cout << "-------------------------- swapNodeSecond " << " " << " = " << swapNodeSecond->getClient()->getId() << " ------------------- " << endl;
#endif
			swapNodeSecondPrev = swapNodeSecond->getPrevious();
			swapNodeSecondNext = swapNodeSecond->getNext();
#ifdef DEBUG_Swap
			if(swapNodeSecondPrev != NULL)
				cout << "swapNodeSecondPrev = " << swapNodeSecondPrev->getClient()->getId() << " ;" ;
			if(swapNodeSecondNext != NULL)
				cout << " swapNodeSecondNext = " << swapNodeSecondNext->getClient()->getId() << endl;
			cout << endl;
#endif
			//remove swapNodeFirst
			this->initSol->getSequence()->removeNode(swapNodeFirst);
#ifdef DEBUG_Swap
			cout << " show list after removing  " << swapNodeFirst->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
			Node *sNodeSecondPrev = swapNodeSecond->getPrevious();
			Node *sNodeSecondNext = swapNodeSecond->getNext();
#ifdef DEBUG_Swap
				if(sNodeSecondPrev != NULL)
					cout << "sNodeSecondPrev = " << sNodeSecondPrev->getClient()->getId() << " ;" ;
				if(sNodeSecondNext != NULL)
					cout << " sNodeSecondNext = " << sNodeSecondNext->getClient()->getId() << endl;
				cout << endl;
#endif
				// remove swapNodeSecond
				this->initSol->getSequence()->removeNode(swapNodeSecond);
#ifdef DEBUG_Swap
				cout << " show list after removing  " << swapNodeSecond->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
			if(swapNodeFirstPrev == NULL)
			{// swap the two nodes : swapNodeFirst and swapNodeSecond

				// put swapNodeFirst in the place of swapNodeSecond then put swapNodeSecond in the place of swapNodeFirst
				if(sNodeSecondPrev == NULL)
				{
					//push front swapNodeFirst then swapNodeSecond
					this->initSol->getSequence()->addHead(swapNodeFirst);
				}
				else if(sNodeSecondNext == NULL)
				{
					// push back swapNodeFirst then push front swapNodeSecond
					this->initSol->getSequence()->addTail(swapNodeFirst);
				}
				else
				{
					// insert swapNodeFirst after sNodeSecondPrev and before sNodeSecondNext
					// then push front swapNodeSecond
					swapNodeFirst->insertBetween(sNodeSecondPrev,sNodeSecondNext);
				}
				this->initSol->getSequence()->addHead(swapNodeSecond);
#ifdef DEBUG_Swap
				cout << " show list after swapping  " << swapNodeFirst->getClient()->getId() << " and " << swapNodeSecond->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
			}
			else
			{ // swap the two nodes : swapNodeFirst and swapNodeSecond

				// put swapNodeFirst in the place of swapNodeSecond then put swapNodeSecond in the place of swapNodeFirst
				if(sNodeSecondNext == NULL)
				{
					// push back swapNodeFirst
					this->initSol->getSequence()->addTail(swapNodeFirst);
					if(swapNodeFirstNext == swapNodeSecond)
					{
						//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeFirst
						swapNodeSecond->insertBetween(swapNodeFirstPrev,swapNodeFirst);
					}
					else
					{
						//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeFirstNext
						swapNodeSecond->insertBetween(swapNodeFirstPrev,swapNodeFirstNext);
					}

				}
				else
				{
					//insert swapNodeFirst after  sNodeSecondPrev and before sNodeSecondNext
					swapNodeFirst->insertBetween(sNodeSecondPrev,sNodeSecondNext);
					if(swapNodeFirstNext == swapNodeSecond)
					{
						//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeFirst
						swapNodeSecond->insertBetween(swapNodeFirstPrev,swapNodeFirst);
					}
					else
					{
						//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeFirstNext
						swapNodeSecond->insertBetween(swapNodeFirstPrev,swapNodeFirstNext);
					}

				}
#ifdef DEBUG_Swap
				cout << " show list after swapping  " << swapNodeFirst->getClient()->getId() << " and " << swapNodeSecond->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
			}

			// decode the list
			if(this->initSol->Decodage(false))
			{
#ifdef DEBUG_Swap
				this->initSol->CheckSolution(false);
#endif
				newObjVal = this->initSol->getObjVal();
			}
#ifdef DEBUG_Swap
			cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();
#endif
			// if the new solution is better than the initial one then keep the new solution
			if(newObjVal < initialObjVal - 0.0001)
			{
#ifdef DEBUG_Swap
				this->initSol->getSequence()->show();
#endif
				return true;
			}
			else // otherwise restore the list
			{
				// reset initialObjVal, tour and number of routes
				this->initSol->setObjVal(initialObjVal);
				this->initSol->setRouteNumber(numberRoute);
				this->initSol->setTourStructure(tour);

				// reset the initial sequence

				// remove swapNodeFirst in its current place
				this->initSol->getSequence()->removeNode(swapNodeFirst);

#ifdef DEBUG_Swap
				cout << " show list after removing " << swapNodeFirst->getClient()->getId() <<  " in its current place " ;  this->initSol->getSequence()->show() ;
#endif
				// insert swapNodeFirst in its initial place
				if(swapNodeFirstPrev == NULL)
				{
					//push front swapNodeFirst
					this->initSol->getSequence()->addHead(swapNodeFirst);
				}
				else if(swapNodeFirstNext == NULL)
				{
					// push back swapNodeFirst
					this->initSol->getSequence()->addTail(swapNodeFirst);
				}
				else
				{
					// insert swapNodeFirst after swapNodeFirstPrev
					swapNodeFirstPrev->insertAfter(swapNodeFirst);
				}
#ifdef DEBUG_Swap
				cout << " show list after inserting " << swapNodeFirst->getClient()->getId() << " in its initial place " ;  this->initSol->getSequence()->show() ;
#endif
				// remove swapNodeSecond in its current place
				this->initSol->getSequence()->removeNode(swapNodeSecond);
#ifdef DEBUG_Swap
				cout << " show list after removing " <<  swapNodeSecond->getClient()->getId() << " in its current place " ;  this->initSol->getSequence()->show() ;
#endif
				// insert swapNodeSecond in its initial place
				if(swapNodeSecondNext == NULL)
				{
					// push back swapNodeSecond
					this->initSol->getSequence()->addTail(swapNodeSecond);
				}
				else
				{
					// insert swapNodeSecond after swapNodeSecondPrev and before swapNodeSecondNext
					swapNodeSecond->insertBetween(swapNodeSecondPrev,swapNodeSecondNext);
				}
#ifdef DEBUG_Swap
				cout << " show list after inserting " <<  swapNodeSecond->getClient()->getId() << " in its initial place " ;  this->initSol->getSequence()->show() ;
#endif
			}

		}
#ifdef DEBUG_Swap
		cout << "-------------------------------------------------------------------------------------------------" << endl;
#endif
	}

	return false;
}

bool LocalSearch::SwapArcs()
{
#ifdef DEBUG_SwapArcs
	cout << endl;
	cout << "local search SwapArc initial sequence = " ; this->initSol->getSequence()->show() ;
#endif
	double initialObjVal = this->initSol->getObjVal();
	int numberRoute = this->initSol->getRoutesNumber();
	vector<int> tour = this->initSol->getTourStructure();
#ifdef DEBUG_SwapArcs
	cout << "initialObjVal = " << initialObjVal << endl;
#endif
	double newObjVal;

	Node *arcFirstNode = NULL; // the first node of the arc
	Node *arcLastNode = NULL; // the last node of the arc

	Node *arcFirstNodePrev = NULL;
	Node *arcLastNodeNext = NULL;

	for(Node *arcNode = this->initSol->getSequence()->getHead(); arcNode != NULL; arcNode = arcNode->getNext())
	{

#ifdef DEBUG_SwapArcs
		cout << endl;
#endif
		arcFirstNode = arcNode;
		arcLastNode = arcFirstNode->getNext();
		if(arcLastNode != NULL)
		{
#ifdef DEBUG_SwapArcs
			cout << "-------------------------------------------------- "<< "swapping arc = " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() << " -------------------------------------------------- "<< endl;
#endif
			arcFirstNodePrev = arcFirstNode->getPrevious();
			arcLastNodeNext = arcLastNode->getNext();
#ifdef DEBUG_SwapArcs
			if(arcFirstNodePrev != NULL)
				cout << "arcFirstNodePrev = " << arcFirstNodePrev->getClient()->getId() << " ;" ;
			if(arcLastNodeNext != NULL)
				cout << "arcLastNodeNext = " << arcLastNodeNext->getClient()->getId() << endl;
			cout << endl;
#endif
			Node *swapNodePrev = NULL;
			Node *swapNodeNext = NULL;

			for(Node *swapNode = this->initSol->getSequence()->getHead(); swapNode != NULL; swapNode = swapNode->getNext())
			{

				if(swapNode != arcFirstNode && swapNode != arcLastNode)
				{
#ifdef DEBUG_SwapArcs
					cout << "-------------------------- swapNode " << " " << " = " << swapNode->getClient()->getId() << " ------------------- " << endl;
#endif
					swapNodePrev = swapNode->getPrevious();
					swapNodeNext = swapNode->getNext();
#ifdef DEBUG_SwapArcs
					if(swapNodePrev != NULL)
						cout << "swapNodePrev = " << swapNodePrev->getClient()->getId() << " ;" ;
					if(swapNodeNext != NULL)
						cout << "swapNodeNext = " << swapNodeNext->getClient()->getId() << endl;
					cout << endl;
#endif
					//remove swapNodeFirst and arcLastNode
					this->initSol->getSequence()->removeNode(arcFirstNode);
					this->initSol->getSequence()->removeNode(arcLastNode);
#ifdef DEBUG_SwapArcs
					cout << " show list after removing  " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
					Node *sNodePrev = swapNode->getPrevious();
					Node *sNodeNext = swapNode->getNext();

#ifdef DEBUG_SwapArcs
					if(sNodePrev != NULL)
						cout << "sNodePrev = " << sNodePrev->getClient()->getId() << " ;" ;
					if(sNodeNext != NULL)
						cout << " SNodeNext = " << sNodeNext->getClient()->getId() << endl;
					cout << endl;
#endif
					//remove swapNode
					this->initSol->getSequence()->removeNode(swapNode);
#ifdef DEBUG_SwapArcs
					cout << " show list after removing  " << swapNode->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
					// swap the arc and the swapNode
					if(arcFirstNodePrev == NULL)
					{
						if(sNodePrev == NULL)
						{
							//push front arcLastNode then arcFirstNode then swapNode
							this->initSol->getSequence()->addHead(arcLastNode);
							this->initSol->getSequence()->addHead(arcFirstNode);
						}
						else if(sNodeNext == NULL)
						{
							// push back arcFirstNode then arcLastNode then push front swapNode
							this->initSol->getSequence()->addTail(arcFirstNode);
							this->initSol->getSequence()->addTail(arcLastNode);
						}
						else
						{
							// insert arcFirstNode after sNodePrev and before sNodeNext
							arcFirstNode->insertBetween(sNodePrev,sNodeNext);
							// insert arcLastNode after arcFirstNode and before sNodeNext
							arcLastNode->insertBetween(arcFirstNode,sNodeNext);
						}
						// then push front swapNode
						this->initSol->getSequence()->addHead(swapNode);
#ifdef DEBUG_SwapArcs
						cout << " show list after swapping  " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() << " and " << swapNode->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
					}
					else
					{
						if(sNodePrev == NULL)
						{
							// push front arcLastNode then arcFirstNode
							this->initSol->getSequence()->addHead(arcLastNode);
							this->initSol->getSequence()->addHead(arcFirstNode);
							if(arcFirstNodePrev == swapNode)
							{
								// insert swapNode after arcLastNode and before arcLastNodeNext
								swapNode->insertBetween(arcLastNode,arcLastNodeNext);
							}
							else if(arcLastNodeNext == NULL)
							{
								// push back swapNode
								this->initSol->getSequence()->addTail(swapNode);
							}
							else
							{
								//insert swapNode after  arcFirstNodePrev and before arcLastNodeNext
								swapNode->insertBetween(arcFirstNodePrev,arcLastNodeNext);
							}
						}
						else if(sNodeNext == NULL)
						{
							// push back arcFirstNode then arcLastNode
							this->initSol->getSequence()->addTail(arcFirstNode);
							this->initSol->getSequence()->addTail(arcLastNode);
							if(arcLastNodeNext == swapNode)
							{
								//insert swapNode after  arcFirstNodePrev and before arcFirstNode
								swapNode->insertBetween(arcFirstNodePrev,arcFirstNode);
							}
							else if(arcFirstNodePrev == swapNode)
							{
								//push back swapNode
								this->initSol->getSequence()->addTail(swapNode);
							}
							else
							{
								//insert swapNode after  arcFirstNodePrev and before arcLastNodeNext
								swapNode->insertBetween(arcFirstNodePrev,arcLastNodeNext);
							}
						}
						else
						{
							//insert arcFirstNode after  sNodePrev and before sNodeNext
							arcFirstNode->insertBetween(sNodePrev,sNodeNext);
							//insert arcLastNode after arcFirstNode and before sNodeNext
							arcLastNode->insertBetween(arcFirstNode,sNodeNext);
							if(arcFirstNodePrev == swapNode)
							{
								//insert swapNode after arcLastNode and before arcLastNodeNext
								swapNode->insertBetween(arcLastNode,arcLastNodeNext);

							}
							else if(arcLastNodeNext == swapNode)
							{
								//insert swapNode after  arcFirstNodePrev and before arcFirstNode
								swapNode->insertBetween(arcFirstNodePrev,arcFirstNode);
							}
							else if(arcLastNodeNext == NULL)
							{
								// push back swapNode
								this->initSol->getSequence()->addTail(swapNode);

							}
							else
							{
								//insert swapNode after  arcFirstNodePrev and before arcLastNodeNext
								swapNode->insertBetween(arcFirstNodePrev,arcLastNodeNext);
							}
						}
#ifdef DEBUG_SwapArcs
						cout << " show list after swapping  " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() << " and " << swapNode->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
					}

					// decode the list

					if(this->initSol->Decodage(false))
					{
#ifdef DEBUG_SwapArcs
						this->initSol->CheckSolution(false);
#endif
						newObjVal = this->initSol->getObjVal();
					}
#ifdef DEBUG_SwapArcs
					cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();
#endif
					// if the new solution is better than the initial one then keep the new solution
					if(newObjVal < initialObjVal - 0.0001)
					{
#ifdef DEBUG_SwapArcs
						this->initSol->getSequence()->show();
#endif
						return true;
					}
					else // otherwise restore the list
					{
						// reset initialObjVal, tour and number of routes
						this->initSol->setObjVal(initialObjVal);
						this->initSol->setRouteNumber(numberRoute);
						this->initSol->setTourStructure(tour);

						// reset the initial sequence

						// remove arcFirstNode and arcLastNode in their current place
						this->initSol->getSequence()->removeNode(arcFirstNode);
						this->initSol->getSequence()->removeNode(arcLastNode);

#ifdef DEBUG_SwapArcs
						cout << " show list after removing " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() <<  " in their current place " ;  this->initSol->getSequence()->show() ;
#endif
						// insert arcFirstNode and arcLastNode in their initial place
						if(arcFirstNodePrev == NULL)
						{
							//push front arcLastNode then arcFirstNode
							this->initSol->getSequence()->addHead(arcLastNode);
							this->initSol->getSequence()->addHead(arcFirstNode);
						}
						else if(arcLastNodeNext == NULL)
						{
							// push back arcFirstNode then arcLastNode
							this->initSol->getSequence()->addTail(arcFirstNode);
							this->initSol->getSequence()->addTail(arcLastNode);
						}
						else
						{
							// insert arcFirstNode after arcFirstNodePrev
							arcFirstNodePrev->insertAfter(arcFirstNode);
							// insert arcLastNode after arcFirstNode
							arcFirstNode->insertAfter(arcLastNode);
						}

#ifdef DEBUG_SwapArcs
						cout << " show list after inserting " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() <<  " in their initial place " ;  this->initSol->getSequence()->show() ;
#endif
						// remove swapNode in its current place
						this->initSol->getSequence()->removeNode(swapNode);
#ifdef DEBUG_SwapArcs
						cout << " show list after removing " << swapNode->getClient()->getId() <<   " in its current place " ;  this->initSol->getSequence()->show() ;
#endif
						// insert swapNode in its initial place // to verify again
						if(swapNodePrev == NULL)
						{
							// push front swapNode
							this->initSol->getSequence()->addHead(swapNode);
						}
						else if(swapNodeNext == NULL)
						{
							// push back swapNode
							this->initSol->getSequence()->addTail(swapNode);
						}
						else
						{
							// insert swapNode after swapNodePrev and before swapNodeNext
							swapNode->insertBetween(swapNodePrev,swapNodeNext);
						}
#ifdef DEBUG_SwapArcs
						cout << " show list after inserting " <<  swapNode->getClient()->getId() << " in its initial place " ;  this->initSol->getSequence()->show() ;
						cout << endl;
#endif
					}
				}
			}
		}

#ifdef DEBUG_SwapArcs
		cout << "-------------------------------------------------------------------------------------------------" << endl;
#endif
	}

	return false;
}

bool LocalSearch::SwapTwoArcs()
{
#ifdef DEBUG_SwapTwoArcs
	cout << endl;
	cout << "local search SwapTwoArcs initial sequence = " ; this->initSol->getSequence()->show() ;
#endif
	double initialObjVal = this->initSol->getObjVal();
	int numberRoute = this->initSol->getRoutesNumber();
	vector<int> tour = this->initSol->getTourStructure();
#ifdef DEBUG_SwapTwoArcs
	cout << "initialObjVal = " << initialObjVal << endl;
#endif
	double newObjVal;

	Node *startArcFirst = NULL;
	Node *endArcFirst = NULL;

	Node *startArcFirstPrev = NULL;
	Node *endArcFirstNext = NULL;

	for(Node *arcFirst = this->initSol->getSequence()->getHead(); arcFirst != NULL; arcFirst = arcFirst->getNext())
	{

#ifdef DEBUG_SwapTwoArcs
		cout << endl;
#endif
		startArcFirst = arcFirst;
		endArcFirst = startArcFirst->getNext();
		if(endArcFirst != NULL)
		{
#ifdef DEBUG_SwapTwoArcs
			cout << "-------------------------------------------------- "<< "first swapping arc = " << startArcFirst->getClient()->getId() << "--" << endArcFirst->getClient()->getId() << "-------------------------------------------------- " << endl;
#endif
			startArcFirstPrev = startArcFirst->getPrevious();
			endArcFirstNext = endArcFirst->getNext();
#ifdef DEBUG_SwapTwoArcs
			if(startArcFirstPrev != NULL)
				cout << "startArcFirstPrev = " << startArcFirstPrev->getClient()->getId() << " ;" ;
			if(endArcFirstNext != NULL)
				cout << " arcFirstNext = " << endArcFirstNext->getClient()->getId() << endl;
			cout << endl;
#endif
			Node *startArcSecond = NULL;
			Node *endArcSecond = NULL;

			Node *startArcSecondPrev = NULL;
			Node *endArcSecondNext = NULL;

			for(Node *arcSecond = endArcFirst->getNext(); arcSecond != NULL; arcSecond = arcSecond->getNext())
			{

				startArcSecond = arcSecond;
				endArcSecond = startArcSecond->getNext();
				if(endArcSecond != NULL)
				{
#ifdef DEBUG_SwapTwoArcs
					cout << "-------------------------" << "second swapping arc = " << startArcSecond->getClient()->getId() << "--" << endArcSecond->getClient()->getId() << "-------------------------" << endl;
#endif
					startArcSecondPrev = startArcSecond->getPrevious();
					endArcSecondNext = endArcSecond->getNext();
#ifdef DEBUG_SwapTwoArcs
					if(startArcSecondPrev != NULL)
						cout << "startArcSecondPrev = " << startArcSecondPrev->getClient()->getId() << " ;" ;
					if(endArcSecondNext != NULL)
						cout << " endArcSecondNext = " << endArcSecondNext->getClient()->getId() << endl;
					cout << endl;
#endif
					//remove startArcFirst and endArcFirst
					this->initSol->getSequence()->removeNode(startArcFirst);
					this->initSol->getSequence()->removeNode(endArcFirst);
#ifdef DEBUG_SwapTwoArcs
					cout << " show list after removing  " << startArcFirst->getClient()->getId() << "--" << endArcFirst->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
					Node *sArcSecondPrev = NULL;
					Node *eArcSecondNext = NULL;

					sArcSecondPrev = startArcSecond->getPrevious();
					eArcSecondNext = endArcSecond->getNext();
#ifdef DEBUG_SwapTwoArcs
					if(sArcSecondPrev != NULL)
						cout << "sArcSecondPrev = " << sArcSecondPrev->getClient()->getId() << " ;" ;
					if(eArcSecondNext != NULL)
						cout << " eArcSecondNext = " << eArcSecondNext->getClient()->getId() << endl;
					cout << endl;
#endif

					//remove startArcSecond and endArcSecond
					this->initSol->getSequence()->removeNode(startArcSecond);
					this->initSol->getSequence()->removeNode(endArcSecond);
#ifdef DEBUG_SwapTwoArcs
					cout << " show list after removing  " << startArcSecond->getClient()->getId() << "--" << endArcSecond->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
					if(startArcFirstPrev == NULL)
					{ // swap the two arcs : first and second one

						if(sArcSecondPrev == NULL)
						{
							//push front endArcFirst then startArcFirst
							this->initSol->getSequence()->addHead(endArcFirst);
							this->initSol->getSequence()->addHead(startArcFirst);
						}
						else if(eArcSecondNext == NULL)
						{
							// push back startArcFirst then endArcFirst
							this->initSol->getSequence()->addTail(startArcFirst);
							this->initSol->getSequence()->addTail(endArcFirst);
						}
						else
						{
							// insert startArcFirst after sArcSecondPrev and before eArcSecondNext
							startArcFirst->insertBetween(sArcSecondPrev,eArcSecondNext);
							// insert endArcFirst after startArcFirst and before eArcSecondNext
							endArcFirst->insertBetween(startArcFirst,eArcSecondNext);
						}

						// push front endArcSecond then startArcSecond
						this->initSol->getSequence()->addHead(endArcSecond); // move it here
						this->initSol->getSequence()->addHead(startArcSecond); // move it here
#ifdef DEBUG_SwapTwoArcs
						cout << " show list after swapping  " << startArcFirst->getClient()->getId() << "--" << endArcFirst->getClient()->getId() << " and " << startArcSecond->getClient()->getId() << "--" << endArcSecond->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
					}
					else
					{ // swap the two arcs : first and second one

						if(eArcSecondNext == NULL)
						{
							// push back startArcFirst then endArcFirst
							this->initSol->getSequence()->addTail(startArcFirst);
							this->initSol->getSequence()->addTail(endArcFirst);
							if(endArcFirstNext == startArcSecond)
							{
								//insert startArcSecond after  startArcFirstPrev and before startArcFirst
								startArcSecond->insertBetween(startArcFirstPrev,startArcFirst);
								// insert endArcSecond after startArcSecond and before startArcFirst
								endArcSecond->insertBetween(startArcSecond,startArcFirst);
							}
							else
							{
								//insert startArcSecond after  startArcFirstPrev and before endArcFirstNext
								startArcSecond->insertBetween(startArcFirstPrev,endArcFirstNext);
								// insert endArcSecond after startArcSecond and before endArcFirstNext
								endArcSecond->insertBetween(startArcSecond,endArcFirstNext);
							}
						}
						else
						{
							//insert startArcFirst after  sArcSecondPrev and before eArcSecondNext
							startArcFirst->insertBetween(sArcSecondPrev,eArcSecondNext);
							//insert endArcFirst after startArcFirst and before eArcSecondNext
							endArcFirst->insertBetween(startArcFirst,eArcSecondNext);

							if(endArcFirstNext == startArcSecond)
							{
								// insert startArcSecond after  startArcFirstPrev and before startArcFirst
								startArcSecond->insertBetween(startArcFirstPrev,startArcFirst);
								// insert endArcSecond after startArcSecond and before startArcFirst
								endArcSecond->insertBetween(startArcSecond,startArcFirst);
							}
							else
							{
								//insert startArcSecond after  startArcFirstPrev and before endArcFirstNext
								startArcSecond->insertBetween(startArcFirstPrev,endArcFirstNext);
								// insert endArcSecond after startArcSecond and before endArcFirstNext
								endArcSecond->insertBetween(startArcSecond,endArcFirstNext);
							}
						}
#ifdef DEBUG_SwapTwoArcs
						cout << " show list after swapping  " << startArcFirst->getClient()->getId() << "--" << endArcFirst->getClient()->getId() << " and " << startArcSecond->getClient()->getId() << "--" << endArcSecond->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
					}

					// decode the list
					if(this->initSol->Decodage(false))
					{
#ifdef DEBUG_SwapTwoArcs
						this->initSol->CheckSolution(false);
#endif
						newObjVal = this->initSol->getObjVal();
					}
#ifdef DEBUG_SwapTwoArcs
					cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();
#endif
					// if the new solution is better than the initial one then keep the new solution
					if(newObjVal < initialObjVal - 0.0001)
					{
#ifdef DEBUG_SwapTwoArcs
						this->initSol->getSequence()->show();
#endif
						return true;
					}
					else // otherwise restore the list
					{
						// reset initialObjVal, tour and number of routes
						this->initSol->setObjVal(initialObjVal);
						this->initSol->setRouteNumber(numberRoute);
						this->initSol->setTourStructure(tour);

						// reset the initial sequence

						// remove startArcFirst and endArcFirst in their current place
						this->initSol->getSequence()->removeNode(startArcFirst);
						this->initSol->getSequence()->removeNode(endArcFirst);

#ifdef DEBUG_SwapTwoArcs
						cout << " show list after removing " << startArcFirst->getClient()->getId() << "--" << endArcFirst->getClient()->getId() <<  " in their current place " ;  this->initSol->getSequence()->show() ;
#endif
						// insert startArcFirst and endArcFirst in their initial place
						if(startArcFirstPrev == NULL)
						{
							//push front endArcFirst then startArcFirst
							this->initSol->getSequence()->addHead(endArcFirst);
							this->initSol->getSequence()->addHead(startArcFirst);
						}
						else
						{
							// insert startArcFirst after startArcFirstPrev
							startArcFirstPrev->insertAfter(startArcFirst);
							// insert endArcFirst after startArcFirst
							startArcFirst->insertAfter(endArcFirst);
						}

#ifdef DEBUG_SwapTwoArcs
						cout << " show list after inserting " << startArcFirst->getClient()->getId() << "--" << endArcFirst->getClient()->getId() <<  " in their initial place " ;  this->initSol->getSequence()->show() ;
#endif
						// remove startArcSecond and endArcSecond in its current place
						this->initSol->getSequence()->removeNode(startArcSecond);
						this->initSol->getSequence()->removeNode(endArcSecond);
#ifdef DEBUG_SwapTwoArcs
						cout << " show list after removing " << startArcSecond->getClient()->getId() << "--" <<  endArcSecond->getClient()->getId() << " in its current place " ;  this->initSol->getSequence()->show() ;
#endif
						// insert startArcSecond and endArcSecond in thier initial place // to verify again
						if(endArcSecondNext == NULL)
						{
							// push back startArcSecond then endArcSecond
							this->initSol->getSequence()->addTail(startArcSecond);
							this->initSol->getSequence()->addTail(endArcSecond);
						}
						else
						{
							// insert startArcSecond after startArcSecondPrev and before endArcSecondNext
							startArcSecond->insertBetween(startArcSecondPrev,endArcSecondNext);
							// insert endArcSecond after startArcSecond and before endArcSecondNext
							endArcSecond->insertBetween(startArcSecond,endArcSecondNext);
						}
#ifdef DEBUG_SwapTwoArcs
						cout << " show list after inserting " <<  startArcSecond->getClient()->getId() << "--" <<  endArcSecond->getClient()->getId() << " in their initial place " ;  this->initSol->getSequence()->show() ;
						cout <<  endl;
#endif
					}
				}
			}
		}

#ifdef DEBUG_SwapTwoArcs
		cout << "-------------------------------------------------------------------------------------------------" << endl;
#endif
	}

	return false;
}

// movements for direct encoding

bool LocalSearch::IntraRouteInsert()
{

#ifdef DEBUG_IntraInsert
	cout << endl;
	cout << "local search IntraRouteInsert initial route = " ; this->initSol->PrintSolution(true) ;
#endif


	// initialize route sub-sequences
	this->initSol->initRouteSetSubSeq();

	for(uint r = 0; r < this->initSol->getRouteForwSeq().size(); r++)
	{
		// record each route cost
		double routeCost;
		if((this->initSol->getRouteForwSeq()[r].size()-2) == 0 || (this->initSol->getRouteForwSeq()[r].size()-2) == 1)
		{
			cout << "empty route or a route containing only one client" << endl;
		}
		else if((this->initSol->getRouteForwSeq()[r].size()-2) == 2)
		{// route contains 2 clients
			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
			if(this->initSol->getRouteBackSeq()[r][0].back()->getDistance() < routeCost - 0.0001)
			{
				// apply the move
				Node *insertNodePrev = NULL;
				Node *insertNodeNext = NULL;
				for(Node *insertNode = this->initSol->getRouteSequence()[r].first->getNext(); insertNode->getClient()->getDemand() != 0; insertNode = insertNode->getNext())
				{
#ifdef DEBUG_IntraInsert
		cout << "--------------------insertNode " << " " << " = " << insertNode->getClient()->getId() << " -----------------------" << endl;
#endif
					insertNodePrev = insertNode->getPrevious();
					insertNodeNext = insertNode->getNext();
#ifdef DEBUG_IntraInsert
		if(insertNodePrev != NULL)
			cout << "insertNodePrev = " << insertNodePrev->getClient()->getId() << " ;" ;
		if(insertNodeNext != NULL)
			cout << " insertNodeNext = " << insertNodeNext->getClient()->getId() << endl;
#endif
					for(Node *moveNode = insertNode->getNext(); moveNode->getClient()->getDemand() != 0; moveNode = moveNode->getNext())
					{
#ifdef DEBUG_IntraInsert
			cout << "-------------------------- moveNode " << " " << " = " << moveNode->getClient()->getId() << " ------------------- " << endl;
#endif					// remove insertNode
						insertNode->getPrevious()->setNext(insertNode->getNext());
						insertNode->getNext()->setPrevious(insertNode->getPrevious());
						insertNode->setPrevious(NULL);
						insertNode->setNext(NULL);

						// insert insertNode after moveNode
						moveNode->insertAfter(insertNode);
					}
				}

				// update the route sequences
				this->initSol->updateRoute(r,make_pair(this->initSol->getRouteSequence()[r].first, this->initSol->getRouteSequence()[r].second));

				// update sub-sequences of the route
				this->initSol->updateOneRouteSetSubSeq(r);

				// update objective function
#ifdef DEBUG_IntraInsert
								cout << " newCostR = " << this->initSol->getRouteBackSeq()[r][0].back()->getDistance() << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraInsert
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + this->initSol->getRouteBackSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraInsert
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);

			}
		}
		else if((this->initSol->getRouteForwSeq()[r].size()-2) == 3)
		{// route contains 3 clients

			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraInsert
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *insertNodePrev = NULL;
			Node *insertNodeNext = NULL;

			uint in = 1;
			for(Node *insertNode = this->initSol->getRouteSequence()[r].first->getNext(); insertNode->getClient()->getDemand() != 0; insertNode = insertNode->getNext())
			{
#ifdef DEBUG_IntraInsert
				cout << "----------- insertNode " << " " << " = " << insertNode->getClient()->getId() << " ----------" <<  " in = " << in << endl;
#endif
				insertNodePrev = insertNode->getPrevious();
				insertNodeNext = insertNode->getNext();
#ifdef DEBUG_IntraInsert
				if(insertNodePrev != NULL)
					cout << "insertNodePrev = " << insertNodePrev->getClient()->getId() << " ;" ;
				if(insertNodeNext != NULL)
					cout << " insertNodeNext = " << insertNodeNext->getClient()->getId() << endl;
#endif
				bool addAfter = false;
				uint mn = 1;
				for(Node *moveNode = this->initSol->getRouteSequence()[r].first->getNext(); moveNode->getClient()->getDemand() != 0; moveNode = moveNode->getNext())
				{
#ifdef DEBUG_IntraInsert
			cout << "---------- moveNode " << " " << " = " << moveNode->getClient()->getId() << " ---------- " << " mn = " << "" << mn << endl;
#endif
					if(in == mn)
					{
						addAfter = true;
					}
					else
					{
						//find sub-sequences used to form the new route

						SeqData *seq1 = NULL;
						SeqData *seq2 = NULL;
						SeqData *seq3 = NULL;

						if(addAfter)
						{
							seq1 = this->initSol->getRouteForwSeq()[r][0][in-1];
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
							if(in+1 ==  mn)
							{
								seq2 = this->initSol->getRouteBackSeq()[r][in][mn-in];
							}
							else
							{
								seq2 = this->initSol->getRouteForwSeq()[r][in+1][mn-(in+1)];
							}

							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;

							if( (in == 1) && (mn == this->initSol->getRouteForwSeq()[r].size()-2))
							{
								seq3 = this->initSol->getRouteBackSeq()[r][in-1][in];
							}
							else
							{
								if(mn == this->initSol->getRouteForwSeq()[r].size()-2)
								{
									seq3 = this->initSol->getRouteForwSeq()[r][mn+1][0];
								}
								else
								{
									seq3 = this->initSol->getRouteForwSeq()[r][mn+1][mn-in];
								}
							}

							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
						}
						else
						{
							if((in == this->initSol->getRouteForwSeq()[r].size()-2) && (mn == 1))
							{
								seq1 = this->initSol->getRouteBackSeq()[r][in][mn];
							}
							else
							{
								seq1 = this->initSol->getRouteForwSeq()[r][0][mn-1];
							}

							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;

							if(in == mn+1)
							{
								seq2 = this->initSol->getRouteBackSeq()[r][mn][in-mn];
							}
							else
							{
								seq2 = this->initSol->getRouteForwSeq()[r][mn][(in-1)-mn]; // to verify
							}

							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;

							if(in == this->initSol->getRouteForwSeq()[r].size()-2)
							{
								seq3 = this->initSol->getRouteForwSeq()[r][in+1][0];
							}
							else
							{
								seq3 = this->initSol->getRouteForwSeq()[r][in+1][in-mn];
							}

							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
						}

						// evaluate the feasibility for the concatenation of the 3 sub-sequences
						double newCostR;
						if(this->initSol->EVALN(&newCostR,3, seq1, seq2, seq3))
						{
							// compare the cost of the new route to the former one
							if(newCostR < routeCost - 0.0001)
							{
								// apply the move
								if(addAfter)
								{
									// remove insertNode
									insertNode->getPrevious()->setNext(insertNode->getNext());
									insertNode->getNext()->setPrevious(insertNode->getPrevious());
									insertNode->setPrevious(NULL);
									insertNode->setNext(NULL);

									// insert insertNode after moveNode
									moveNode->insertAfter(insertNode);
								}
								else
								{
									// remove insertNode
									insertNode->getPrevious()->setNext(insertNode->getNext());
									insertNode->getNext()->setPrevious(insertNode->getPrevious());
									insertNode->setPrevious(NULL);
									insertNode->setNext(NULL);

									// insert insertNode before moveNode
									moveNode->insertBefore(insertNode);
								}

								// update the route sequences
								this->initSol->updateRoute(r,make_pair(this->initSol->getRouteSequence()[r].first, this->initSol->getRouteSequence()[r].second));

								// update sub-sequences of the route
								this->initSol->updateOneRouteSetSubSeq(r);

								// update objective functions
#ifdef DEBUG_IntraInsert
								cout << " newCostR = " << newCostR << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraInsert
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraInsert
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);
								break;

							}
						}
					}

					mn++;
				}

				in++;
			}


		}
		else if((this->initSol->getRouteForwSeq()[r].size()-2) > 3)
		{// route contains more than 3 clients

			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraInsert
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *insertNodePrev = NULL;
			Node *insertNodeNext = NULL;

			uint in = 1;
			for(Node *insertNode = this->initSol->getRouteSequence()[r].first->getNext(); insertNode->getClient()->getDemand() != 0; insertNode = insertNode->getNext())
			{
#ifdef DEBUG_IntraInsert
				cout << "----------- insertNode " << " " << " = " << insertNode->getClient()->getId() << " ----------" <<  " in = " << in << endl;
#endif
				insertNodePrev = insertNode->getPrevious();
				insertNodeNext = insertNode->getNext();
#ifdef DEBUG_IntraInsert
				if(insertNodePrev != NULL)
					cout << "insertNodePrev = " << insertNodePrev->getClient()->getId() << " ;" ;
				if(insertNodeNext != NULL)
					cout << " insertNodeNext = " << insertNodeNext->getClient()->getId() << endl;
#endif
				bool addAfter = false;
				uint mn = 1;
				for(Node *moveNode = this->initSol->getRouteSequence()[r].first->getNext(); moveNode->getClient()->getDemand() != 0; moveNode = moveNode->getNext())
				{
#ifdef DEBUG_IntraInsert
					cout << "---------- moveNode " << " " << " = " << moveNode->getClient()->getId() << " ---------- " << " mn = " << "" << mn << endl;
#endif

					if(in == mn)
					{
						addAfter = true;
					}
					else
					{
						//find sub-sequences used to form the new route

						SeqData *seq1 = NULL;
						SeqData *seq2 = NULL;
						SeqData *seq3 = NULL;
						SeqData *seq4 = NULL;

						if(addAfter)
						{
							seq1 = this->initSol->getRouteForwSeq()[r][0][in-1];

							if(in+1 == mn)
							{
								seq2 = this->initSol->getRouteBackSeq()[r][in][mn-in];
							}
							else
							{
								seq2 = this->initSol->getRouteForwSeq()[r][in+1][mn-(in+1)];
							}

							// we have subSeq seq3 under some conditions
							if(((in+1) != mn) && ( in != 1 || mn != this->initSol->getRouteForwSeq()[r].size()-2))
							{
								seq3 = this->initSol->getRouteForwSeq()[r][in][0];
							}

							if((in == 1) && (mn == this->initSol->getRouteForwSeq()[r].size()-2))
							{
								seq4 = this->initSol->getRouteBackSeq()[r][in-1][in];
							}
							else
							{
								if(mn == this->initSol->getRouteForwSeq()[r].size()-2)
								{
									seq4 = this->initSol->getRouteForwSeq()[r][mn+1][0];
								}
								else
								{
									seq4 = this->initSol->getRouteForwSeq()[r][mn+1][mn-in];
								}
							}
						}
						else
						{
							if((in == this->initSol->getRouteForwSeq()[r].size()-2) && (mn == 1))
							{
								seq1 = this->initSol->getRouteBackSeq()[r][in][mn];
							}
							else
							{
								seq1 = this->initSol->getRouteForwSeq()[r][0][mn-1];
							}

							// we have subSeq seq2 under some conditions
							if(((in+1) != mn) && ( in != 1 || mn != this->initSol->getRouteForwSeq()[r].size()-2))
							{
								seq2 = this->initSol->getRouteForwSeq()[r][in][0];
							}

							if(in == mn+1)
							{
								seq3 = this->initSol->getRouteBackSeq()[r][mn][in-mn];
							}
							else
							{
								seq3 = this->initSol->getRouteForwSeq()[r][mn][(in-1)-mn]; // to verify
							}

							if(in == this->initSol->getRouteForwSeq()[r].size()-2)
							{
								seq4 = this->initSol->getRouteForwSeq()[r][in+1][0];
							}
							else
							{
								seq4 = this->initSol->getRouteForwSeq()[r][in+1][in-mn];
							}
						}

						// evaluate the feasibility for the concatenation of the 3 sub-sequences
						double newCostR;
						if(addAfter)
						{
							if(((in+1) != mn) && ( in != 1 || mn != this->initSol->getRouteForwSeq()[r].size()-2))
							{
								if(this->initSol->EVALN(&newCostR, 4, seq1, seq2, seq3, seq4))
								{
									// compare the cost of the new route to the former one
									if(newCostR < routeCost - 0.0001)
									{// Apply the move

										// remove insertNode
										insertNode->getPrevious()->setNext(insertNode->getNext());
										insertNode->getNext()->setPrevious(insertNode->getPrevious());
										insertNode->setPrevious(NULL);
										insertNode->setNext(NULL);

										// insert insertNode after moveNode
										moveNode->insertAfter(insertNode);

										// update the route sequences
										this->initSol->updateRoute(r,make_pair(this->initSol->getRouteSequence()[r].first, this->initSol->getRouteSequence()[r].second));

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
#ifdef DEBUG_IntraInsert
										cout << " newCostR = " << newCostR << endl;
#endif
										double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraInsert
										cout << "restCout = " << restCout << endl;
#endif
										double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraInsert
										cout << "newObjVal = " << newObjVal << endl;
#endif
										this->initSol->setObjVal(newObjVal);
										break;
									}
								}
							}
							else
							{
								if(this->initSol->EVALN(&newCostR, 3, seq1, seq2, seq4))
								{
									// compare the cost of the new route to the former one
									if(newCostR < routeCost - 0.0001)
									{// Apply the move

										// remove insertNode
										insertNode->getPrevious()->setNext(insertNode->getNext());
										insertNode->getNext()->setPrevious(insertNode->getPrevious());
										insertNode->setPrevious(NULL);
										insertNode->setNext(NULL);

										// insert insertNode after moveNode
										moveNode->insertAfter(insertNode);

										// update the route sequences
										this->initSol->updateRoute(r,make_pair(this->initSol->getRouteSequence()[r].first, this->initSol->getRouteSequence()[r].second));

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
#ifdef DEBUG_IntraInsert
										cout << " newCostR = " << newCostR << endl;
#endif
										double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraInsert
										cout << "restCout = " << restCout << endl;
#endif
										double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraInsert
										cout << "newObjVal = " << newObjVal << endl;
#endif
										this->initSol->setObjVal(newObjVal);
										break;
									}
								}
							}
						}
						else
						{
							if(((in+1) != mn) && ( in != 1 || mn != this->initSol->getRouteForwSeq()[r].size()-2))
							{
								if(this->initSol->EVALN(&newCostR, 4, seq1, seq2, seq3, seq4))
								{
									// compare the cost of the new route to the former one
									if(newCostR < routeCost - 0.0001)
									{// Apply the move

										// remove insertNode
										insertNode->getPrevious()->setNext(insertNode->getNext());
										insertNode->getNext()->setPrevious(insertNode->getPrevious());
										insertNode->setPrevious(NULL);
										insertNode->setNext(NULL);

										// insert insertNode after moveNode
										moveNode->insertAfter(insertNode);

										// update the route sequences
										this->initSol->updateRoute(r,make_pair(this->initSol->getRouteSequence()[r].first, this->initSol->getRouteSequence()[r].second));

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
#ifdef DEBUG_IntraInsert
										cout << " newCostR = " << newCostR << endl;
#endif
										double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraInsert
										cout << "restCout = " << restCout << endl;
#endif
										double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraInsert
										cout << "newObjVal = " << newObjVal << endl;
#endif
										this->initSol->setObjVal(newObjVal);
										break;
									}
								}
							}
							else
							{
								if(this->initSol->EVALN(&newCostR, 3, seq1, seq3, seq4))
								{
									// compare the cost of the new route to the former one
									if(newCostR < routeCost - 0.0001)
									{// Apply the move

										// remove insertNode
										insertNode->getPrevious()->setNext(insertNode->getNext());
										insertNode->getNext()->setPrevious(insertNode->getPrevious());
										insertNode->setPrevious(NULL);
										insertNode->setNext(NULL);

										// insert insertNode after moveNode
										moveNode->insertAfter(insertNode);

										// update the route sequences
										this->initSol->updateRoute(r,make_pair(this->initSol->getRouteSequence()[r].first, this->initSol->getRouteSequence()[r].second));

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
#ifdef DEBUG_IntraInsert
										cout << " newCostR = " << newCostR << endl;
#endif
										double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraInsert
										cout << "restCout = " << restCout << endl;
#endif
										double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraInsert
										cout << "newObjVal = " << newObjVal << endl;
#endif
										this->initSol->setObjVal(newObjVal);
										break;
									}
								}
							}
						}
					}
				}
				mn++;
			} // move
			in++;
		} // insert
	} // > 3


	return true; // to change latter by false
}
