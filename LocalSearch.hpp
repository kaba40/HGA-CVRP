/*
 * LocalSearch.hpp
 *
 *  Created on: 19 déc. 2018
 *      Author: kkeita
 */

#ifndef LOCALSEARCH_HPP_
#define LOCALSEARCH_HPP_

#include<stdio.h>

#include "DLinkedList.hpp"
#include "DataAP.hpp"
#include "Solution.hpp"
class LocalSearch
{
private:
	DLinkedList *encoding;
public:
	LocalSearch(Solution *initSol); // solution seulement
	virtual ~LocalSearch();

	DLinkedList* Insert(DLinkedList *encod);
	DLinkedList* TwoInsert(DLinkedList *encod);
	DLinkedList* Swap(DLinkedList *encod);
	DLinkedList* SwapArcs(DLinkedList *encod);
	DLinkedList* SwapTwoArcs(DLinkedList *encod);
	DLinkedList* LocalAlgo(DLinkedList *encod);

};

#endif /* LOCALSEARCH_HPP_ */
