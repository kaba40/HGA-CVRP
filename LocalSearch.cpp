/*
 * LocalSearch.cpp
 *
 *  Created on: 19 déc. 2018
 *      Author: kkeita
 */

#include "LocalSearch.hpp"

LocalSearch::LocalSearch(Solution *initSol, bool direct)
{
	this->initSol = initSol;

	if(direct)
	{
		// initialize route sub-sequences
		this->initSol->initRouteSetSubSeq();
	}
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

	// return value
	bool retVal = false;

	for(int r = 0; r < this->initSol->getRoutesNumber(); r++)
	{

		// record each route cost
		double routeCost;
		bool breakIfImproved = false; // stop insertion move for a route if improved

		// no movement if the route contains only one client
		// intraRouteInsert movement start with more than 2 customers
		if(this->initSol->getNbClientsForRoute(r) == 2)
		{// route contains 2 clients, move is necessary in case of multi-depot or asymetric distance
			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
			if(this->initSol->getRouteBackSeq()[r][0].back()->getDistance() < routeCost - 0.0001)
			{

				// apply the move
				Node *insertNode = this->initSol->getRoutes()[r].first->getNext();
				Node *moveNode = insertNode->getNext();

				// remove insertNode
				insertNode->removeNode();

				// insert insertNode after moveNode
				moveNode->insertAfter(insertNode);

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

				retVal = true;
			}
		}
		else if(this->initSol->getNbClientsForRoute(r) == 3)
		{// route contains 3 clients

			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraInsert
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *insertNodePrev = NULL;
			Node *insertNodeNext = NULL;


			int in = 1; /* 0--a--b--c--0, in = 1 --> in = a   */
			for(Node *insertNode = this->initSol->getRoutes()[r].first->getNext(); !insertNode->isDepot() && breakIfImproved == false; insertNode = insertNode->getNext(), in++)
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
				int mn = 1; /* 0--a--b--c--0, mn = 1 --> mn = a   */
				for(Node *moveNode = this->initSol->getRoutes()[r].first->getNext(); !moveNode->isDepot() && breakIfImproved == false; moveNode = moveNode->getNext(), mn++)
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
#ifdef DEBUG_IntraInsert
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
							if(in+1 ==  mn) /* 0--a--b--c--0 , if in = 1 and mn = 2 */
							{
								seq2 = this->initSol->getRouteBackSeq()[r][in][mn-in];
							}
							else
							{
								seq2 = this->initSol->getRouteForwSeq()[r][in+1][mn-(in+1)];
							}
#ifdef DEBUG_IntraInsert
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
							//this->initSol->getRouteForwSeq()[r].size()-2
							if( (in == 1) && (mn == this->initSol->getNbClientsForRoute(r))) /* 0--a--b--c--0 , if in = 1 and mn = 3 */
							{
								seq3 = this->initSol->getRouteBackSeq()[r][in-1][in];
							}
							else
							{
							    int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
								seq3 = this->initSol->getRouteForwSeq()[r][mn+1][indexLastNodeRoute-(mn+1)];
							}
#ifdef DEBUG_IntraInsert
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
						}
						else
						{
							if((in == this->initSol->getNbClientsForRoute(r)) && (mn == 1))
							{
								seq1 = this->initSol->getRouteBackSeq()[r][in][mn];
							}
							else
							{
								seq1 = this->initSol->getRouteForwSeq()[r][0][mn-1];
							}
#ifdef DEBUG_IntraInsert
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
							if(in == mn+1)
							{
								seq2 = this->initSol->getRouteBackSeq()[r][mn][in-mn];
							}
							else
							{
								seq2 = this->initSol->getRouteForwSeq()[r][mn][(in-1)-mn]; // to verify
							}
#ifdef DEBUG_IntraInsert
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
							int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
							seq3 = this->initSol->getRouteForwSeq()[r][in+1][indexLastNodeRoute-(in+1)];

#ifdef DEBUG_IntraInsert
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
						}

						// evaluate the feasibility for the concatenation of the 3 sub-sequences
						// then compare the cost of the new route to the former one
						double newCostR;
						if(this->initSol->EVALN(&newCostR,3, seq1, seq2, seq3) && newCostR < routeCost - 0.0001)
						{
							// apply the move

							// remove insertNode
							insertNode->removeNode();

							if(addAfter)
							{
								// insert insertNode after moveNode
								moveNode->insertAfter(insertNode);
							}
							else
							{
								// insert insertNode before moveNode
								moveNode->insertBefore(insertNode);
							}

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

							breakIfImproved = true;
							retVal = true;
						}
					}
				}
			}
		}
		else if(this->initSol->getNbClientsForRoute(r) > 3)
		{// route contains more than 3 clients

			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraInsert
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *insertNodePrev = NULL;
			Node *insertNodeNext = NULL;

			int in = 1;
			for(Node *insertNode = this->initSol->getRoutes()[r].first->getNext(); !insertNode->isDepot() && breakIfImproved == false; insertNode = insertNode->getNext(), in++)
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
				int mn = 1;
				for(Node *moveNode = this->initSol->getRoutes()[r].first->getNext(); !moveNode->isDepot() && breakIfImproved == false; moveNode = moveNode->getNext(), mn++)
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
#ifdef DEBUG_IntraInsert
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
							if(in+1 == mn)
							{
								seq2 = this->initSol->getRouteBackSeq()[r][in][mn-in];
							}
							else
							{
								seq2 = this->initSol->getRouteForwSeq()[r][in+1][mn-(in+1)];
							}
#ifdef DEBUG_IntraInsert
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
							// we have subSeq seq3 under some conditions
							if(((in+1) != mn) && ( in != 1 || mn != this->initSol->getNbClientsForRoute(r))) //this->initSol->getRouteForwSeq()[r].size()-2
							{
								seq3 = this->initSol->getRouteForwSeq()[r][in][0];

#ifdef DEBUG_IntraInsert
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
							}

							if((in == 1) && (mn == this->initSol->getNbClientsForRoute(r)))
							{
								seq4 = this->initSol->getRouteBackSeq()[r][in-1][in];
							}
							else
							{
								int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
								seq4 = this->initSol->getRouteForwSeq()[r][mn+1][indexLastNodeRoute-(mn+1)];
							}
#ifdef DEBUG_IntraInsert
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
						}
						else
						{
							if((in == this->initSol->getNbClientsForRoute(r)) && (mn == 1))
							{
								seq1 = this->initSol->getRouteBackSeq()[r][in][mn];
							}
							else
							{
								seq1 = this->initSol->getRouteForwSeq()[r][0][mn-1];
							}
#ifdef DEBUG_IntraInsert
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
							// we have subSeq seq2 under some conditions
							if(((in+1) != mn) && ( in != 1 || mn != this->initSol->getNbClientsForRoute(r)))
							{
								seq2 = this->initSol->getRouteForwSeq()[r][in][0];
#ifdef DEBUG_IntraInsert
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
							}

							if(in == mn+1)
							{
								seq3 = this->initSol->getRouteBackSeq()[r][mn][in-mn];
							}
							else
							{
								seq3 = this->initSol->getRouteForwSeq()[r][mn][(in-1)-mn]; // to verify
							}
#ifdef DEBUG_IntraInsert
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
							int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;

							seq4 = this->initSol->getRouteForwSeq()[r][in+1][indexLastNodeRoute-(in+1)];

#ifdef DEBUG_IntraInsert
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
						}


						double newCostR;
						if(addAfter)
						{
							if(((in+1) != mn) && ( in != 1 || mn != this->initSol->getNbClientsForRoute(r)))
							{
								// evaluate the feasibility for the concatenation of the 3 sub-sequences
								// then compare the cost of the new route to the former one
								if(this->initSol->EVALN(&newCostR, 4, seq1, seq2, seq3, seq4) && newCostR < routeCost - 0.0001)
								{// Apply the move

									// remove insertNode
									insertNode->removeNode();

									// insert insertNode after moveNode
									moveNode->insertAfter(insertNode);

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

									retVal = true;

									breakIfImproved = true;
								}
							}
							else
							{
								// evaluate the feasibility for the concatenation of the 3 sub-sequences
								// then compare the cost of the new route to the former one
								if(this->initSol->EVALN(&newCostR, 3, seq1, seq2, seq4) && newCostR < routeCost - 0.0001)
								{// Apply the move

									// remove insertNode
									insertNode->removeNode();

									// insert insertNode after moveNode
									moveNode->insertAfter(insertNode);

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

									retVal = true;

									breakIfImproved = true;
								}
							}
						}
						else
						{
							if(((in+1) != mn) && ( in != 1 || mn != this->initSol->getNbClientsForRoute(r)))
							{
								// evaluate the feasibility for the concatenation of the 3 sub-sequences
								// then compare the cost of the new route to the former one
								if(this->initSol->EVALN(&newCostR, 4, seq1, seq2, seq3, seq4) && newCostR < routeCost - 0.0001)
								{// Apply the move

									// remove insertNode
									insertNode->removeNode();

									// insert insertNode after moveNode
									moveNode->insertAfter(insertNode);

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

									retVal = true;

									breakIfImproved = true;

								}
							}
							else
							{
								// evaluate the feasibility for the concatenation of the 3 sub-sequences
								// then compare the cost of the new route to the former one
								if(this->initSol->EVALN(&newCostR, 3, seq1, seq3, seq4) && newCostR < routeCost - 0.0001)
								{// Apply the move

									// remove insertNode
									insertNode->removeNode();

									// insert insertNode after moveNode
									moveNode->insertAfter(insertNode);

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

									retVal = true;

									breakIfImproved = true;
								}
							}
						}
					}
				}
			}
		}
	}

	return retVal;
}

