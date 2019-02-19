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
#include "SeqData.hpp"
#include "Solution.hpp"
class LocalSearch
{
private:
	Solution *initSol;

public:
	LocalSearch(Solution *initSol, bool direct = true); // solution to modify if improved
	virtual ~LocalSearch();

	// movements for indirect encoding, i.e, giant tour
	bool Insert();
	bool ArcInsert();
	bool Swap();
	bool SwapArcs();
	bool SwapTwoArcs();

	// movements for direct encoding
	bool IntraRouteInsert();
	bool IntraRouteArcInsert();
};

#endif /* LOCALSEARCH_HPP_ */
