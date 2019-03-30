/*
 * Population.hpp
 *
 *  Created on: 26 mars 2019
 *      Author: kkeita
 */

#ifndef POPULATION_HPP_
#define POPULATION_HPP_

#include "Solution.hpp"
#include "LocalSearch.hpp"

class Population
{
private:
	vector<Solution*> individus;
	int nbIndivs;
	DataAP *cvrp_data;
public:
	Population(DataAP *vrp_dat, int numInds);
	virtual ~Population();

	// create a local search class
//	LocalSearch *educate;

	// getter methods
	vector<Solution*> getIndividus();
	int getNumberOfIndividus();

	// education methods
	bool Education(Solution *individu);

	// add et remove an individu to the population
	void addIndividu(Solution *individu);
	void removeIndividu(Solution *individu);
};

#endif /* POPULATION_HPP_ */