bool LocalSearch::IntraRouteArcInsert()
{
#ifdef DEBUG_IntraArcInsert
	cout << endl;
	cout << "local search IntraRouteArcInsert initial route = " ; this->initSol->PrintSolution(true) ;
#endif

	// return value
	bool retVal = false;

	for(int r = 0; r < this->initSol->getRoutesNumber(); r++)
	{
		// record each route cost
		double routeCost;
		bool breakIfImproved = false; // stop insertion move for a route if improved

		// if a route contains less than 2 customers then no move
		// intraRouteArcInsert movement start with more than 3 customers
		if(this->initSol->getNbClientsForRoute(r) == 3)
		{// route contains 3 customers
			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraArcInsert
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *insertNodeFirstPrev = NULL;
			Node *insertNodeLastNext = NULL;

			Node *insertNodeFirst = NULL;
			Node *insertNodeLast = NULL;

			int in1 = 1;
			int in2 = in1+1; /* 0--a--b--c--0, in1 = 1, in2 = 2 --> in1-in2 = a-b   */
			for(Node *insertNode = this->initSol->getRoutes()[r].first->getNext(); !insertNode->isDepot() && breakIfImproved == false; insertNode = insertNode->getNext(), in1++,in2++)
			{
				insertNodeFirst = insertNode;
				insertNodeLast = insertNodeFirst->getNext();

				if(!insertNodeLast->isDepot())
				{
#ifdef DEBUG_IntraArcInsert
					cout << " ------ insert arc = " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << " ----------" <<  " in1 = " << in1 << "---------- " << "in2 = " << in2 << endl;
#endif
					insertNodeFirstPrev = insertNode->getPrevious();
					insertNodeLastNext = insertNodeLast->getNext();
#ifdef DEBUG_IntraArcInsert
					if(insertNodeFirstPrev != NULL)
						cout << "insertNodeFirstPrev = " << insertNodeFirstPrev->getClient()->getId() << " ;" ;
					if(insertNodeLastNext != NULL)
						cout << " insertNodeLastNext = " << insertNodeLastNext->getClient()->getId() << endl;
					cout << endl;
#endif
					bool addAfter = false;
					int mn = 1; /* 0--a--b--c--0, mn = 1 --> mn = a   */
					for(Node *moveNode = this->initSol->getRoutes()[r].first->getNext(); !moveNode->isDepot() && breakIfImproved == false; moveNode = moveNode->getNext(), mn++)
					{
#ifdef DEBUG_IntraArcInsert
						cout << "---------- moveNode " << " " << " = " << moveNode->getClient()->getId() << " ---------- " << " mn = " << "" << mn << endl;
#endif
						// if insertNodeFirst == moveNode or insertNodeLast == moveNode do nothing and addAfter = true
						// or if (in1 = mn || in2 = mn) do nothing and addAfter = true
						if(insertNodeFirst == moveNode || insertNodeLast == moveNode)
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
								seq1 = this->initSol->getRouteBackSeq()[r][mn][mn-in2];
#ifdef DEBUG_IntraArcInsert
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
								seq2 = this->initSol->getRouteForwSeq()[r][in1][in2-in1];
#ifdef DEBUG_IntraArcInsert
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
								int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
								seq3 = this->initSol->getRouteForwSeq()[r][mn+1][indexLastNodeRoute-(mn+1)];
#ifdef DEBUG_IntraArcInsert
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
							}
							else
							{
								seq1 = this->initSol->getRouteForwSeq()[r][0][mn-1];
#ifdef DEBUG_IntraArcInsert
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
								seq2 = this->initSol->getRouteForwSeq()[r][in1][in2-in1];
#ifdef DEBUG_IntraArcInsert
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
								seq3 = this->initSol->getRouteBackSeq()[r][0][mn];
#ifdef DEBUG_IntraArcInsert
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
							}


							// evaluate the feasibility for the concatenation of the 3 sub-sequences
							// then compare the cost of the new route to the former one
							double newCostR;
							if(this->initSol->EVALN(&newCostR,3, seq1, seq2, seq3) && newCostR < routeCost - 0.0001)
							{
								// apply the move

								// remove insertNodeFirst
								insertNodeFirst->removeNode();
								// remove insertNodeLast
								insertNodeLast->removeNode();

								if(addAfter)
								{
									// insert insertNodeFirst after moveNode
									moveNode->insertAfter(insertNodeFirst);
									// insert insertNodeLast after insertNodeFirst
									insertNodeFirst->insertAfter(insertNodeLast);
								}
								else
								{
									// insert insertNodeLast before moveNode
									moveNode->insertBefore(insertNodeLast);
									// insert insertNodeFirst before insertNodeLast
									insertNodeLast->insertBefore(insertNodeFirst);
								}

								// update sub-sequences of the route
								this->initSol->updateOneRouteSetSubSeq(r);

								// update objective functions
#ifdef DEBUG_IntraArcInsert
								cout << " newCostR = " << newCostR << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraArcInsert
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraArcInsert
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);

								breakIfImproved = true;
								retVal = true;
							}
						}
					}
				}
			}

		}
		else if(this->initSol->getNbClientsForRoute(r) > 3)
		{
			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraArcInsert
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *insertNodeFirstPrev = NULL;
			Node *insertNodeLastNext = NULL;

			Node *insertNodeFirst = NULL;
			Node *insertNodeLast = NULL;

			int in1 = 1;
			int in2 = in1+1; /* 0--a--b--c--d--0, in1 = 1, in2 = 2 --> in1-in2 = a-b   */
			for(Node *insertNode = this->initSol->getRoutes()[r].first->getNext(); !insertNode->isDepot() && breakIfImproved == false; insertNode = insertNode->getNext(), in1++,in2++)
			{
				insertNodeFirst = insertNode;
				insertNodeLast = insertNodeFirst->getNext();

				if(!insertNodeLast->isDepot())
				{
#ifdef DEBUG_IntraArcInsert
					cout << " ------ insert arc = " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << " ----------" <<  " in1 = " << in1 << "---------- " << "in2 = " << in2  << endl;
#endif
					insertNodeFirstPrev = insertNode->getPrevious();
					insertNodeLastNext = insertNodeLast->getNext();
#ifdef DEBUG_IntraArcInsert
					if(insertNodeFirstPrev != NULL)
						cout << "insertNodeFirstPrev = " << insertNodeFirstPrev->getClient()->getId() << " ;" ;
					if(insertNodeLastNext != NULL)
						cout << " insertNodeLastNext = " << insertNodeLastNext->getClient()->getId() << endl;
					cout << endl;
#endif
					bool addAfter = false;
					int mn = 1; /* 0--a--b--c--d--0, mn = 1 --> mn = a   */
					for(Node *moveNode = this->initSol->getRoutes()[r].first->getNext(); !moveNode->isDepot() && breakIfImproved == false; moveNode = moveNode->getNext(), mn++)
					{
#ifdef DEBUG_IntraArcInsert
						cout << "---------- moveNode " << " " << " = " << moveNode->getClient()->getId() << " ---------- " << " mn = " << "" << mn << endl;
#endif
						// if insertNodeFirst == moveNode or insertNodeLast == moveNode do nothing and addAfter = true
						// or if (in1 = mn || in2 = mn) do nothing and addAfter = true
						if(insertNodeFirst == moveNode || insertNodeLast == moveNode)
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
								seq1 = this->initSol->getRouteForwSeq()[r][0][in1-1];
#ifdef DEBUG_IntraArcInsert
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
								seq2 = this->initSol->getRouteForwSeq()[r][in2+1][mn-(in2+1)];
#ifdef DEBUG_IntraArcInsert
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
								seq3 = this->initSol->getRouteForwSeq()[r][in1][in2-in1];
#ifdef DEBUG_IntraArcInsert
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
								int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
								seq4 = this->initSol->getRouteForwSeq()[r][mn+1][indexLastNodeRoute-(mn+1)];
#ifdef DEBUG_IntraArcInsert
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
							}
							else
							{
								seq1 = this->initSol->getRouteForwSeq()[r][0][mn-1];
#ifdef DEBUG_IntraArcInsert
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
								seq2 = this->initSol->getRouteForwSeq()[r][in1][in2-in1];
#ifdef DEBUG_IntraArcInsert
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
								seq3 = this->initSol->getRouteForwSeq()[r][mn][(in1-1)-mn];
#ifdef DEBUG_IntraArcInsert
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
							int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
							seq4 = this->initSol->getRouteForwSeq()[r][in2+1][indexLastNodeRoute-(in2+1)];
#ifdef DEBUG_IntraArcInsert
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
							}


							// evaluate the feasibility for the concatenation of the 3 sub-sequences
							// then compare the cost of the new route to the former one
							double newCostR;
							if(this->initSol->EVALN(&newCostR,4, seq1, seq2, seq3, seq4) && newCostR < routeCost - 0.0001)
							{
								// apply the move

								// remove insertNodeFirst
								insertNodeFirst->removeNode();
								// remove insertNodeLast
								insertNodeLast->removeNode();

								if(addAfter)
								{
									// insert insertNodeFirst after moveNode
									moveNode->insertAfter(insertNodeFirst);
									// insert insertNodeLast after insertNodeFirst
									insertNodeFirst->insertAfter(insertNodeLast);
								}
								else
								{
									// insert insertNodeLast before moveNode
									moveNode->insertBefore(insertNodeLast);
									// insert insertNodeFirst before insertNodeLast
									insertNodeLast->insertBefore(insertNodeFirst);
								}

								// update sub-sequences of the route
								this->initSol->updateOneRouteSetSubSeq(r);

								// update objective functions
#ifdef DEBUG_IntraArcInsert
								cout << " newCostR = " << newCostR << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraArcInsert
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraArcInsert
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);

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

bool LocalSearch::IntraRouteSwap()
{
#ifdef DEBUG_IntraSwap
	cout << endl;
	cout << "local search IntraRouteArcInsert initial route = " ; this->initSol->PrintSolution(true) ;
#endif

	// return value
	bool retVal = false;

	for(int r = 0; r < this->initSol->getRoutesNumber(); r++)
	{
		// record each route cost
		double routeCost;
		bool breakIfImproved = false; // stop insertion move for a route if improved

		// if a route contains less than 2 customers then no move
		// intraRouteSwap movement start with more than 2 customers
		if(this->initSol->getNbClientsForRoute(r) == 3)
		{// route contains 3 customers
			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraSwap
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *swapNodeFirstPrev = NULL;

			int s1 = 1;
			for(Node *swapNodeFirst = this->initSol->getRoutes()[r].first->getNext(); !swapNodeFirst->isDepot() && breakIfImproved == false; swapNodeFirst = swapNodeFirst->getNext(), s1++)
			{
#ifdef DEBUG_IntraSwap
					cout << " ------ swapNodeFirst = " << swapNodeFirst->getClient()->getId() << " ----------" <<  " s1 = " << s1 << endl;
#endif
					swapNodeFirstPrev = swapNodeFirst->getPrevious();
#ifdef DEBUG_IntraSwap
					if(swapNodeFirstPrev != NULL)
						cout << "swapNodeFirstPrev = " << swapNodeFirstPrev->getClient()->getId() << endl ;
#endif
					Node *swapNodeSecondNext = NULL;

					int s2 = s1+1;
					for(Node *swapNodeSecond = swapNodeFirst->getNext(); !swapNodeSecond->isDepot() && breakIfImproved == false; swapNodeSecond = swapNodeSecond->getNext(), s2++)
					{
#ifdef DEBUG_IntraSwap
						cout << " ------ swapNodeSecond = " << swapNodeSecond->getClient()->getId() << " ----------" <<  " s2 = " << s2 << endl;
#endif
						swapNodeSecondNext = swapNodeSecond->getNext();
#ifdef DEBUG_IntraSwap
						if(swapNodeSecondNext != NULL)
							cout << "swapNodeSecondNext = " << swapNodeSecondNext->getClient()->getId() << endl ;
#endif
						// if swapNodeFirst is the first client and swapNodeSecond is the last one
						// then evaluate the inverse route
						if((s1 == 1) && (s2 == this->initSol->getNbClientsForRoute(r)))
						{
							double newCostR = this->initSol->getRouteBackSeq()[r][0][s2+1]->getDistance();
							if(newCostR < routeCost - 0.0001)
							{
								// remove swapNodeFirst
								swapNodeFirst->removeNode();
								// remove swapNodeSecond
								swapNodeSecond->removeNode();

								// insert swapNodeSecond after swapNodeFirstPrev
								swapNodeFirstPrev->insertAfter(swapNodeSecond);
								// insert swapNodeFirst before swapNodeSecondNext
								swapNodeSecondNext->insertBefore(swapNodeFirst);

								// update sub-sequences of the route
								this->initSol->updateOneRouteSetSubSeq(r);

								// update objective functions
#ifdef DEBUG_IntraSwap
								cout << " newCostR = " << newCostR << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraSwap
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraSwap
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);

								breakIfImproved = true;
								retVal = true;

							}
						}
						else if( (s1+1 == s2) || (s1+1 == s2-1))
						{
							SeqData *seq1 = NULL;
							SeqData *seq2 = NULL;
							SeqData *seq3 = NULL;

							seq1 = this->initSol->getRouteForwSeq()[r][0][s1-1];
#ifdef DEBUG_IntraSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
							seq2 = this->initSol->getRouteBackSeq()[r][s1][s2-s1];
#ifdef DEBUG_IntraSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
							int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
							seq3 = this->initSol->getRouteForwSeq()[r][s2+1][indexLastNodeRoute - (s2+1)];
#ifdef DEBUG_IntraSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif

							double newCostR;
							if(this->initSol->EVALN(&newCostR,3, seq1, seq2, seq3) && newCostR < routeCost - 0.0001)
							{
								// remove swapNodeFirst
								swapNodeFirst->removeNode();
								// remove swapNodeSecond
								swapNodeSecond->removeNode();

								// insert swapNodeSecond after swapNodeFirstPrev
								swapNodeFirstPrev->insertAfter(swapNodeSecond);
								// insert swapNodeFirst before swapNodeSecondNext
								swapNodeSecondNext->insertBefore(swapNodeFirst);

								// update sub-sequences of the route
								this->initSol->updateOneRouteSetSubSeq(r);

								// update objective functions
#ifdef DEBUG_IntraSwap
								cout << " newCostR = " << newCostR << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraSwap
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraSwap
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);

								breakIfImproved = true;
								retVal = true;
							}
						}

					}

			}
		}
		else if(this->initSol->getNbClientsForRoute(r) > 3)
		{
			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraSwap
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *swapNodeFirstPrev = NULL;

			int s1 = 1;
			for(Node *swapNodeFirst = this->initSol->getRoutes()[r].first->getNext(); !swapNodeFirst->isDepot() && breakIfImproved == false; swapNodeFirst = swapNodeFirst->getNext(), s1++)
			{
#ifdef DEBUG_IntraSwap
					cout << " ------ swapNodeFirst = " << swapNodeFirst->getClient()->getId() << " ----------" <<  " s1 = " << s1 << endl;
#endif
					swapNodeFirstPrev = swapNodeFirst->getPrevious();
#ifdef DEBUG_IntraSwap
					if(swapNodeFirstPrev != NULL)
						cout << "swapNodeFirstPrev = " << swapNodeFirstPrev->getClient()->getId() << endl ;
#endif
					Node *swapNodeSecondNext = NULL;

					int s2 = s1+1;
					for(Node *swapNodeSecond = swapNodeFirst->getNext(); !swapNodeSecond->isDepot() && breakIfImproved == false; swapNodeSecond = swapNodeSecond->getNext(), s2++)
					{
#ifdef DEBUG_IntraSwap
						cout << " ------ swapNodeSecond = " << swapNodeSecond->getClient()->getId() << " ----------" <<  " s2 = " << s2 << endl;
#endif
						swapNodeSecondNext = swapNodeSecond->getNext();
#ifdef DEBUG_IntraSwap
						if(swapNodeSecondNext != NULL)
							cout << "swapNodeSecondNext = " << swapNodeSecondNext->getClient()->getId() << endl ;
#endif
						// find sub-sequences
						SeqData *seq1 = NULL;
						SeqData *seq2 = NULL;
						SeqData *seq3 = NULL;
						SeqData *seq4 = NULL;
						SeqData *seq5 = NULL;

						// if swapNodeFirst is the first client and swapNodeSecond is the last one
						// then determine 3 sub-sequences
						if((s1 == 1) && (s2 == this->initSol->getNbClientsForRoute(r)))
						{
							int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
							seq1 = this->initSol->getRouteBackSeq()[r][s2][indexLastNodeRoute - s2];
#ifdef DEBUG_IntraSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
							seq2 = this->initSol->getRouteForwSeq()[r][s1+1][(s2-1)-(s1+1)];
#ifdef DEBUG_IntraSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif

							seq3 = this->initSol->getRouteBackSeq()[r][0][s1];
#ifdef DEBUG_IntraSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
							double newCostR;
							if(this->initSol->EVALN(&newCostR,3, seq1, seq2, seq3) && newCostR < routeCost - 0.0001)
							{
								// remove swapNodeFirst
								swapNodeFirst->removeNode();
								// remove swapNodeSecond
								swapNodeSecond->removeNode();

								// insert swapNodeSecond after swapNodeFirstPrev
								swapNodeFirstPrev->insertAfter(swapNodeSecond);
								// insert swapNodeFirst before swapNodeSecondNext
								swapNodeSecondNext->insertBefore(swapNodeFirst);

								// update sub-sequences of the route
								this->initSol->updateOneRouteSetSubSeq(r);

								// update objective functions
#ifdef DEBUG_IntraSwap
								cout << " newCostR = " << newCostR << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraSwap
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraSwap
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);

								breakIfImproved = true;
								retVal = true;
							}
						}
						else if( (s1+1 == s2) || (s1+1 == s2-1))
						{
							seq1 = this->initSol->getRouteForwSeq()[r][0][s1-1];
#ifdef DEBUG_IntraSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
							seq2 = this->initSol->getRouteBackSeq()[r][s1][s2-s1];
#ifdef DEBUG_IntraSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
							int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
							seq3 = this->initSol->getRouteForwSeq()[r][s2+1][indexLastNodeRoute - (s2+1)];
#ifdef DEBUG_IntraSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
							double newCostR;
							if(this->initSol->EVALN(&newCostR,3, seq1, seq2, seq3) && newCostR < routeCost - 0.0001)
							{
								// remove swapNodeFirst
								swapNodeFirst->removeNode();
								// remove swapNodeSecond
								swapNodeSecond->removeNode();

								// insert swapNodeSecond after swapNodeFirstPrev
								swapNodeFirstPrev->insertAfter(swapNodeSecond);
								// insert swapNodeFirst before swapNodeSecondNext
								swapNodeSecondNext->insertBefore(swapNodeFirst);

								// update sub-sequences of the route
								this->initSol->updateOneRouteSetSubSeq(r);

								// update objective functions
#ifdef DEBUG_IntraSwap
								cout << " newCostR = " << newCostR << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraSwap
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraSwap
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);

								breakIfImproved = true;
								retVal = true;
							}
						}
						else if( s1+1 != s2-1)
						{
							seq1 = this->initSol->getRouteForwSeq()[r][0][s1-1];
#ifdef DEBUG_IntraSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
							seq2 = this->initSol->getRouteForwSeq()[r][s2][0];
#ifdef DEBUG_IntraSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
							seq3 = this->initSol->getRouteForwSeq()[r][s1+1][(s2-1)-(s1+1)];
#ifdef DEBUG_IntraSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
							seq4 = this->initSol->getRouteForwSeq()[r][s1][0];
#ifdef DEBUG_IntraSwap
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
							int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
							seq5 = this->initSol->getRouteForwSeq()[r][s2+1][indexLastNodeRoute - (s2+1)];
#ifdef DEBUG_IntraSwap
							cout << seq5->getHead()->getClient()->getId() << "--" << seq5->getTail()->getClient()->getId() << endl;
#endif
							double newCostR;
							if(this->initSol->EVALN(&newCostR,5, seq1, seq2, seq3, seq4, seq5) && newCostR < routeCost - 0.0001)
							{
								// remove swapNodeFirst
								swapNodeFirst->removeNode();
								// remove swapNodeSecond
								swapNodeSecond->removeNode();

								// insert swapNodeSecond after swapNodeFirstPrev
								swapNodeFirstPrev->insertAfter(swapNodeSecond);
								// insert swapNodeFirst before swapNodeSecondNext
								swapNodeSecondNext->insertBefore(swapNodeFirst);

								// update sub-sequences of the route
								this->initSol->updateOneRouteSetSubSeq(r);

								// update objective functions
#ifdef DEBUG_IntraSwap
								cout << " newCostR = " << newCostR << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraSwap
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraSwap
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);

								breakIfImproved = true;
								retVal = true;
							}
						}
					}
			}

		}
	}

	return retVal;
}

