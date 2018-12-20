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
	Solution *initSol = new Solution(data_vrp, initEncode);
	if(initSol->Decodage())
	{
		initSol->CheckSolution();

		double obj = initSol->getObjVal();

		do
		{
			Node *firstNode = initEncode->pop_front(); //case 1
			initEncode->push_back(firstNode);

//			Node *lastNode = initEncode->pop_back(); same result as the first one
//			initEncode->push_front(lastNode);

//			Node *firstNode = initEncode->pop_front(); //case 2
//			for(int i = 0; i < initEncode->getSize(); i++)
//			{
//				initEncode->insert(i, firstNode);
//			}





		} while(obj > 100);
	}



}

DLinkedList* LocalSearch::TwoInsert(DLinkedList *encod)
{

}

DLinkedList* LocalSearch::Swap(DLinkedList *encod)
{

}

DLinkedList* LocalSearch::SwapArcs(DLinkedList *encod)
{

}

DLinkedList* LocalSearch::SwapTwoArcs(DLinkedList *encod)
{

}

DLinkedList* LocalSearch::LocalAlgo(DLinkedList *encod)
{

}
