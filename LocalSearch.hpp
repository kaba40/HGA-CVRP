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

	void removeNode(Node* insertNode);
	void addTail(Node* insertNode);
	void addHead(Node* insertNode);
	void insertAfter(Node* insertNode, Node* moveNode);
	void insertBefore(Node* insertNode, Node* moveNode);
	void insertBetween(Node* insertNodePrev, Node* insertNode,
			Node* insertNodeNext);

public:
	LocalSearch(Solution *initSol); // solution seulement
	virtual ~LocalSearch();

	bool Insert();
	bool ArcInsert();
	bool Swap();
	bool SwapArcs();
	bool SwapTwoArcs();
};

#endif /* LOCALSEARCH_HPP_ */
