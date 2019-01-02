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

void LocalSearch::removeInsertNode(Node* insertNode) {
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
				removeInsertNode(insertNode);

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
//					this->initSol->getSequence()->show();
					return true;
				}
				else
				{
					// remove insertNode at its current place
					removeInsertNode(insertNode);

					cout << " show list after removing " << insertNode->getClient()->getId() << " in its current place " ;  this->initSol->getSequence()->show() ;

					// insert insertNode in its initial place
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
						insertNodeNext->setPrevious(insertNode);
					}

					cout << " show list after inserting " << insertNode->getClient()->getId() << " in its initial place " ;  this->initSol->getSequence()->show() ;

				}
cout << "-------------------------------------------------------------------------------------------------" << endl;
			}
cout << "-------------------------------------------------------------------------------------------------" << endl;

		}

	return false;
}

bool LocalSearch::TwoInsert()
{
	cout << "local search insert initial sequence = " ; this->initSol->getSequence()->show() ; cout << endl;

	double initialObjVal = this->initSol->getObjVal();
	cout << "initialObjVal = " << initialObjVal << endl;
	double newObjVal;

	Node *insertNodePrev = NULL;
	Node *insertNodeNext = NULL;

	Node *insertNodeFirst = NULL;
	Node *insertNodeLast = NULL;

	for(Node *insertNode = this->initSol->getSequence()->getHead(); insertNode != NULL; insertNode = insertNode->getNext())
	{
		cout << "--------------------insertNode " << " " << " = " << insertNode->getClient()->getId() << "-----------------------" << endl;

		insertNodePrev = insertNode->getPrevious();
		insertNodeNext = insertNode->getNext();

		if(insertNodePrev != NULL)
			cout << "insertNodePrev = " << insertNodePrev->getClient()->getId() << " ;" ;
		if(insertNodeNext != NULL)
			cout << " insertNodeNext = " << insertNodeNext->getClient()->getId() << endl;

		insertNodeFirst = insertNode;
		insertNodeLast = insertNodeFirst->getNext();

		if(insertNodeLast != NULL)
		{
			cout << " insertNodeFirst = " << insertNodeFirst->getClient()->getId() << endl;
			cout << " insertNodeLast = " << insertNodeLast->getClient()->getId() << endl;


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
					removeInsertNode(insertNodeFirst);
					removeInsertNode(insertNodeLast);

					cout << " show list after removing  " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() ;  this->initSol->getSequence()->show() ;

					// insert insertNodeFirst and insertNodeLast after moveNode if addAfter = true
					if(addAfter == true)
					{
						if(moveNode->getNext() == NULL)
						{
							insertNodeFirst->setPrevious(this->initSol->getSequence()->getTail());
							this->initSol->getSequence()->getTail()->setNext(insertNodeFirst);
							this->initSol->getSequence()->setTail(insertNodeFirst);
							this->initSol->getSequence()->getTail()->setNext(NULL);

							insertNodeLast->setPrevious(this->initSol->getSequence()->getTail());
							this->initSol->getSequence()->getTail()->setNext(insertNodeLast);
							this->initSol->getSequence()->setTail(insertNodeLast);
							this->initSol->getSequence()->getTail()->setNext(NULL);


						}
						else
						{
							insertNodeFirst->setNext(moveNode->getNext());
							moveNode->getNext()->setPrevious(insertNodeFirst);
							moveNode->setNext(insertNodeFirst);
							insertNodeFirst->setPrevious(moveNode);

							insertNodeLast->setNext(insertNodeFirst->getNext());
							insertNodeFirst->getNext()->setPrevious(insertNodeLast);
							insertNodeFirst->setNext(insertNodeLast);
							insertNodeLast->setPrevious(insertNodeFirst);
						}

						cout << " show list after inserting " << insertNodeFirst->getClient()->getId() << "--" << insertNodeLast->getClient()->getId() << " after " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;

					}
					else // insert insertNodeFirst and insertNodeLast before moveNode if addAfter = false
					{
						if(moveNode->getPrevious() == NULL)
						{
							insertNodeLast->setNext(this->initSol->getSequence()->getHead());
							this->initSol->getSequence()->getHead()->setPrevious(insertNodeLast);
							this->initSol->getSequence()->setHead(insertNodeLast);
							this->initSol->getSequence()->getHead()->setPrevious(NULL);

							insertNodeFirst->setNext(this->initSol->getSequence()->getHead());
							this->initSol->getSequence()->getHead()->setPrevious(insertNodeFirst);
							this->initSol->getSequence()->setHead(insertNodeFirst);
							this->initSol->getSequence()->getHead()->setPrevious(NULL);
						}
						else
						{
							insertNodeLast->setPrevious(moveNode->getPrevious());
							moveNode->getPrevious()->setNext(insertNodeLast);
							moveNode->setPrevious(insertNodeLast);
							insertNodeLast->setNext(moveNode);

							insertNodeFirst->setPrevious(insertNodeLast->getPrevious());
							insertNodeLast->getPrevious()->setNext(insertNodeFirst);
							insertNodeLast->setPrevious(insertNodeFirst);
							insertNodeFirst->setNext(insertNodeLast);
						}

						cout << " show list after inserting " << insertNode->getClient()->getId() << " before " << moveNode->getClient()->getId()  ;  this->initSol->getSequence()->show() ;

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
					//					this->initSol->getSequence()->show();
					return true;
				}
				else
				{
					// remove insertNodeFirst and insertNodeLast at their current place
					removeInsertNode(insertNodeFirst);
					removeInsertNode(insertNodeLast);


					///////////////////////////////////to do tomorrow /////////////////////////////////

					cout << " show list after removing " << insertNode->getClient()->getId() << " in its current place " ;  this->initSol->getSequence()->show() ;
					exit(-1);
					// insert insertNode in its initial place
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
						insertNodeNext->setPrevious(insertNode);
					}
					////////////////////////////////////////////////////////////////////////////////////
					cout << " show list after inserting " << insertNode->getClient()->getId() << " in its initial place " ;  this->initSol->getSequence()->show() ;


				}
				cout << "-------------------------------------------------------------------------------------------------" << endl;
			}
		}

		cout << "-------------------------------------------------------------------------------------------------" << endl;
	}


	return false;
}


//bool LocalSearch::Swap()
//{
//
//}
//
//bool LocalSearch::SwapArcs()
//{
//
//}
//
//bool LocalSearch::SwapTwoArcs()
//{
//
//}

