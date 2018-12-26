/*
 * LocalSearch.cpp
 *
 *  Created on: 19 déc. 2018
 *      Author: kkeita
 */

#include "LocalSearch.hpp"

LocalSearch::LocalSearch(Solution *initSol)
{
	this->encoding = initSol->getSequence();
}

LocalSearch::~LocalSearch() {
	// TODO Auto-generated destructor stub
}

DLinkedList* LocalSearch::Insert(DLinkedList *encod)
{
	DLinkedList *initEncode = encod;

	cout << "initEncode1 = " ; initEncode->show() ; cout << endl;

		int t1 = 0;
		for(Node *tmp1 = initEncode->getHead(); tmp1 != NULL; tmp1 = tmp1->getNext())
		{
			cout << "tmp1 " << t1 << " = " << tmp1->getClient()->getId() << endl;;

			int t2 = 0;
			for(Node *tmp2 = initEncode->getHead(); tmp2 != NULL; tmp2 = tmp2->getNext())
			{
				cout << "tmp2 " << t2 << " = " << tmp2->getClient()->getId() << endl;
				if(tmp1 == tmp2)
				{

				}
				else
				{
					tmp1->setNext(NULL);
					tmp1->setPrevious(NULL);

					tmp2->setNext(tmp1);
					tmp1->setPrevious(tmp2);
					tmp1->setNext(tmp2->getNext());
					tmp2->getNext()->setPrevious(tmp1);


				}

				cout << "initEncode2  " << " " << " =" ; encod->show() ; cout << endl;

				t2++;
			}
			t1++;
		}


	return initEncode;
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
