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

bool LocalSearch::Insert()
{

	cout << "local search insert initial sequence = " ; this->initSol->getSequence()->show() ; cout << endl;

		double initialObjVal = this->initSol->getObjVal();
		cout << "initialObjVal = " << initialObjVal << endl;
		double newObjVal;

		Node *insertNodePrev = NULL;
		Node *insertNodeNext = NULL;

		for(Node *insertNode = this->initSol->getSequence()->getHead(); insertNode != NULL; insertNode = insertNode->getNext())
		{
			cout << "insertNode " << " " << " = " << insertNode->getClient()->getId() << ";";

			insertNodePrev = insertNode->getPrevious();
			insertNodeNext = insertNode->getNext();

			if(insertNodePrev != NULL)
				cout << "insertNodePrev = " << insertNodePrev->getClient()->getId() << " ;" ;
			if(insertNodeNext != NULL)
				cout << " insertNodeNext = " << insertNodeNext->getClient()->getId() << endl;

			bool addAfter = false;

			for(Node *moveNode = this->initSol->getSequence()->getHead(); moveNode != NULL; moveNode = moveNode->getNext())
			{
				cout << " moveNode " << " " << " = " << moveNode->getClient()->getId() << endl;

				// si tmp1 == tmp2 rien faire et addAfter = true
				if(insertNode == moveNode)
				{
					addAfter = true;
				}
				else
				{
					// enlever tmp1 de la liste
					if(insertNode == this->initSol->getSequence()->getHead())
					{
						this->initSol->getSequence()->setHead(insertNode->getNext());
						this->initSol->getSequence()->getHead()->setPrevious(NULL);
					}
					else if(insertNode == this->initSol->getSequence()->getTail())
					{
						this->initSol->getSequence()->setTail(insertNode->getPrevious());
						this->initSol->getSequence()->getTail()->setNext(NULL);
					}
					else
					{
						insertNode->getPrevious()->setNext(insertNode->getNext());
						insertNode->getNext()->setPrevious(insertNode->getPrevious());
						insertNode->setPrevious(NULL);
						insertNode->setNext(NULL);

					}

					cout << " show list after removing  " << insertNode->getClient()->getId() << " "  ;  this->initSol->getSequence()->show() ;

					// insert insertNode after moveNode if addAfter = true
					if(addAfter == true)
					{
						if(moveNode->getNext() == NULL)
						{
							insertNode->setPrevious(this->initSol->getSequence()->getTail());
							this->initSol->getSequence()->getTail()->setNext(insertNode);
							this->initSol->getSequence()->setTail(insertNode);
							this->initSol->getSequence()->getTail()->setNext(NULL);


						}
						else
						{
							insertNode->setNext(moveNode->getNext());
							moveNode->getNext()->setPrevious(insertNode);
							moveNode->setNext(insertNode);
							insertNode->setPrevious(moveNode);
						}

						cout << " show list after inserting " << insertNode->getClient()->getId() << " after " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;

					}
					else // insert insertNode before moveNode if addAfter = false
					{
						if(moveNode->getPrevious() == NULL)
						{
					    	insertNode->setNext(this->initSol->getSequence()->getHead());
					    	this->initSol->getSequence()->getHead()->setPrevious(insertNode);
					    	this->initSol->getSequence()->setHead(insertNode);
					    	this->initSol->getSequence()->getHead()->setPrevious(NULL);
						}
						else
						{
							insertNode->setPrevious(moveNode->getPrevious());
							moveNode->getPrevious()->setNext(insertNode);
							moveNode->setPrevious(insertNode);
							insertNode->setNext(moveNode);
						}

						cout << " show list after inserting " << insertNode->getClient()->getId() << " before " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;

					}

				}




				// evaluer la liste

				if(this->initSol->Decodage())
				{
					this->initSol->CheckSolution();
//					this->initSol->PrintSolution();
					newObjVal = this->initSol->getObjVal();

				}

				cout << " objVal = " << this->initSol->getObjVal() << " "; this->initSol->getSequence()->show();

				//	si improve affiche new list and return
				if(newObjVal < initialObjVal - 0.0001)
				{
//					this->initSol->getSequence()->show();
					return true;
				}
				else
				{
					// enlever insertNode à sa place actuelle
					if(insertNode == this->initSol->getSequence()->getHead())
					{
						this->initSol->getSequence()->setHead(insertNode->getNext());
						this->initSol->getSequence()->getHead()->setPrevious(NULL);
					}
					else if(insertNode == this->initSol->getSequence()->getTail())
					{
						this->initSol->getSequence()->setTail(insertNode->getPrevious());
						this->initSol->getSequence()->getTail()->setNext(NULL);
					}
					else
					{
						insertNode->getPrevious()->setNext(insertNode->getNext());
						insertNode->getNext()->setPrevious(insertNode->getPrevious());
						insertNode->setPrevious(NULL);
						insertNode->setNext(NULL);
					}

					// inserer insertNode à sa place initiale
					if(insertNodePrev == NULL)
					{
				    	insertNode->setNext(this->initSol->getSequence()->getHead());
				    	this->initSol->getSequence()->getHead()->setPrevious(insertNode);
				    	this->initSol->getSequence()->setHead(insertNode);
				    	this->initSol->getSequence()->getHead()->setPrevious(NULL);
					}
					else if( insertNodeNext == NULL)
					{
						insertNode->setPrevious(this->initSol->getSequence()->getTail());
						this->initSol->getSequence()->getTail()->setNext(insertNode);
						this->initSol->getSequence()->setTail(insertNode);
						this->initSol->getSequence()->getTail()->setNext(NULL);
					}
					else
					{
						insertNodePrev->setNext(insertNode);
						insertNode->setPrevious(insertNodePrev);
						insertNode->setNext(insertNodeNext);
						insertNode->setPrevious(insertNode);
					}

				}

			}

		}

	return false;
}

//DLinkedList* LocalSearch::TwoInsert(DLinkedList *encod)
//{
//
//}
//
//DLinkedList* LocalSearch::Swap(DLinkedList *encod)
//{
//
//}
//
//DLinkedList* LocalSearch::SwapArcs(DLinkedList *encod)
//{
//
//}
//
//DLinkedList* LocalSearch::SwapTwoArcs(DLinkedList *encod)
//{
//
//}
//
//DLinkedList* LocalSearch::LocalAlgo(DLinkedList *encod)
//{
//
//}
