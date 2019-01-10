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

void LocalSearch::removeNode(Node* insertNode) {
	// remove insertNode from the list
	if (insertNode == this->initSol->getSequence()->getHead()) {
		this->initSol->getSequence()->setHead(insertNode->getNext());
		this->initSol->getSequence()->getHead()->setPrevious(NULL);
	} else if (insertNode == this->initSol->getSequence()->getTail()) {
		this->initSol->getSequence()->setTail(insertNode->getPrevious());
		this->initSol->getSequence()->getTail()->setNext(NULL);
	} else {
		insertNode->getPrevious()->setNext(insertNode->getNext());
		insertNode->getNext()->setPrevious(insertNode->getPrevious());
//		insertNode->setPrevious(NULL);
//		insertNode->setNext(NULL);
	}

	insertNode->setPrevious(NULL);
	insertNode->setNext(NULL);
}

void LocalSearch::addTail(Node* insertNode) {
	insertNode->setPrevious(this->initSol->getSequence()->getTail());
	this->initSol->getSequence()->getTail()->setNext(insertNode);
	this->initSol->getSequence()->setTail(insertNode);
	this->initSol->getSequence()->getTail()->setNext(NULL);
}

void LocalSearch::addHead(Node* insertNode) {
	insertNode->setNext(this->initSol->getSequence()->getHead());
	this->initSol->getSequence()->getHead()->setPrevious(insertNode);
	this->initSol->getSequence()->setHead(insertNode);
	this->initSol->getSequence()->getHead()->setPrevious(NULL);
}

void LocalSearch::insertAfter(Node* insertNode, Node* moveNode) {
	//insert insertNode after moveNode
	insertNode->setNext(moveNode->getNext());
	moveNode->getNext()->setPrevious(insertNode);
	moveNode->setNext(insertNode);
	insertNode->setPrevious(moveNode);
}

void LocalSearch::insertBefore(Node* insertNode, Node* moveNode) {
	//insert insertNode before moveNode
	insertNode->setPrevious(moveNode->getPrevious());
	moveNode->getPrevious()->setNext(insertNode);
	moveNode->setPrevious(insertNode);
	insertNode->setNext(moveNode);
}

void LocalSearch::insertBetween(Node* insertNodePrev, Node* insertNode,
		Node* insertNodeNext) {
	insertNodePrev->setNext(insertNode);
	insertNode->setPrevious(insertNodePrev);
	insertNode->setNext(insertNodeNext);
	insertNodeNext->setPrevious(insertNode);
}

