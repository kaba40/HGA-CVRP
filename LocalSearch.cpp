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
		cout << "--------------------insertNode " << " " << " = " << insertNode->getClient()->getId() << "-----------------------" << endl;
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
			cout << "-------------------------- moveNode " << " " << " = " << moveNode->getClient()->getId() << "------------------- " << endl;
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
				//this->initSol->PrintSolution();
#endif
				newObjVal = this->initSol->getObjVal();

			}
#ifdef DEBUG_Insert
			cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();
#endif

			//	if improve show the new list and return
			if(newObjVal < initialObjVal - 0.0001)
			{
				//this->initSol->getSequence()->show();
				return true;
			}
			else
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
		cout << "--------------------insertNode " << " " << " = " << insertNode->getClient()->getId() << "-----------------------" << endl;
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
				cout << "-------------------------- moveNode " << " " << " = " << moveNode->getClient()->getId() << "------------------- " << endl;
#endif
				// if insertNodeFirst == moveNode ou insertNodeLast == moveNode do nothing and addAfter = true
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
					this->initSol->CheckSolution();
					//					this->initSol->PrintSolution();
					newObjVal = this->initSol->getObjVal();

				}
#ifdef DEBUG_ArcInsert
				cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();
#endif
				//	if improve show the new list and return
				if(newObjVal < initialObjVal - 0.0001)
				{
					//this->initSol->getSequence()->show();
					return true;
				}
				else
				{
					// remove insertNodeFirst and insertNodeLast at their current place
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
		cout << "--------------------swapNodeFirst " << " " << " = " << swapNodeFirst->getClient()->getId() << "-----------------------" << endl;
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
			cout << "-------------------------- swapNodeSecond " << " " << " = " << swapNodeSecond->getClient()->getId() << "------------------- " << endl;
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
			Node *sNodeSecondPrev = NULL;
			Node *SNodeSecondNext = NULL;
			if(swapNodeFirstPrev == NULL)
			{
				sNodeSecondPrev = swapNodeSecond->getPrevious();
				SNodeSecondNext = swapNodeSecond->getNext();

				removeNode(swapNodeSecond);
#ifdef DEBUG_Swap
				cout << " show list after removing  " << swapNodeSecond->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
				if(sNodeSecondPrev == NULL)
				{
					//push front swapNodeFirst then swapNodeSecond
					addHead(swapNodeFirst);
					addHead(swapNodeSecond);
				}
				else if(SNodeSecondNext == NULL)
				{
					// push back swapNodeFirst then push front swapNodeSecond
					addTail(swapNodeFirst);
					addHead(swapNodeSecond);
				}
				else
				{
					// insert swapNodeFirst after swapNodeSecondPrev and before swapNodeSecondNext
					// then push front swapNodeSecond
					insertBetween(swapNodeSecondPrev, swapNodeFirst, swapNodeSecondNext);
					addHead(swapNodeSecond);
				}
#ifdef DEBUG_Swap
				cout << " show list after swapping  " << swapNodeFirst->getClient()->getId() << " and " << swapNodeSecond->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
			}
			else
			{
				sNodeSecondPrev = swapNodeSecond->getPrevious();
				SNodeSecondNext = swapNodeSecond->getNext();
#ifdef DEBUG_Swap
				if(sNodeSecondPrev != NULL)
					cout << "sNodeSecondPrev = " << sNodeSecondPrev->getClient()->getId() << " ;" ;
				if(SNodeSecondNext != NULL)
					cout << " SNodeSecondNext = " << SNodeSecondNext->getClient()->getId() << endl;
				cout << endl;
#endif
				removeNode(swapNodeSecond);
#ifdef DEBUG_Swap
				cout << " show list after removing  " << swapNodeSecond->getClient()->getId() << " "  ;this->initSol->getSequence()->show();
#endif

				if(SNodeSecondNext == NULL)
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
					//insert swapNodeFirst after  sNodeSecondPrev and before SNodeSecondNext
					insertBetween(sNodeSecondPrev,swapNodeFirst,SNodeSecondNext);
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
				this->initSol->CheckSolution();
				//					this->initSol->PrintSolution();
				newObjVal = this->initSol->getObjVal();

			}
#ifdef DEBUG_Swap
			cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();
#endif
			//	if improve show the new list and return
			if(newObjVal < initialObjVal - 0.0001)
			{
				//this->initSol->getSequence()->show();
				return true;
			}
			else // to verify
			{
				// remove swapNodeFirst and swapNodeSecond at their current place
				removeNode(swapNodeFirst);
//				removeNode(swapNodeSecond);
//#ifdef DEBUG_Swap
//				cout << " show list after removing " << swapNodeFirst->getClient()->getId() << " and " << swapNodeSecond->getClient()->getId() << " in their current place " ;  this->initSol->getSequence()->show() ;
//#endif
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
				// remove swapNodeSecond at its current place
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

	Node *swapNodeFirst = NULL;
	Node *swapNodeLast = NULL;

	Node *swapNodeFirstPrev = NULL;
	Node *swapNodeLastNext = NULL;

	for(Node *swapNode = this->initSol->getSequence()->getHead(); swapNode != NULL; swapNode = swapNode->getNext())
	{
		cout << endl;
#ifdef DEBUG_SwapArcs
		cout << "-------------------------------------------------------------------------------------------------" << endl;
#endif
		swapNodeFirst = swapNode;
		swapNodeLast = swapNodeFirst->getNext();
		if(swapNodeLast != NULL)
		{
#ifdef DEBUG_SwapArcs
			cout << "swapping arc = " << swapNodeFirst->getClient()->getId() << "--" << swapNodeLast->getClient()->getId() << endl;
#endif
			swapNodeFirstPrev = swapNodeFirst->getPrevious();
			swapNodeLastNext = swapNodeLast->getNext();
#ifdef DEBUG_SwapArcs
			if(swapNodeFirstPrev != NULL)
				cout << "swapNodeFirstPrev = " << swapNodeFirstPrev->getClient()->getId() << " ;" ;
			if(swapNodeLastNext != NULL)
				cout << " swapNodeLastNext = " << swapNodeLastNext->getClient()->getId() << endl;
			cout << endl;
#endif
			Node *swapNodeSecondPrev = NULL;
			Node *swapNodeSecondNext = NULL;

			for(Node *swapNodeSecond = swapNodeLast->getNext(); swapNodeSecond != NULL; swapNodeSecond = swapNodeSecond->getNext())
			{
#ifdef DEBUG_SwapArcs
				cout << "-------------------------- swapNodeSecond " << " " << " = " << swapNodeSecond->getClient()->getId() << "------------------- " << endl;
#endif
				swapNodeSecondPrev = swapNodeSecond->getPrevious();
				swapNodeSecondNext = swapNodeSecond->getNext();
#ifdef DEBUG_SwapArcs
				if(swapNodeSecondPrev != NULL)
					cout << "swapNodeSecondPrev = " << swapNodeSecondPrev->getClient()->getId() << " ;" ;
				if(swapNodeSecondNext != NULL)
					cout << " swapNodeSecondNext = " << swapNodeSecondNext->getClient()->getId() << endl;
				cout << endl;
#endif
				//remove swapNodeFirst and swapNodeLast
				removeNode(swapNodeFirst);
				removeNode(swapNodeLast);
#ifdef DEBUG_SwapArcs
				cout << " show list after removing  " << swapNodeFirst->getClient()->getId() << "--" << swapNodeLast->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
				Node *sNodeSecondPrev = NULL;
				Node *SNodeSecondNext = NULL;
				if(swapNodeFirstPrev == NULL)
				{
					sNodeSecondPrev = swapNodeSecond->getPrevious();
					SNodeSecondNext = swapNodeSecond->getNext();
					//remove swapNodeSecond
					removeNode(swapNodeSecond);
#ifdef DEBUG_SwapArcs
					cout << " show list after removing  " << swapNodeSecond->getClient()->getId() << " "  ; this->initSol->getSequence()->show();
#endif
					if(sNodeSecondPrev == NULL)
					{
						//push front swapNodeLast then swapNodeFirst then swapNodeSecond
						addHead(swapNodeLast);
						addHead(swapNodeFirst);
						addHead(swapNodeSecond); // not possible to put this instruction before if condition
					}
					else if(SNodeSecondNext == NULL)
					{
						// push back swapNodeFirst then swapNodeLast then push front swapNodeSecond
						addTail(swapNodeFirst);
						addTail(swapNodeLast);
						addHead(swapNodeSecond);
					}
					else
					{
						// insert swapNodeFirst after swapNodeSecondPrev and before swapNodeSecondNext
						insertBetween(swapNodeSecondPrev, swapNodeFirst, swapNodeSecondNext);
						// insert swapNodeLast after swapNodeFirst and before swapNodeSecondNext
						insertBetween(swapNodeFirst,swapNodeLast, swapNodeSecondNext);
						// then push front swapNodeSecond
						addHead(swapNodeSecond);
					}
#ifdef DEBUG_SwapArcs
					cout << " show list after swapping  " << swapNodeFirst->getClient()->getId() << "--" << swapNodeLast->getClient()->getId() << " and " << swapNodeSecond->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
				}
				else
				{
					sNodeSecondPrev = swapNodeSecond->getPrevious();
					SNodeSecondNext = swapNodeSecond->getNext();
#ifdef DEBUG_SwapArcs
					if(sNodeSecondPrev != NULL)
						cout << "sNodeSecondPrev = " << sNodeSecondPrev->getClient()->getId() << " ;" ;
					if(SNodeSecondNext != NULL)
						cout << " SNodeSecondNext = " << SNodeSecondNext->getClient()->getId() << endl;
					cout << endl;
#endif
					// remove swapNodeSecond
					removeNode(swapNodeSecond);
#ifdef DEBUG_SwapArcs
					cout << " show list after removing  " << swapNodeSecond->getClient()->getId() << " "  ;this->initSol->getSequence()->show();
#endif

					if(SNodeSecondNext == NULL)
					{
						// push back swapNodeFirst then swapNodeLast
						addTail(swapNodeFirst);
						addTail(swapNodeLast);
						if(swapNodeLastNext == swapNodeSecond)
						{
							//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeFirst
							insertBetween(swapNodeFirstPrev,swapNodeSecond,swapNodeFirst);
						}
						else
						{
							//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeLastNext
							insertBetween(swapNodeFirstPrev,swapNodeSecond,swapNodeLastNext);
							//insert swapNodeSecond after  swapNodeFirstPrev
							//						insertAfter(swapNodeSecond, swapNodeFirstPrev);
						}

					}
					else
					{
						//insert swapNodeFirst after  sNodeSecondPrev and before SNodeSecondNext
						insertBetween(sNodeSecondPrev,swapNodeFirst,SNodeSecondNext);
						//insert swapNodeLast after swapNodeFirst and before SNodeSecondNext
						insertBetween(swapNodeFirst,swapNodeLast,SNodeSecondNext);
						if(swapNodeLastNext == swapNodeSecond)
						{
							//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeFirst
							insertBetween(swapNodeFirstPrev,swapNodeSecond,swapNodeFirst);
						}
						else
						{
							//insert swapNodeSecond after  swapNodeFirstPrev and before swapNodeLastNext
							insertBetween(swapNodeFirstPrev,swapNodeSecond,swapNodeLastNext);
						}

					}
#ifdef DEBUG_SwapArcs
					cout << " show list after swapping  " << swapNodeFirst->getClient()->getId() << "--" << swapNodeLast->getClient()->getId() << " and " << swapNodeSecond->getClient()->getId() << " " ; this->initSol->getSequence()->show();
#endif
				}

				// decode the list

				if(this->initSol->Decodage())
				{
					this->initSol->CheckSolution();
					//					this->initSol->PrintSolution();
					newObjVal = this->initSol->getObjVal();

				}
#ifdef DEBUG_SwapArcs
				cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();
#endif
				//	if improve show the new list and return
				//			if(newObjVal < initialObjVal - 0.0001)
				//			{
				//				//this->initSol->getSequence()->show();
				//				return true;
				//			}
				//			else // to verify
				//			{
				// remove swapNodeFirst, swapNodeLast and swapNodeSecond at their current place
				removeNode(swapNodeFirst);
				removeNode(swapNodeLast);
//				removeNode(swapNodeSecond);
#ifdef DEBUG_SwapArcs
				cout << " show list after removing " << swapNodeFirst->getClient()->getId() << "--" << swapNodeLast->getClient()->getId() <<  " in their current place " ;  this->initSol->getSequence()->show() ;
#endif
				// insert swapNodeFirst and swapNodeLast in their initial place
				if(swapNodeFirstPrev == NULL)
				{
					//push front swapNodeLast then swapNodeFirst
					addHead(swapNodeLast);
					addHead(swapNodeFirst);
				}
				else if(swapNodeLastNext == NULL)
				{
					// push back swapNodeFirst then swapNodeLast
					addTail(swapNodeFirst);
					addTail(swapNodeLast);
				}
				else
				{
//					// insert swapNodeFirst after swapNodeFirstPrev and before swapNodeLastNext
//					insertBetween(swapNodeFirstPrev,swapNodeFirst,swapNodeLastNext);
//					// insert swapNodeLast after swapNodeFirst and before swapNodeLastNext
//					insertBetween(swapNodeFirst, swapNodeLast, swapNodeLastNext);
					// insert swapNodeFirst after swapNodeFirstPrev
					insertAfter(swapNodeFirst,swapNodeFirstPrev);
					// insert swapNodeLast after swapNodeFirst
					insertAfter(swapNodeLast,swapNodeFirst);

				}

#ifdef DEBUG_SwapArcs
				cout << " show list after inserting " << swapNodeFirst->getClient()->getId() << "--" << swapNodeLast->getClient()->getId() <<  " in their initial place " ;  this->initSol->getSequence()->show() ;
#endif
				removeNode(swapNodeSecond);
#ifdef DEBUG_SwapArcs
				cout << " show list after removing " << swapNodeSecond->getClient()->getId() <<   " in its current place " ;  this->initSol->getSequence()->show() ;
#endif
				// insert swapNodeSecond in its initial place // to verify again
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
#ifdef DEBUG_SwapArcs
				cout << " show list after inserting " <<  swapNodeSecond->getClient()->getId() << " in its initial place " ;  this->initSol->getSequence()->show() ;
#endif
				//			}

			}
		}

#ifdef DEBUG_SwapArcs
		cout << "-------------------------------------------------------------------------------------------------" << endl;
#endif
	}

	return false;
}

//bool LocalSearch::SwapTwoArcs()
//{
//
//}

