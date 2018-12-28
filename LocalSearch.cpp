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


		for(Node *insertNode = this->initSol->getSequence()->getHead(); insertNode != NULL; insertNode = insertNode->getNext())
		{
			cout << "insertNode " << " " << " = " << insertNode->getClient()->getId() << ";";

			bool addAfter = false;
			int initPosition ;

			for(Node *moveNode = this->initSol->getSequence()->getHead(); moveNode != NULL; moveNode = moveNode->getNext())
			{
				cout << " moveNode " << " " << " = " << moveNode->getClient()->getId() ;

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
						initPosition = 1;
					}
					else if(insertNode == this->initSol->getSequence()->getTail())
					{
						this->initSol->getSequence()->setTail(insertNode->getPrevious());
						this->initSol->getSequence()->getTail()->setNext(NULL);
						initPosition = 2;
					}
					else
					{
						insertNode->getPrevious()->setNext(insertNode->getNext());
						insertNode->getNext()->setPrevious(insertNode->getPrevious());
						insertNode->setPrevious(NULL);
						insertNode->setNext(NULL);
						initPosition = 3;

					}

					// inserer tmp1 apres tmp2 si addafter = true
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

					}
					else // inserer tmp1 avant tmp2 si addafter = false
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
					}

				}


				this->initSol->getSequence()->show();

				// evaluer la liste

				if(this->initSol->Decodage())
				{
					this->initSol->CheckSolution();
					this->initSol->PrintSolution();
				}

//				exit(-1);




				//	si improve affiche new list and return
				//	si non improve execute line suivante
				// inserer tmp1 à sa place initiale en l'enlevant de sa place actuelle
//				switch(initPosition)
//				{
//
//				case 1:
//				{
//			    	insertNode->setNext(this->initSol->getSequence()->getHead());
//			    	this->initSol->getSequence()->getHead()->setPrevious(insertNode);
//			    	this->initSol->getSequence()->setHead(insertNode);
//			    	this->initSol->getSequence()->getHead()->setPrevious(NULL);
//					break;
//				};
//
//				case 2:
//				{
//					insertNode->setPrevious(this->initSol->getSequence()->getTail());
//					this->initSol->getSequence()->getTail()->setNext(insertNode);
//					this->initSol->getSequence()->setTail(insertNode);
//					this->initSol->getSequence()->getTail()->setNext(NULL);
//					break;
//				};
//
//				case 3:
//				{
//					break;
//				};
//
//				default:
//					cout << " bad value for initPosition " << endl;
//					break;
//				}

				this->initSol->getSequence()->show();

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