bool LocalSearch::IntraRouteArcSwap()
{
#ifdef DEBUG_IntraArcSwap
	cout << endl;
	cout << "local search IntraRouteArcSwap initial route = " ; this->initSol->PrintSolution(true) ;
#endif

	// return value
	bool retVal = false;

	for(int r = 0; r < this->initSol->getRoutesNumber(); r++)
	{
		// record each route cost
		double routeCost;
		bool breakIfImproved = false; // stop insertion move for a route if improved

		// if a route contains less than 2 customers then no move
		// intraRouteSwap movement start with more than 2 customers
		if(this->initSol->getNbClientsForRoute(r) == 3)
		{
			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraArcSwap
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *arcFirstNode = NULL; // the first node of the arc
			Node *arcLastNode = NULL; // the last node of the arc

			Node *arcFirstNodePrev = NULL;
			Node *arcLastNodeNext = NULL;

			int a1 = 1; /* a1 is the index of first client in arc, a2 is the one of second one */
			int a2 = a1+1; /* 0--b--c--d--0, a1 = 1, a2 = 2 --> a1--a2 = b--c   */
			for(arcFirstNode = this->initSol->getRoutes()[r].first->getNext(); !arcFirstNode->isDepot() && breakIfImproved == false; arcFirstNode = arcFirstNode->getNext(), a1++, a2++)
			{
#ifdef DEBUG_IntraArcSwap
		cout << endl;
#endif
				arcLastNode = arcFirstNode->getNext();

				// if the second client in the arc is not the depot
				if(!arcLastNode->isDepot())
				{
#ifdef DEBUG_IntraArcSwap
					cout << " ------ insert arc = " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() << " ----------" <<  " a1 = " << a1 << "---------- " << "a2 = " << a2  << endl;
#endif
					arcFirstNodePrev = arcFirstNode->getPrevious();
					arcLastNodeNext = arcLastNode->getNext();
#ifdef DEBUG_IntraArcSwap
					if(arcFirstNodePrev != NULL)
						cout << "arcFirstNodePrev = " << arcFirstNodePrev->getClient()->getId() << " ; " ;
					if(arcLastNodeNext != NULL)
						cout << "arcLastNodeNext = " << arcLastNodeNext->getClient()->getId() << endl;
#endif
					bool arcBefore = false;
					Node *swapNodeNext = NULL;
					Node *swapNodePrev = NULL;
					int s = 1; /* 0--b--c--d--0, a1 = 1 --> a1 = b   */
					for(Node *swapNode = this->initSol->getRoutes()[r].first->getNext(); !swapNode->isDepot() && breakIfImproved == false; swapNode = swapNode->getNext(),s++)
					{
#ifdef DEBUG_IntraArcSwap
						cout << " ------ swapNode = " << swapNode->getClient()->getId() << " ----------" <<  " s = " << s << endl;
#endif
						if( a1 == s || a2 == s)
						{
							arcBefore = true;
						}
						else
						{
							swapNodeNext = swapNode->getNext();
							swapNodePrev = swapNode->getPrevious();
#ifdef DEBUG_IntraArcSwap
							if(swapNodePrev != NULL)
								cout << "swapNodePrev = " << swapNodePrev->getClient()->getId() <<" ; " ;
							if(swapNodeNext != NULL)
								cout << "swapNodeNext = " << swapNodeNext->getClient()->getId() << endl;
#endif
							// find sub-sequences
							SeqData *seq1 = NULL;
							SeqData *seq2 = NULL;
							SeqData *seq3 = NULL;
							if(arcBefore)
							{
								int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
								seq1 = this->initSol->getRouteBackSeq()[r][s][indexLastNodeRoute-s];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
								seq2 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
								seq3 = this->initSol->getRouteForwSeq()[r][s+1][indexLastNodeRoute-(s+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
							}
							else
							{
								seq1 = this->initSol->getRouteForwSeq()[r][0][s-1];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
								seq2 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
								seq3 = this->initSol->getRouteBackSeq()[r][0][s];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
							}

							double newCostR;
							if(this->initSol->EVALN(&newCostR,3, seq1, seq2, seq3) && newCostR < routeCost - 0.0001)
							{
								// remove arcFirstNode
								arcFirstNode->removeNode();

								// remove arcLastNode
								arcLastNode->removeNode();

								// remove swapNode
								swapNode->removeNode();

								if(arcBefore)
								{
									// insert arcFirstNode before swapNodeNext
									swapNodeNext->insertBefore(arcFirstNode);
									// insert arcLastNode before swapNodeNext
									swapNodeNext->insertBefore(arcLastNode);
									// insert swapNode after arcFirstNodePrev
									arcFirstNodePrev->insertAfter(swapNode);
								}
								else
								{
									// insert arcFirstNode after swapNodePrev
									swapNodePrev->insertAfter(arcFirstNode);
									// insert arcLastNode after arcFirstNode
									arcFirstNode->insertAfter(arcLastNode);
									// insert swapNode before arcLastNodeNext
									arcLastNodeNext->insertBefore(swapNode);
								}


								// update sub-sequences of the route
								this->initSol->updateOneRouteSetSubSeq(r);

								// update objective functions
#ifdef DEBUG_IntraArcSwap
								cout << " newCostR = " << newCostR << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_IntraArcSwap
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + newCostR;
#ifdef DEBUG_IntraArcSwap
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);

								breakIfImproved = true;
								retVal = true;
							}
						}
					}

				}
			}


		}
		else if(this->initSol->getNbClientsForRoute(r) == 4)
		{
			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraArcSwap
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *arcFirstNode = NULL; // the first node of the arc
			Node *arcLastNode = NULL; // the last node of the arc

			Node *arcFirstNodePrev = NULL;
			Node *arcLastNodeNext = NULL;

			int a1 = 1;
			int a2 = a1+1; /* 0--b--c--d--0, a1 = 1, a2 = 2 --> a1--a2 = b--c */
			for(arcFirstNode = this->initSol->getRoutes()[r].first->getNext(); !arcFirstNode->isDepot() && breakIfImproved == false; arcFirstNode = arcFirstNode->getNext(), a1++, a2++)
			{
#ifdef DEBUG_IntraArcSwap
		cout << endl;
#endif
				arcLastNode = arcFirstNode->getNext();

				if(!arcLastNode->isDepot())
				{
#ifdef DEBUG_IntraArcSwap
					cout << " ------ insert arc = " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() << " ----------" <<  " a1 = " << a1 << "---------- " << "a2 = " << a2  << endl;
#endif
					arcFirstNodePrev = arcFirstNode->getPrevious();
					arcLastNodeNext = arcLastNode->getNext();
#ifdef DEBUG_IntraArcSwap
					if(arcFirstNodePrev != NULL)
						cout << "arcFirstNodePrev = " << arcFirstNodePrev->getClient()->getId() << " ; " ;
					if(arcLastNodeNext != NULL)
						cout << "arcLastNodeNext = " << arcLastNodeNext->getClient()->getId() << endl;
#endif
					bool arcBefore = false;
					Node *swapNodeNext = NULL;
					Node *swapNodePrev = NULL;
					int s = 1; /* 0--b--c--d--0, a1 = 1 --> a1 = b   */
					for(Node *swapNode = this->initSol->getRoutes()[r].first->getNext(); !swapNode->isDepot() && breakIfImproved == false; swapNode = swapNode->getNext(),s++)
					{
#ifdef DEBUG_IntraArcSwap
						cout << " ------ swapNode = " << swapNode->getClient()->getId() << " ----------" <<  " s = " << s << endl;
#endif
						if( a1 == s || a2 == s)
						{
							arcBefore = true;
						}
						else
						{
							swapNodeNext = swapNode->getNext();
							swapNodePrev = swapNode->getPrevious();
#ifdef DEBUG_IntraArcSwap
							if(swapNodePrev != NULL)
								cout << "swapNodePrev = " << swapNodePrev->getClient()->getId() <<" ; " ;
							if(swapNodeNext != NULL)
								cout << "swapNodeNext = " << swapNodeNext->getClient()->getId() << endl;
#endif
							// find sub-sequences
							SeqData *seq1 = NULL;
							SeqData *seq2 = NULL;
							SeqData *seq3 = NULL;
							SeqData *seq4 = NULL;

							double newCostR;
							if(arcBefore)
							{
								if(a2+1 == s)
								{
									seq1 = this->initSol->getRouteForwSeq()[r][0][a1-1];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
									seq2 = this->initSol->getRouteBackSeq()[r][a2+1][s-(a2+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
									seq3 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
									int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
									seq4 = this->initSol->getRouteForwSeq()[r][s+1][indexLastNodeRoute-(s+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif

									// evaluate
									if(this->initSol->EVALN(&newCostR,4, seq1, seq2, seq3,seq4) && newCostR < routeCost - 0.0001)
									{
										// remove arcFirstNode
										arcFirstNode->removeNode();

										// remove arcLastNode
										arcLastNode->removeNode();

										// remove swapNode
										swapNode->removeNode();

										// insert arcFirstNode before swapNodeNext
										swapNodeNext->insertBefore(arcFirstNode);
										// insert arcLastNode before swapNodeNext
										swapNodeNext->insertBefore(arcLastNode);
										// insert swapNode after arcFirstNodePrev
										arcFirstNodePrev->insertAfter(swapNode);

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
		#ifdef DEBUG_IntraArcSwap
										cout << " newCostR = " << newCostR << endl;
		#endif
										double restCout = this->initSol->getObjVal() - routeCost;
		#ifdef DEBUG_IntraArcSwap
										cout << "restCout = " << restCout << endl;
		#endif
										double newObjVal = restCout + newCostR;
		#ifdef DEBUG_IntraArcSwap
										cout << "newObjVal = " << newObjVal << endl;
		#endif
										this->initSol->setObjVal(newObjVal);

										breakIfImproved = true;
										retVal = true;
									}
								}
								else
								{
									seq1 = this->initSol->getRouteBackSeq()[r][a2+1][s-a2];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
									seq2 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
									int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
									seq3 = this->initSol->getRouteForwSeq()[r][s+1][indexLastNodeRoute-(s+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
									// evaluate
									if(this->initSol->EVALN(&newCostR,3, seq1, seq2, seq3) && newCostR < routeCost - 0.0001)
									{
										// remove arcFirstNode
										arcFirstNode->removeNode();

										// remove arcLastNode
										arcLastNode->removeNode();

										// remove swapNode
										swapNode->removeNode();

										// insert arcFirstNode before swapNodeNext
										swapNodeNext->insertBefore(arcFirstNode);
										// insert arcLastNode before swapNodeNext
										swapNodeNext->insertBefore(arcLastNode);
										// insert swapNode after arcFirstNodePrev
										arcFirstNodePrev->insertAfter(swapNode);

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
		#ifdef DEBUG_IntraArcSwap
										cout << " newCostR = " << newCostR << endl;
		#endif
										double restCout = this->initSol->getObjVal() - routeCost;
		#ifdef DEBUG_IntraArcSwap
										cout << "restCout = " << restCout << endl;
		#endif
										double newObjVal = restCout + newCostR;
		#ifdef DEBUG_IntraArcSwap
										cout << "newObjVal = " << newObjVal << endl;
		#endif
										this->initSol->setObjVal(newObjVal);

										breakIfImproved = true;
										retVal = true;
									}
								}
							}
							else
							{
								if(a1-1 == s)
								{
									seq1 = this->initSol->getRouteForwSeq()[r][0][a1-1];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
									seq2 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
									seq3 = this->initSol->getRouteBackSeq()[r][s][(a1-1)-s];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
									int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
									seq4 = this->initSol->getRouteForwSeq()[r][s][indexLastNodeRoute-(a2+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
									// evaluate concatenation of the 4 sub-seqs then apply the move if the cost is improved
									if(this->initSol->EVALN(&newCostR,4, seq1, seq2, seq3,seq4) && newCostR < routeCost - 0.0001)
									{
										// remove arcFirstNode
										arcFirstNode->removeNode();

										// remove arcLastNode
										arcLastNode->removeNode();

										// remove swapNode
										swapNode->removeNode();

										// insert arcFirstNode after swapNodePrev
										swapNodePrev->insertAfter(arcFirstNode);
										// insert arcLastNode after arcFirstNode
										arcFirstNode->insertAfter(arcLastNode);
										// insert swapNode before arcLastNodeNext
										arcLastNodeNext->insertBefore(swapNode);

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
		#ifdef DEBUG_IntraArcSwap
										cout << " newCostR = " << newCostR << endl;
		#endif
										double restCout = this->initSol->getObjVal() - routeCost;
		#ifdef DEBUG_IntraArcSwap
										cout << "restCout = " << restCout << endl;
		#endif
										double newObjVal = restCout + newCostR;
		#ifdef DEBUG_IntraArcSwap
										cout << "newObjVal = " << newObjVal << endl;
		#endif
										this->initSol->setObjVal(newObjVal);

										breakIfImproved = true;
										retVal = true;
									}
								}
								else
								{
									seq1 = this->initSol->getRouteForwSeq()[r][0][s-1];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
									seq2 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
									seq3 = this->initSol->getRouteBackSeq()[r][0][(a2-1)-s];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
									// evaluate concatenation of the 3 sub-seqs then apply the move if the cost is improved
									if(this->initSol->EVALN(&newCostR,3, seq1, seq2, seq3) && newCostR < routeCost - 0.0001)
									{
										// remove arcFirstNode
										arcFirstNode->removeNode();

										// remove arcLastNode
										arcLastNode->removeNode();

										// remove swapNode
										swapNode->removeNode();

										// insert arcFirstNode after swapNodePrev
										swapNodePrev->insertAfter(arcFirstNode);
										// insert arcLastNode after arcFirstNode
										arcFirstNode->insertAfter(arcLastNode);
										// insert swapNode before arcLastNodeNext
										arcLastNodeNext->insertBefore(swapNode);

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
		#ifdef DEBUG_IntraArcSwap
										cout << " newCostR = " << newCostR << endl;
		#endif
										double restCout = this->initSol->getObjVal() - routeCost;
		#ifdef DEBUG_IntraArcSwap
										cout << "restCout = " << restCout << endl;
		#endif
										double newObjVal = restCout + newCostR;
		#ifdef DEBUG_IntraArcSwap
										cout << "newObjVal = " << newObjVal << endl;
		#endif
										this->initSol->setObjVal(newObjVal);

										breakIfImproved = true;
										retVal = true;
									}
								}
							}
						}
					}

				}
			}
		}
		else if( this->initSol->getNbClientsForRoute(r) > 4)
		{
			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_IntraArcSwap
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *arcFirstNode = NULL; // the first node of the arc
			Node *arcLastNode = NULL; // the last node of the arc

			Node *arcFirstNodePrev = NULL;
			Node *arcLastNodeNext = NULL;

			int a1 = 1;
			int a2 = a1+1; /* 0--b--c--d--0, a1 = 1, a2 = 2 --> a1--a2 = b--c   */
			for(arcFirstNode = this->initSol->getRoutes()[r].first->getNext(); !arcFirstNode->isDepot() && breakIfImproved == false; arcFirstNode = arcFirstNode->getNext(), a1++, a2++)
			{
#ifdef DEBUG_IntraArcSwap
		cout << endl;
#endif
				arcLastNode = arcFirstNode->getNext();

				if(!arcLastNode->isDepot())
				{
#ifdef DEBUG_IntraArcSwap
					cout << " ------ insert arc = " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() << " ----------" <<  " a1 = " << a1 << "---------- " << "a2 = " << a2  << endl;
#endif
					arcFirstNodePrev = arcFirstNode->getPrevious();
					arcLastNodeNext = arcLastNode->getNext();
#ifdef DEBUG_IntraArcSwap
					if(arcFirstNodePrev != NULL)
						cout << "arcFirstNodePrev = " << arcFirstNodePrev->getClient()->getId() << " ; " ;
					if(arcLastNodeNext != NULL)
						cout << "arcLastNodeNext = " << arcLastNodeNext->getClient()->getId() << endl;
#endif
					bool arcBefore = false;
					Node *swapNodeNext = NULL;
					Node *swapNodePrev = NULL;
					int s = 1; /* 0--b--c--d--0, a1 = 1 --> a1 = b   */
					for(Node *swapNode = this->initSol->getRoutes()[r].first->getNext(); !swapNode->isDepot() && breakIfImproved == false; swapNode = swapNode->getNext(),s++)
					{
#ifdef DEBUG_IntraArcSwap
						cout << " ------ swapNode = " << swapNode->getClient()->getId() << " ----------" <<  " s = " << s << endl;
#endif
						if( a1 == s || a2 == s)
						{
							arcBefore = true;
						}
						else
						{
							swapNodeNext = swapNode->getNext();
							swapNodePrev = swapNode->getPrevious();
#ifdef DEBUG_IntraArcSwap
							if(swapNodePrev != NULL)
								cout << "swapNodePrev = " << swapNodePrev->getClient()->getId() <<" ; " ;
							if(swapNodeNext != NULL)
								cout << "swapNodeNext = " << swapNodeNext->getClient()->getId() << endl;
#endif
							// find sub-sequences
							SeqData *seq1 = NULL;
							SeqData *seq2 = NULL;
							SeqData *seq3 = NULL;
							SeqData *seq4 = NULL;
							SeqData *seq5 = NULL;

							double newCostR;
							if(arcBefore)
							{
								if(a1 == 1 && s == this->initSol->getNbClientsForRoute(r))
								{
									int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
									seq1 = this->initSol->getRouteBackSeq()[r][s][indexLastNodeRoute-s];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
									seq2 = this->initSol->getRouteForwSeq()[r][a2+1][(s-1)-(a2+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
									seq3 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
									seq4 = this->initSol->getRouteForwSeq()[r][s+1][indexLastNodeRoute-(s+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
								}
								else if(a2+1 == s-1 || a2+1 == s)
								{
									seq1 = this->initSol->getRouteForwSeq()[r][0][a1-1];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
									seq2 = this->initSol->getRouteBackSeq()[r][a2+1][s-(a2+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
									seq3 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
									int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
									seq4 = this->initSol->getRouteForwSeq()[r][s+1][indexLastNodeRoute-(s+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
								}
								else
								{
									seq1 = this->initSol->getRouteForwSeq()[r][0][a1-1];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
									seq2 = this->initSol->getRouteForwSeq()[r][s][0];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
									seq3 = this->initSol->getRouteForwSeq()[r][a2+1][(s-1)-(a2+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
									seq4 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
									int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
									seq5 = this->initSol->getRouteForwSeq()[r][s+1][indexLastNodeRoute-(s+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq5->getHead()->getClient()->getId() << "--" << seq5->getTail()->getClient()->getId() << endl;
#endif
								}

								if((a1 == 1 && s == this->initSol->getNbClientsForRoute(r)) || (a2+1 == s-1 || a2+1 == s))
								{
									// evaluate concatenation of the 4 sub-seqs then apply the move if the cost is improved
									if(this->initSol->EVALN(&newCostR,4, seq1, seq2, seq3,seq4) && newCostR < routeCost - 0.0001)
									{
										// remove arcFirstNode
										arcFirstNode->removeNode();

										// remove arcLastNode
										arcLastNode->removeNode();

										// remove swapNode
										swapNode->removeNode();

										// insert arcFirstNode before swapNodeNext
										swapNodeNext->insertBefore(arcFirstNode);
										// insert arcLastNode before swapNodeNext
										swapNodeNext->insertBefore(arcLastNode);
										// insert swapNode arcFirstNodePrev
										arcFirstNodePrev->insertAfter(swapNode);

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
		#ifdef DEBUG_IntraArcSwap
										cout << " newCostR = " << newCostR << endl;
		#endif
										double restCout = this->initSol->getObjVal() - routeCost;
		#ifdef DEBUG_IntraArcSwap
										cout << "restCout = " << restCout << endl;
		#endif
										double newObjVal = restCout + newCostR;
		#ifdef DEBUG_IntraArcSwap
										cout << "newObjVal = " << newObjVal << endl;
		#endif
										this->initSol->setObjVal(newObjVal);

										breakIfImproved = true;
										retVal = true;
									}
								}
								else
								{
									// evaluate concatenation of the 5 sub-seqs then apply the move if the cost is improved
									if(this->initSol->EVALN(&newCostR,5, seq1, seq2, seq3,seq4,seq5) && newCostR < routeCost - 0.0001)
									{
										// remove arcFirstNode
										arcFirstNode->removeNode();

										// remove arcLastNode
										arcLastNode->removeNode();

										// remove swapNode
										swapNode->removeNode();

										// insert arcFirstNode before swapNodeNext
										swapNodeNext->insertBefore(arcFirstNode);
										// insert arcLastNode before swapNodeNext
										swapNodeNext->insertBefore(arcLastNode);
										// insert swapNode arcFirstNodePrev
										arcFirstNodePrev->insertAfter(swapNode);

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
		#ifdef DEBUG_IntraArcSwap
										cout << " newCostR = " << newCostR << endl;
		#endif
										double restCout = this->initSol->getObjVal() - routeCost;
		#ifdef DEBUG_IntraArcSwap
										cout << "restCout = " << restCout << endl;
		#endif
										double newObjVal = restCout + newCostR;
		#ifdef DEBUG_IntraArcSwap
										cout << "newObjVal = " << newObjVal << endl;
		#endif
										this->initSol->setObjVal(newObjVal);

										breakIfImproved = true;
										retVal = true;
									}
								}
							}
							else
							{
								if(s == 1 and a2 == this->initSol->getNbClientsForRoute(r))
								{
									seq1 = this->initSol->getRouteForwSeq()[r][0][s-1];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
									seq2 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
									seq3 = this->initSol->getRouteForwSeq()[r][s+1][(a1-1)-(s+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
									seq4 = this->initSol->getRouteBackSeq()[r][0][s];
#ifdef DEBUG_IntraArcSwap
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
								}
								else if( a1-1 == s+1 || a1-1 == s)
								{
									seq1 = this->initSol->getRouteForwSeq()[r][0][s-1];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
									seq2 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
									seq3 = this->initSol->getRouteBackSeq()[r][a1-1][(a1-1)-(s+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
									int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
									seq4 = this->initSol->getRouteForwSeq()[r][a2+1][indexLastNodeRoute-(a2+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
								}
								else
								{
									seq1 = this->initSol->getRouteForwSeq()[r][0][s-1];
#ifdef DEBUG_IntraArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
									seq2 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_IntraArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
									seq3 = this->initSol->getRouteForwSeq()[r][s+1][(a1-1)-(s+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
									seq4 = this->initSol->getRouteForwSeq()[r][s][0];
#ifdef DEBUG_IntraArcSwap
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
									int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
									seq5 = this->initSol->getRouteForwSeq()[r][a2+1][indexLastNodeRoute-(a2+1)];
#ifdef DEBUG_IntraArcSwap
							cout << seq5->getHead()->getClient()->getId() << "--" << seq5->getTail()->getClient()->getId() << endl;
#endif
								}

								if((s == 1 and a2 == this->initSol->getNbClientsForRoute(r)) || (a1-1 == s+1 || a1-1 == s))
								{
									// evaluate concatenation of the 4 sub-seqs then apply the move if the cost is improved
									if(this->initSol->EVALN(&newCostR,4, seq1, seq2, seq3,seq4) && newCostR < routeCost - 0.0001)
									{
										// remove arcFirstNode
										arcFirstNode->removeNode();

										// remove arcLastNode
										arcLastNode->removeNode();

										// remove swapNode
										swapNode->removeNode();

										// insert arcFirstNode after swapNodePrev
										swapNodePrev->insertAfter(arcFirstNode);
										// insert arcLastNode after arcFirstNode
										arcFirstNode->insertAfter(arcLastNode);
										// insert swapNode before arcLastNodeNext
										arcLastNodeNext->insertBefore(swapNode);

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
		#ifdef DEBUG_IntraArcSwap
										cout << " newCostR = " << newCostR << endl;
		#endif
										double restCout = this->initSol->getObjVal() - routeCost;
		#ifdef DEBUG_IntraArcSwap
										cout << "restCout = " << restCout << endl;
		#endif
										double newObjVal = restCout + newCostR;
		#ifdef DEBUG_IntraArcSwap
										cout << "newObjVal = " << newObjVal << endl;
		#endif
										this->initSol->setObjVal(newObjVal);

										breakIfImproved = true;
										retVal = true;
									}
								}
								else
								{
									// evaluate concatenation of the 5 sub-seqs then apply the move if the cost is improved
									if(this->initSol->EVALN(&newCostR,5, seq1, seq2, seq3,seq4,seq5) && newCostR < routeCost - 0.0001)
									{
										// remove arcFirstNode
										arcFirstNode->removeNode();

										// remove arcLastNode
										arcLastNode->removeNode();

										// remove swapNode
										swapNode->removeNode();

										// insert arcFirstNode after swapNodePrev
										swapNodePrev->insertAfter(arcFirstNode);
										// insert arcLastNode after arcFirstNode
										arcFirstNode->insertAfter(arcLastNode);
										// insert swapNode before arcLastNodeNext
										arcLastNodeNext->insertBefore(swapNode);

										// update sub-sequences of the route
										this->initSol->updateOneRouteSetSubSeq(r);

										// update objective functions
		#ifdef DEBUG_IntraArcSwap
										cout << " newCostR = " << newCostR << endl;
		#endif
										double restCout = this->initSol->getObjVal() - routeCost;
		#ifdef DEBUG_IntraArcSwap
										cout << "restCout = " << restCout << endl;
		#endif
										double newObjVal = restCout + newCostR;
		#ifdef DEBUG_IntraArcSwap
										cout << "newObjVal = " << newObjVal << endl;
		#endif
										this->initSol->setObjVal(newObjVal);

										breakIfImproved = true;
										retVal = true;
									}
								}
							}
						}
					}
				}
			}

		}
	}
	return retVal;
}

bool LocalSearch::IntraRoute2ArcSwap()
{
#ifdef DEBUG_Intra2ArcSwap
	cout << endl;
	cout << "local search IntraRoute2ArcSwap initial route = " ; this->initSol->PrintSolution(true) ;
#endif

	// return value
	bool retVal = false;

	for(int r = 0; r < this->initSol->getRoutesNumber(); r++)
	{
		// record each route cost
		double routeCost;
		bool breakIfImproved = false; // stop insertion move for a route if improved

		// if a route contains less than 4 customers then no move
		// intraRoute2ArcSwap movement start with more than 4 customers
		if(this->initSol->getNbClientsForRoute(r) >= 4)
		{
			routeCost = this->initSol->getRouteForwSeq()[r][0].back()->getDistance();
#ifdef DEBUG_Intra2ArcSwap
			cout << "route[" << r << "]" << " cost = " << routeCost << endl;
#endif
			Node *arcFirstNode = NULL; // the first node of the arc
			Node *arcLastNode = NULL; // the last node of the arc

			Node *arcFirstNodePrev = NULL;

			int a1 = 1;
			int a2 = a1+1; /* 0--b--c--d--0, a1 = 1, a2 = 2 --> a1--a2 = b--c   */
			for(arcFirstNode = this->initSol->getRoutes()[r].first->getNext(); !arcFirstNode->isDepot() && breakIfImproved == false; arcFirstNode = arcFirstNode->getNext(), a1++, a2++)
			{
#ifdef DEBUG_Intra2ArcSwap
				cout << endl;
#endif
				arcLastNode = arcFirstNode->getNext();
#ifdef DEBUG_Intra2ArcSwap
				cout << " ------ swap arc first = " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() << " ----------" <<  " a1 = " << a1 << "---------- " << "a2 = " << a2  << endl;
#endif
				arcFirstNodePrev = arcFirstNode->getPrevious();

#ifdef DEBUG_Intra2ArcSwap
				if(arcFirstNodePrev != NULL)
					cout << "arcFirstNodePrev = " << arcFirstNodePrev->getClient()->getId() <<  endl;
#endif

				Node *swapNodeFirst = NULL; // the first node of the second arc
				Node *swapNodeSecond = NULL; // the last node of the second arc

				Node *swapNodeSecondNext = NULL;

				int s1 = a2+1;
				int s2 = s1+1;
				for(swapNodeFirst = arcLastNode->getNext(); !swapNodeFirst->isDepot() && breakIfImproved == false; swapNodeFirst = swapNodeFirst->getNext(), s1++, s2++)
				{
					swapNodeSecond = swapNodeFirst->getNext();
					if(!swapNodeSecond->isDepot())
					{
#ifdef DEBUG_Intra2ArcSwap
						cout << " ------ swap arc second = " << swapNodeFirst->getClient()->getId() << "--" << swapNodeSecond->getClient()->getId() << " ----------" <<  " s1 = " << s1 << "---------- " << " s2 = " << s2  << endl;
#endif

						swapNodeSecondNext = swapNodeSecond->getNext();
#ifdef DEBUG_Intra2ArcSwap
						if(swapNodeSecondNext != NULL)
							cout << "swapNodeSecondNext = " << swapNodeSecondNext->getClient()->getId() <<  endl;
#endif
						// find sub-sequences
						SeqData *seq1 = NULL;
						SeqData *seq2 = NULL;
						SeqData *seq3 = NULL;
						SeqData *seq4 = NULL;
						SeqData *seq5 = NULL;

						double newCostR;

						if(a2+1 == s1)
						{
							seq1 = this->initSol->getRouteForwSeq()[r][0][a1-1];
#ifdef DEBUG_Intra2ArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
							seq2 = this->initSol->getRouteForwSeq()[r][s1][s2-s1];
#ifdef DEBUG_Intra2ArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
							seq3 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_Intra2ArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif

							int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
							seq4 = this->initSol->getRouteForwSeq()[r][s2+1][indexLastNodeRoute-(s2+1)];
#ifdef DEBUG_Intra2ArcSwap
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif
							// evaluate concatenation of the 4 sub-seqs then apply the move if the cost is improved
							if(this->initSol->EVALN(&newCostR,4, seq1, seq2, seq3,seq4) && newCostR < routeCost - 0.0001)
							{
								// remove arcFirstNode
								arcFirstNode->removeNode();
								// remove arcLastNode
								arcLastNode->removeNode();

								// remove swapNodeFirst
								swapNodeFirst->removeNode();
								// remove swapNodeSecond
								swapNodeSecond->removeNode();

								// insert swapNodeFirst after arcFirstNodePrev
								arcFirstNodePrev->insertAfter(swapNodeFirst);
								// insert swapNodeSecond after swapNodeFirst
								swapNodeFirst->insertAfter(swapNodeSecond);
								// insert arcFirstNode before swapNodeSecondNext
								swapNodeSecondNext->insertBefore(arcFirstNode);
								// insert arcLastNode before swapNodeSecondNext
								swapNodeSecondNext->insertBefore(arcLastNode);

								// update sub-sequences of the route
								this->initSol->updateOneRouteSetSubSeq(r);

								// update objective functions
#ifdef DEBUG_Intra2ArcSwap
								cout << " newCostR = " << newCostR << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_Intra2ArcSwap
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + newCostR;
#ifdef DEBUG_Intra2ArcSwap
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);

								breakIfImproved = true;
								retVal = true;
							}
						}
						else
						{
							seq1 = this->initSol->getRouteForwSeq()[r][0][a1-1];
#ifdef DEBUG_Intra2ArcSwap
							cout << seq1->getHead()->getClient()->getId() << "--" << seq1->getTail()->getClient()->getId() << endl;
#endif
							seq2 = this->initSol->getRouteForwSeq()[r][s1][s2-s1];
#ifdef DEBUG_Intra2ArcSwap
							cout << seq2->getHead()->getClient()->getId() << "--" << seq2->getTail()->getClient()->getId() << endl;
#endif
							seq3 = this->initSol->getRouteForwSeq()[r][a2+1][(s1-1)-(a2+1)];
#ifdef DEBUG_Intra2ArcSwap
							cout << seq3->getHead()->getClient()->getId() << "--" << seq3->getTail()->getClient()->getId() << endl;
#endif
							seq4 = this->initSol->getRouteForwSeq()[r][a1][a2-a1];
#ifdef DEBUG_Intra2ArcSwap
							cout << seq4->getHead()->getClient()->getId() << "--" << seq4->getTail()->getClient()->getId() << endl;
#endif

							int indexLastNodeRoute = this->initSol->getNbClientsForRoute(r)+1;
							seq5 = this->initSol->getRouteForwSeq()[r][s2+1][indexLastNodeRoute-(s2+1)];
#ifdef DEBUG_Intra2ArcSwap
							cout << seq5->getHead()->getClient()->getId() << "--" << seq5->getTail()->getClient()->getId() << endl;
#endif
							// evaluate concatenation of the 5 sub-seqs then apply the move if the cost is improved
							if(this->initSol->EVALN(&newCostR,5, seq1, seq2, seq3,seq4, seq5) && newCostR < routeCost - 0.0001)
							{
								// remove arcFirstNode
								arcFirstNode->removeNode();
								// remove arcLastNode
								arcLastNode->removeNode();

								// remove swapNodeFirst
								swapNodeFirst->removeNode();
								// remove swapNodeSecond
								swapNodeSecond->removeNode();

								// insert swapNodeFirst after arcFirstNodePrev
								arcFirstNodePrev->insertAfter(swapNodeFirst);
								// insert swapNodeSecond after swapNodeFirst
								swapNodeFirst->insertAfter(swapNodeSecond);
								// insert arcFirstNode before swapNodeSecondNext
								swapNodeSecondNext->insertBefore(arcFirstNode);
								// insert arcLastNode before swapNodeSecondNext
								swapNodeSecondNext->insertBefore(arcLastNode);

								// update sub-sequences of the route
								this->initSol->updateOneRouteSetSubSeq(r);

								// update objective functions
#ifdef DEBUG_Intra2ArcSwap
								cout << " newCostR = " << newCostR << endl;
#endif
								double restCout = this->initSol->getObjVal() - routeCost;
#ifdef DEBUG_Intra2ArcSwap
								cout << "restCout = " << restCout << endl;
#endif
								double newObjVal = restCout + newCostR;
#ifdef DEBUG_Intra2ArcSwap
								cout << "newObjVal = " << newObjVal << endl;
#endif
								this->initSol->setObjVal(newObjVal);

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
