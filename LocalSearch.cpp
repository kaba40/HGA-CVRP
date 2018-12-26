/*
 * LocalSearch.cpp
 *
 *  Created on: 19 déc. 2018
 *      Author: kkeita
 */

#include "LocalSearch.hpp"

LocalSearch::LocalSearch(DataAP *data, DLinkedList *encod)
{
	this->encoding = encod;
	this->data_vrp = data;
}

LocalSearch::~LocalSearch() {
	// TODO Auto-generated destructor stub
}

DLinkedList* LocalSearch::Insert(DLinkedList *encod)
{
	DLinkedList *initEncode = encod;

	cout << "initEncode1 = " ; initEncode->show() ;
	Solution *initSol = new Solution(data_vrp, initEncode);
	if(initSol->Decodage())
	{
		initSol->CheckSolution();

		double obj = initSol->getObjVal();

		cout << "obj = " << obj << endl;

		int t = 0;
		for(Node *tmp = encod->getHead(); tmp != NULL; tmp = tmp->getNext())
		{
			Node *insertNode = initEncode->pop_position(t);


			cout << "insertNode " << t << " =" << insertNode->getClient()->getId() << endl;
			cout << "initEncode2  " << t << " =" ; initEncode->show() ;
			int i = 0;
			double obj2;

			do
			{
				initEncode->insert(i, insertNode);
				cout << "initEncode3  " << i << " =" ; initEncode->show() ;

				Solution *initSol2 = new Solution(data_vrp, initEncode);

				if(initSol2->Decodage())
				{
					initSol2->CheckSolution();
					obj2 = initSol2->getObjVal();
				}
				cout << "obj2 = " << obj2 << " obj = " << obj << endl;

				initEncode->pop_position(t);
				i++;

				cout << "encod->getSize() = " << encod->getSize() << endl;

			} while(obj2 >= obj && i <= encod->getSize());

			if(obj2 < obj - 0.0001)
				break;

			t++;
		}

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
