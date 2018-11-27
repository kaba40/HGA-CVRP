/*
 * SplitBellman.hpp
 *
 *  Created on: 27 nov. 2018
 *      Author: kkeita
 */

#ifndef SPLITBELLMAN_HPP_
#define SPLITBELLMAN_HPP_

#include "DataAP.hpp"

class SplitBellman
{
private:
	DataAP * _tspData; //data instance
	vector<double> _pathCost; //cost of the shortest path
	vector<int> _pred; //index of the predecessor
public:
	SplitBellman(DataAP* tspData);
	int solve();
	virtual ~SplitBellman();
};

#endif /* SPLITBELLMAN_HPP_ */