bool LocalSearch::Insert()
{
#ifdef DEBUG_Insert
	cout << endl;
	cout << "local search insert initial sequence = " ; this->initSol->getSequence()->show() ; cout << endl;
#endif
	double initialObjVal = this->initSol->getObjVal();
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
				removeNode(insertNode);
#ifdef DEBUG_Insert
				cout << " show list after removing  " << insertNode->getClient()->getId() << " "  ;  this->initSol->getSequence()->show() ;
#endif
				// insert insertNode after moveNode if addAfter = true
				if(addAfter == true)
				{
					if(moveNode->getNext() == NULL)
					{
						//push back insertNode
						addTail(insertNode);
					}
					else
					{
						//insert insertNode after moveNode
						insertAfter(insertNode, moveNode);
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
						addHead(insertNode);
					}
					else
					{
						//insert insertNode before moveNode
						insertBefore(insertNode, moveNode);
					}
#ifdef DEBUG_Insert
					cout << " show list after inserting " << insertNode->getClient()->getId() << " before " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;
#endif
				}

			}

			// decode the list
			if(this->initSol->Decodage())
			{
#ifdef DEBUG_Insert
				this->initSol->CheckSolution();
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
				// remove insertNode at its current place
				removeNode(insertNode);
#ifdef DEBUG_Insert
				cout << " show list after removing " << insertNode->getClient()->getId() << " in its current place " ;  this->initSol->getSequence()->show() ;
#endif

				// insert insertNode in its initial place
				if(insertNodePrev == NULL)
				{
					addHead(insertNode);
				}
				else if( insertNodeNext == NULL)
				{
					addTail(insertNode);
				}
				else
				{
					insertBetween(insertNodePrev, insertNode, insertNodeNext);
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
#ifdef DEBUG_ArcInsert
		cout << "--------------------insertNode " << " " << " = " << insertNode->getClient()->getId() << " -----------------------" << endl;
#endif
		insertNodeFirst = insertNode;
		insertNodeLast = insertNodeFirst->getNext();

		if(insertNodeLast != NULL)
		{
#ifdef DEBUG_ArcInsert
			cout << " insert arc = " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << endl;
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
					removeNode(insertNodeFirst);
					removeNode(insertNodeLast);
#ifdef DEBUG_ArcInsert
					cout << " show list after removing  " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() ;  this->initSol->getSequence()->show() ;
#endif
					// insert insertNodeFirst and insertNodeLast after moveNode if addAfter = true
					if(addAfter == true)
					{
						if(moveNode->getNext() == NULL)
						{
							// add back insertNodeFirst
							addTail(insertNodeFirst);

							// add back insertNodeLast
							addTail(insertNodeLast);
						}
						else
						{
							// insert insertNodeFirst after moveNode
							insertAfter(insertNodeFirst, moveNode);

							// insert insertNodeLast after insertNodeFirst
							insertAfter(insertNodeLast, insertNodeFirst);
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
							addHead(insertNodeLast);

							// add front insertNodeFirst
							addHead(insertNodeFirst);
						}
						else
						{
							//insert insertNodeLast before moveNode
							insertBefore(insertNodeLast, moveNode);

							//insert insertNodeFirst before insertNodeLast
							insertBefore(insertNodeFirst, insertNodeLast);
						}
#ifdef DEBUG_ArcInsert
						cout << " show list after inserting " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId()  << " before " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show();
#endif
					}

				}

				// decode the list
				if(this->initSol->Decodage())
				{
#ifdef DEBUG_ArcInsert
					this->initSol->CheckSolution();
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
					// remove insertNodeFirst and insertNodeLast in their current place
					removeNode(insertNodeFirst);
					removeNode(insertNodeLast);
#ifdef DEBUG_ArcInsert
					cout << " show list after removing " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << " in their current place " ;  this->initSol->getSequence()->show() ;
#endif
					// insert insertNode in its initial place
					if(insertNodeFirstPrev == NULL)
					{
						// add insertNodeLast to the front
						addHead(insertNodeLast);

						// add insertNodeFirst to the front
						addHead(insertNodeFirst);
					}
					else if( insertNodeLastNext == NULL)
					{
						// push back insertNodeFirst
						addTail(insertNodeFirst);

						// push back insertNodeLast
						addTail(insertNodeLast);
					}
					else if(insertNodeFirstPrev != NULL && insertNodeLastNext != NULL)
					{
						// insert insertNodeFirst after insertNodeFirstPrev and before insertNodeLastNext
						insertBetween(insertNodeFirstPrev, insertNodeFirst, insertNodeLastNext);

						//insert insertNodeLast after insertNodeFirst and before insertNodeLastNext
						insertBetween(insertNodeFirst,insertNodeLast,insertNodeLastNext);
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
			removeNode(swapNodeFirst);
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
				removeNode(swapNodeSecond);
#ifdef DEBUG_Swap
				cout << " show list after removing  " << swapNodeSecond->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
			if(swapNodeFirstPrev == NULL)
			{// swap the two nodes : swapNodeFirst and swapNodeSecond

				// put swapNodeFirst in the place of swapNodeSecond then put swapNodeSecond in the place of swapNodeFirst
				if(sNodeSecondPrev == NULL)
				{
					//push front swapNodeFirst then swapNodeSecond
					addHead(swapNodeFirst);
					addHead(swapNodeSecond);
				}
				else if(sNodeSecondNext == NULL)
				{
					// push back swapNodeFirst then push front swapNodeSecond
					addTail(swapNodeFirst);
					addHead(swapNodeSecond);
				}
				else
				{
					// insert swapNodeFirst after sNodeSecondPrev and before sNodeSecondNext
					// then push front swapNodeSecond
					insertBetween(sNodeSecondPrev, swapNodeFirst, sNodeSecondNext);
					addHead(swapNodeSecond);
				}
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
					addTail(swapNodeFirst);
					if(swapNodeFirstNext == swapNodeSecond)
					{
						//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeFirst
						insertBetween(swapNodeFirstPrev,swapNodeSecond,swapNodeFirst);
					}
					else
					{
						//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeFirstNext
						insertBetween(swapNodeFirstPrev,swapNodeSecond,swapNodeFirstNext);
					}

				}
				else
				{
					//insert swapNodeFirst after  sNodeSecondPrev and before sNodeSecondNext
					insertBetween(sNodeSecondPrev,swapNodeFirst,sNodeSecondNext);
					if(swapNodeFirstNext == swapNodeSecond)
					{
						//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeFirst
						insertBetween(swapNodeFirstPrev,swapNodeSecond,swapNodeFirst);
					}
					else
					{
						//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeFirstNext
						insertBetween(swapNodeFirstPrev,swapNodeSecond,swapNodeFirstNext);
					}

				}
#ifdef DEBUG_Swap
				cout << " show list after swapping  " << swapNodeFirst->getClient()->getId() << " and " << swapNodeSecond->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
			}

			// decode the list
			if(this->initSol->Decodage())
			{
#ifdef DEBUG_Swap
				this->initSol->CheckSolution();
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
				// remove swapNodeFirst in its current place
				removeNode(swapNodeFirst);

#ifdef DEBUG_Swap
				cout << " show list after removing " << swapNodeFirst->getClient()->getId() <<  " in its current place " ;  this->initSol->getSequence()->show() ;
#endif
				// insert swapNodeFirst in its initial place
				if(swapNodeFirstPrev == NULL)
				{
					//push front swapNodeFirst
					addHead(swapNodeFirst);
				}
				else if(swapNodeFirstNext == NULL)
				{
					// push back swapNodeFirst
					addTail(swapNodeFirst);
				}
				else
				{
					// insert swapNodeFirst after swapNodeFirstPrev
					insertAfter(swapNodeFirst, swapNodeFirstPrev);
				}
#ifdef DEBUG_Swap
				cout << " show list after inserting " << swapNodeFirst->getClient()->getId() << " in its initial place " ;  this->initSol->getSequence()->show() ;
#endif
				// remove swapNodeSecond in its current place
				removeNode(swapNodeSecond);
#ifdef DEBUG_Swap
				cout << " show list after removing " <<  swapNodeSecond->getClient()->getId() << " in its current place " ;  this->initSol->getSequence()->show() ;
#endif
				// insert swapNodeSecond in its initial place
				if(swapNodeSecondNext == NULL)
				{
					// push back swapNodeSecond
					addTail(swapNodeSecond);
				}
				else
				{
					// insert swapNodeSecond after swapNodeSecondPrev and before swapNodeSecondNext
					insertBetween(swapNodeSecondPrev,swapNodeSecond,swapNodeSecondNext);
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
					removeNode(arcFirstNode);
					removeNode(arcLastNode);
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
					removeNode(swapNode);
#ifdef DEBUG_SwapArcs
					cout << " show list after removing  " << swapNode->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
					// swap the arc and the swapNode
					if(arcFirstNodePrev == NULL)
					{
						if(sNodePrev == NULL)
						{
							//push front arcLastNode then arcFirstNode then swapNode
							addHead(arcLastNode);
							addHead(arcFirstNode);
							addHead(swapNode); // not possible to put this instruction before if condition
						}
						else if(sNodeNext == NULL)
						{
							// push back arcFirstNode then arcLastNode then push front swapNode
							addTail(arcFirstNode);
							addTail(arcLastNode);
							addHead(swapNode);
						}
						else
						{
							// insert arcFirstNode after sNodePrev and before sNodeNext
							insertBetween(sNodePrev, arcFirstNode, sNodeNext);
							// insert arcLastNode after arcFirstNode and before sNodeNext
							insertBetween(arcFirstNode,arcLastNode, sNodeNext);
							// then push front swapNode
							addHead(swapNode);
						}
#ifdef DEBUG_SwapArcs
						cout << " show list after swapping  " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() << " and " << swapNode->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
					}
					else
					{
						if(sNodePrev == NULL)
						{
							// push front arcLastNode then arcFirstNode
							addHead(arcLastNode);
							addHead(arcFirstNode);
							if(arcFirstNodePrev == swapNode)
							{
								// insert swapNode after arcLastNode and before arcLastNodeNext
								insertBetween(arcLastNode,swapNode,arcLastNodeNext);
							}
							else if(arcLastNodeNext == NULL)
							{
								// push back swapNode
								addTail(swapNode);
							}
							else
							{
								//insert swapNode after  arcFirstNodePrev and before arcLastNodeNext
								insertBetween(arcFirstNodePrev,swapNode,arcLastNodeNext);
							}
						}
						else if(sNodeNext == NULL)
						{
							// push back arcFirstNode then arcLastNode
							addTail(arcFirstNode);
							addTail(arcLastNode);
							if(arcLastNodeNext == swapNode)
							{
								//insert swapNode after  arcFirstNodePrev and before arcFirstNode
								insertBetween(arcFirstNodePrev,swapNode,arcFirstNode);
							}
							else if(arcFirstNodePrev == swapNode)
							{
								//push back swapNode
								addTail(swapNode);
							}
							else
							{
								//insert swapNode after  arcFirstNodePrev and before arcLastNodeNext
								insertBetween(arcFirstNodePrev,swapNode,arcLastNodeNext);
							}

						}
						else
						{
							//insert arcFirstNode after  sNodePrev and before sNodeNext
							insertBetween(sNodePrev,arcFirstNode,sNodeNext);
							//insert arcLastNode after arcFirstNode and before sNodeNext
							insertBetween(arcFirstNode,arcLastNode,sNodeNext);
							if(arcFirstNodePrev == swapNode)
							{
								//insert swapNode after arcLastNode and before arcLastNodeNext
								insertBetween(arcLastNode,swapNode,arcLastNodeNext);

							}else if(arcLastNodeNext == swapNode)
							{
								//insert swapNode after  arcFirstNodePrev and before arcFirstNode
								insertBetween(arcFirstNodePrev,swapNode,arcFirstNode);
							}else if(arcLastNodeNext == NULL)
							{
								// push back swapNode
								addTail(swapNode);
							}
							else
							{
								//insert swapNode after  arcFirstNodePrev and before arcLastNodeNext
								insertBetween(arcFirstNodePrev,swapNode,arcLastNodeNext);
							}

						}
#ifdef DEBUG_SwapArcs
						cout << " show list after swapping  " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() << " and " << swapNode->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
					}

					// decode the list

					if(this->initSol->Decodage())
					{
#ifdef DEBUG_SwapArcs
						this->initSol->CheckSolution();
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
						// remove arcFirstNode and arcLastNode in their current place
						removeNode(arcFirstNode);
						removeNode(arcLastNode);

#ifdef DEBUG_SwapArcs
						cout << " show list after removing " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() <<  " in their current place " ;  this->initSol->getSequence()->show() ;
#endif
						// insert arcFirstNode and arcLastNode in their initial place
						if(arcFirstNodePrev == NULL)
						{
							//push front arcLastNode then arcFirstNode
							addHead(arcLastNode);
							addHead(arcFirstNode);
						}
						else if(arcLastNodeNext == NULL)
						{
							// push back arcFirstNode then arcLastNode
							addTail(arcFirstNode);
							addTail(arcLastNode);
						}
						else
						{
							// insert arcFirstNode after arcFirstNodePrev
							insertAfter(arcFirstNode,arcFirstNodePrev);
							// insert arcLastNode after arcFirstNode
							insertAfter(arcLastNode,arcFirstNode);
						}

#ifdef DEBUG_SwapArcs
						cout << " show list after inserting " << arcFirstNode->getClient()->getId() << "--" << arcLastNode->getClient()->getId() <<  " in their initial place " ;  this->initSol->getSequence()->show() ;
#endif
						// remove swapNode in its current place
						removeNode(swapNode);
#ifdef DEBUG_SwapArcs
						cout << " show list after removing " << swapNode->getClient()->getId() <<   " in its current place " ;  this->initSol->getSequence()->show() ;
#endif
						// insert swapNode in its initial place // to verify again
						if(swapNodePrev == NULL)
						{
							// push front swapNode
							addHead(swapNode);
						}
						else if(swapNodeNext == NULL)
						{
							// push back swapNode
							addTail(swapNode);
						}
						else
						{
							// insert swapNode after swapNodePrev and before swapNodeNext
							insertBetween(swapNodePrev,swapNode,swapNodeNext);
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
//	this->initSol->Decodage();
	double initialObjVal = this->initSol->getObjVal();
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
					removeNode(startArcFirst);
					removeNode(endArcFirst);
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
					removeNode(startArcSecond);
					removeNode(endArcSecond);
#ifdef DEBUG_SwapTwoArcs
					cout << " show list after removing  " << startArcSecond->getClient()->getId() << "--" << endArcSecond->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
					if(startArcFirstPrev == NULL)
					{ // swap the two arcs : first and second one

						if(sArcSecondPrev == NULL)
						{
							//push front endArcFirst then startArcFirst
							addHead(endArcFirst);
							addHead(startArcFirst);
							//push front endArcSecond then startArcSecond
							addHead(endArcSecond); // not possible to put this instruction before if condition
							addHead(startArcSecond);
						}
						else if(eArcSecondNext == NULL)
						{
							// push back startArcFirst then endArcFirst then push front swapNodeSecond
							addTail(startArcFirst);
							addTail(endArcFirst);
							// push front endArcSecond then startArcSecond
							addHead(endArcSecond);
							addHead(startArcSecond);
						}
						else
						{
							// insert startArcFirst after sArcSecondPrev and before eArcSecondNext
							insertBetween(sArcSecondPrev, startArcFirst, eArcSecondNext);
							// insert endArcFirst after startArcFirst and before eArcSecondNext
							insertBetween(startArcFirst,endArcFirst, eArcSecondNext);

							// push front endArcSecond then startArcSecond
							addHead(endArcSecond);
							addHead(startArcSecond);
						}
#ifdef DEBUG_SwapTwoArcs
						cout << " show list after swapping  " << startArcFirst->getClient()->getId() << "--" << endArcFirst->getClient()->getId() << " and " << startArcSecond->getClient()->getId() << "--" << endArcSecond->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
					}
					else
					{ // swap the two arcs : first and second one

						if(eArcSecondNext == NULL)
						{
							// push back startArcFirst then endArcFirst
							addTail(startArcFirst);
							addTail(endArcFirst);
							if(endArcFirstNext == startArcSecond)
							{
								//insert startArcSecond after  startArcFirstPrev and before startArcFirst
								insertBetween(startArcFirstPrev,startArcSecond,startArcFirst);
								// insert endArcSecond after startArcSecond and before startArcFirst
								insertBetween(startArcSecond,endArcSecond,startArcFirst);
							}
							else
							{
								//insert startArcSecond after  startArcFirstPrev and before endArcFirstNext
								insertBetween(startArcFirstPrev,startArcSecond,endArcFirstNext);
								// insert endArcSecond after startArcSecond and before endArcFirstNext
								insertBetween(startArcSecond,endArcSecond,endArcFirstNext);
							}

						}
						else
						{
							//insert startArcFirst after  sArcSecondPrev and before eArcSecondNext
							insertBetween(sArcSecondPrev,startArcFirst,eArcSecondNext);
							//insert endArcFirst after startArcFirst and before eArcSecondNext
							insertBetween(startArcFirst,endArcFirst,eArcSecondNext);

							if(endArcFirstNext == startArcSecond)
							{
								// insert startArcSecond after  startArcFirstPrev and before startArcFirst
								insertBetween(startArcFirstPrev,startArcSecond,startArcFirst);
								// insert endArcSecond after startArcSecond and before startArcFirst
								insertBetween(startArcSecond,endArcSecond,startArcFirst);
							}
							else
							{
								//insert startArcSecond after  startArcFirstPrev and before endArcFirstNext
								insertBetween(startArcFirstPrev,startArcSecond,endArcFirstNext);
								// insert endArcSecond after startArcSecond and before endArcFirstNext
								insertBetween(startArcSecond,endArcSecond,endArcFirstNext);
							}

						}
#ifdef DEBUG_SwapTwoArcs
						cout << " show list after swapping  " << startArcFirst->getClient()->getId() << "--" << endArcFirst->getClient()->getId() << " and " << startArcSecond->getClient()->getId() << "--" << endArcSecond->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
					}

					// decode the list
					if(this->initSol->Decodage())
					{
#ifdef DEBUG_SwapTwoArcs
						this->initSol->CheckSolution();
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
						// remove startArcFirst and endArcFirst in their current place
						removeNode(startArcFirst);
						removeNode(endArcFirst);

#ifdef DEBUG_SwapTwoArcs
						cout << " show list after removing " << startArcFirst->getClient()->getId() << "--" << endArcFirst->getClient()->getId() <<  " in their current place " ;  this->initSol->getSequence()->show() ;
#endif
						// insert startArcFirst and endArcFirst in their initial place
						if(startArcFirstPrev == NULL)
						{
							//push front endArcFirst then startArcFirst
							addHead(endArcFirst);
							addHead(startArcFirst);
						}
						else
						{
							// insert startArcFirst after startArcFirstPrev
							insertAfter(startArcFirst,startArcFirstPrev);
							// insert endArcFirst after startArcFirst
							insertAfter(endArcFirst,startArcFirst);
						}

#ifdef DEBUG_SwapTwoArcs
						cout << " show list after inserting " << startArcFirst->getClient()->getId() << "--" << endArcFirst->getClient()->getId() <<  " in their initial place " ;  this->initSol->getSequence()->show() ;
#endif
						// remove startArcSecond and endArcSecond in its current place
						removeNode(startArcSecond);
						removeNode(endArcSecond);
#ifdef DEBUG_SwapTwoArcs
						cout << " show list after removing " << startArcSecond->getClient()->getId() << "--" <<  endArcSecond->getClient()->getId() << " in its current place " ;  this->initSol->getSequence()->show() ;
#endif
						// insert startArcSecond and endArcSecond in thier initial place // to verify again
						if(endArcSecondNext == NULL)
						{
							// push back startArcSecond then endArcSecond
							addTail(startArcSecond);
							addTail(endArcSecond);
						}
						else
						{
							// insert startArcSecond after startArcSecondPrev and before endArcSecondNext
							insertBetween(startArcSecondPrev,startArcSecond,endArcSecondNext);
							// insert endArcSecond after startArcSecond and before endArcSecondNext
							insertBetween(startArcSecond,endArcSecond,endArcSecondNext);
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

