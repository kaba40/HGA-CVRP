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

	cout << "initEncode1 = " ; this->initSol->getSequence()->show() ; cout << endl;


		for(Node *insertNode = this->initSol->getSequence()->getHead(); insertNode != NULL; insertNode = insertNode->getNext())
		{
			cout << "insertNode " << " " << " = " << insertNode->getClient()->getId() ;

			bool addAfter = false;

			for(Node *moveNode = this->initSol->getSequence()->getHead(); moveNode != NULL; moveNode = moveNode->getNext())
			{
				cout << "moveNode " << " " << " = " << moveNode->getClient()->getId() ;

				// si tmp1 == tmp2 rien faire et addAfter = true
				// enlever tmp1 de la liste
				// inserer tmp1 apres tmp2 si addafter = true
				// inserer tmp1 avant tmp2 si addafter = false
				// evaluer la liste
				//	si improve affiche new list and return
				//	si non improve execute line suivante
				// inserer tmp1 à sa place initiale en l'enlevant de sa place actuelle

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
