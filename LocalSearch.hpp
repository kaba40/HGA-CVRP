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
#include "Solution.hpp"
class LocalSearch
{
private:
	Solution *initSol;

	void removeInsertNode(Node* insertNode);

public:
	LocalSearch(Solution *initSol); // solution seulement
	virtual ~LocalSearch();

	bool Insert();
	bool TwoInsert();
	bool Swap();
	bool SwapArcs();
	bool SwapTwoArcs();
};

#endif /* LOCALSEARCH_HPP_ */
