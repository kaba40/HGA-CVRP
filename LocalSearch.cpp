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
		insertNode->setPrevious(NULL);
		insertNode->setNext(NULL);
	}
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
	cout << endl;
	cout << "local search insert initial sequence = " ; this->initSol->getSequence()->show() ; cout << endl;

	double initialObjVal = this->initSol->getObjVal();
	cout << "initialObjVal = " << initialObjVal << endl;
	double newObjVal;

	Node *insertNodePrev = NULL;
	Node *insertNodeNext = NULL;

	for(Node *insertNode = this->initSol->getSequence()->getHead(); insertNode != NULL; insertNode = insertNode->getNext())
	{
		cout << "--------------------insertNode " << " " << " = " << insertNode->getClient()->getId() << "-----------------------" << endl;

		insertNodePrev = insertNode->getPrevious();
		insertNodeNext = insertNode->getNext();

		if(insertNodePrev != NULL)
			cout << "insertNodePrev = " << insertNodePrev->getClient()->getId() << " ;" ;
		if(insertNodeNext != NULL)
			cout << " insertNodeNext = " << insertNodeNext->getClient()->getId() << endl;

		bool addAfter = false;

		for(Node *moveNode = this->initSol->getSequence()->getHead(); moveNode != NULL; moveNode = moveNode->getNext())
		{
			cout << "-------------------------- moveNode " << " " << " = " << moveNode->getClient()->getId() << "------------------- " << endl;

			// if insertNode == moveNode do nothing and addAfter = true
			if(insertNode == moveNode)
			{
				addAfter = true;
			}
			else
			{
				// remove insertNode from the list
				removeNode(insertNode);

				cout << " show list after removing  " << insertNode->getClient()->getId() << " "  ;  this->initSol->getSequence()->show() ;

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
					cout << " show list after inserting " << insertNode->getClient()->getId() << " after " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;
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
					cout << " show list after inserting " << insertNode->getClient()->getId() << " before " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;
				}

			}

			// decode the list

			if(this->initSol->Decodage())
			{
				this->initSol->CheckSolution();
				//this->initSol->PrintSolution();
				newObjVal = this->initSol->getObjVal();

			}

			cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();

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

				cout << " show list after removing " << insertNode->getClient()->getId() << " in its current place " ;  this->initSol->getSequence()->show() ;

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
				cout << " show list after inserting " << insertNode->getClient()->getId() << " in its initial place " ;  this->initSol->getSequence()->show() ;
			}
		}
		cout << "-------------------------------------------------------------------------------------------------" << endl;

	}

	return false;
}

bool LocalSearch::ArcInsert()
{
	cout << endl;
	cout << "local search ArcInsert initial sequence = " ; this->initSol->getSequence()->show() ;

	double initialObjVal = this->initSol->getObjVal();
	cout << "initialObjVal = " << initialObjVal << endl;
	double newObjVal;

	Node *insertNodeFirstPrev = NULL;
	Node *insertNodeLastNext = NULL;

	Node *insertNodeFirst = NULL;
	Node *insertNodeLast = NULL;

	for(Node *insertNode = this->initSol->getSequence()->getHead(); insertNode != NULL; insertNode = insertNode->getNext())
	{
		cout << "--------------------insertNode " << " " << " = " << insertNode->getClient()->getId() << "-----------------------" << endl;

		insertNodeFirst = insertNode;
		insertNodeLast = insertNodeFirst->getNext();

		if(insertNodeLast != NULL)
		{
			cout << " insertNodeFirst = " << insertNodeFirst->getClient()->getId() << endl;
			cout << " insertNodeLast = " << insertNodeLast->getClient()->getId() << endl;

			insertNodeFirstPrev = insertNode->getPrevious();
			insertNodeLastNext = insertNodeLast->getNext();

			if(insertNodeFirstPrev != NULL)
				cout << "insertNodeFirstPrev = " << insertNodeFirstPrev->getClient()->getId() << " ;" ;
			if(insertNodeLastNext != NULL)
				cout << " insertNodeLastNext = " << insertNodeLastNext->getClient()->getId() << endl;
			cout << endl;

			bool addAfter = false;

			for(Node *moveNode = this->initSol->getSequence()->getHead(); moveNode != NULL; moveNode = moveNode->getNext())
			{
				cout << "-------------------------- moveNode " << " " << " = " << moveNode->getClient()->getId() << "------------------- " << endl;

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

					cout << " show list after removing  " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() ;  this->initSol->getSequence()->show() ;

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
						cout << " show list after inserting " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << " after " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;
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
						cout << " show list after inserting " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId()  << " before " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;
					}

				}

				// decode the list

				if(this->initSol->Decodage())
				{
					this->initSol->CheckSolution();
					//					this->initSol->PrintSolution();
					newObjVal = this->initSol->getObjVal();

				}

				cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();

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

					cout << " show list after removing " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << " in their current place " ;  this->initSol->getSequence()->show() ;

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
					cout << " show list after inserting " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << " in their initial place " ;  this->initSol->getSequence()->show() ;
				}
			}
		}

		cout << "-------------------------------------------------------------------------------------------------" << endl;
	}

	return false;
}


