/*
 * ILS.hpp
 *
 *  Created on: 24 avr. 2019
 *      Author: kkeita
 */

#ifndef ILS_HPP_
#define ILS_HPP_

#include<stdio.h>

#include "Solution.hpp"
#include "LocalSearch.hpp"

class ILS
{
private:
	Solution *incumbent;
public:
	ILS(Solution *initSol);
	virtual ~ILS();

	bool SteepestDescent();
	bool Perturbation();
	bool Accept(Solution* lastLocalOptimum, Solution* firstLocalOptimum);
	int Solve();
};

#endif /* ILS_HPP_ */
