/*
 * LocalSearch.hpp
 *
 *  Created on: 19 déc. 2018
 *      Author: kkeita
 */

#ifndef LOCALSEARCH_HPP_
#define LOCALSEARCH_HPP_

#include<stdio.h>
#include<limits>
#include<float.h>
#include<math.h>

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

	// intra route movements for direct encoding, i.e, feasible solution
	bool IntraRouteInsert();
	bool IntraRouteArcInsert();
	bool IntraRouteSwap();
	bool IntraRouteArcSwap();
	bool IntraRoute2ArcSwap();

	// inter-route movements
	bool InterRouteInsert();
	bool InterRouteArcInsert();
	bool InterRouteSwap();
	bool InterRouteArcSwap();
	bool InterRoute2ArcSwap();

	// 2-opt movement
	int InterRoute2Opt();

	//successive localSearch on direct and indirect encoding
	void IterativeSolutionImprovement(bool directEncoding);

};

#endif /* LOCALSEARCH_HPP_ */