bool LocalSearch::Swap()
{
	cout << endl;
	cout << "local search ArcInsert initial sequence = " ; this->initSol->getSequence()->show() ;

	double initialObjVal = this->initSol->getObjVal();
	cout << "initialObjVal = " << initialObjVal << endl;
	double newObjVal;

	Node *swapNodeFirstPrev = NULL;
	Node *swapNodeFirstNext = NULL;

	for(Node *swapNodeFirst = this->initSol->getSequence()->getHead(); swapNodeFirst != NULL; swapNodeFirst = swapNodeFirst->getNext())
	{
		cout << "--------------------swapNodeFirst " << " " << " = " << swapNodeFirst->getClient()->getId() << "-----------------------" << endl;

		swapNodeFirstPrev = swapNodeFirst->getPrevious();
		swapNodeFirstNext = swapNodeFirst->getNext();

		if(swapNodeFirstPrev != NULL)
			cout << "swapNodeFirstPrev = " << swapNodeFirstPrev->getClient()->getId() << " ;" ;
		if(swapNodeFirstNext != NULL)
			cout << " swapNodeFirstNext = " << swapNodeFirstNext->getClient()->getId() << endl;
		cout << endl;

		Node *swapNodeSecondPrev = NULL;
		Node *swapNodeSecondNext = NULL;

		for(Node *swapNodeSecond = this->initSol->getSequence()->getHead(); swapNodeSecond != NULL; swapNodeSecond = swapNodeSecond->getNext())
		{
			cout << "-------------------------- swapNodeSecond " << " " << " = " << swapNodeSecond->getClient()->getId() << "------------------- " << endl;

			swapNodeSecondPrev = swapNodeSecond->getPrevious();
			swapNodeSecondNext = swapNodeSecond->getNext();

			if(swapNodeSecondPrev != NULL)
				cout << "swapNodeSecondPrev = " << swapNodeSecondPrev->getClient()->getId() << " ;" ;
			if(swapNodeSecondNext != NULL)
				cout << " swapNodeSecondNext = " << swapNodeSecondNext->getClient()->getId() << endl;
			cout << endl;

			if(swapNodeSecond != swapNodeFirst)
			{
				//remove swapNodeFirst
				removeNode(swapNodeFirst);
				this->initSol->getSequence()->show();
				if(swapNodeFirstPrev == NULL)
				{
					Node *sNodeSecondPrev = swapNodeSecond->getPrevious();
					Node *SNodeSecondNext = swapNodeSecond->getNext();

					removeNode(swapNodeSecond);
					this->initSol->getSequence()->show();
					if(sNodeSecondPrev == NULL)
					{

						addHead(swapNodeFirst);
						this->initSol->getSequence()->show();
						addHead(swapNodeSecond);
						this->initSol->getSequence()->show();
//						exit(-1);
					}
					else if(SNodeSecondNext == NULL)
					{
						addTail(swapNodeFirst);
						this->initSol->getSequence()->show();
						addHead(swapNodeSecond);
						this->initSol->getSequence()->show();
//						exit(-1);
					}
					else
					{
						insertBetween(swapNodeSecondPrev, swapNodeFirst, swapNodeSecondNext);
						this->initSol->getSequence()->show();
						addHead(swapNodeSecond);
						this->initSol->getSequence()->show();
//						exit(-1);
					}
				}
				else // to finish
				{
					Node *sNodeSecondPrev = swapNodeSecond->getPrevious();
					Node *SNodeSecondNext = swapNodeSecond->getNext();

					removeNode(swapNodeSecond);
					this->initSol->getSequence()->show();
					exit(-1);
					if(sNodeSecondPrev == NULL)
					{
//						cout << "swapNodeSecond " << swapNodeSecond->getClient()->getId() << endl;

						exit(-1);
					}
					else if(SNodeSecondNext == NULL)
					{
						// 6 = swapNodeSecond, 7 = swapNodeFirst ?
						exit(-1);
					}
					else
					{

					}
				}

				// decode the list

				if(this->initSol->Decodage())
				{
					this->initSol->CheckSolution();
					//					this->initSol->PrintSolution();
					newObjVal = this->initSol->getObjVal();

				}

				cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();

				//	if improve show the new list and return
//				if(newObjVal < initialObjVal - 0.0001)
//				{
//					//this->initSol->getSequence()->show();
//					return true;
//				}
//				else
//				{
					// remove swapNodeFirst and swapNodeSecond at their current place
					removeNode(swapNodeFirst);
					removeNode(swapNodeSecond);

					cout << " show list after removing " << swapNodeFirst->getClient()->getId() << " and " << swapNodeSecond->getClient()->getId() << " in their current place " ;  this->initSol->getSequence()->show() ;

					if(swapNodeFirstPrev == NULL)
					{
						addHead(swapNodeFirst);
					}
					else if(swapNodeFirstNext == NULL)
					{
						addTail(swapNodeFirst);
					}
					else
					{
						insertBetween(swapNodeFirstPrev,swapNodeFirst,swapNodeFirstNext);
					}


					if(swapNodeSecondPrev == NULL)
					{
						addHead(swapNodeSecond);
					}
					else if(swapNodeSecondNext == NULL)
					{
						addTail(swapNodeSecond);
					}
					else
					{
						insertBetween(swapNodeSecondPrev,swapNodeSecond,swapNodeSecondNext);
					}

					cout << " show list after inserting " << swapNodeFirst->getClient()->getId() << " and " << swapNodeSecond->getClient()->getId() << " in their initial place " ;  this->initSol->getSequence()->show() ;

//				}
			}


		}
	}

 return false;
}

//bool LocalSearch::SwapArcs()
//{
//
//}
//
//bool LocalSearch::SwapTwoArcs()
//{
//
//}

