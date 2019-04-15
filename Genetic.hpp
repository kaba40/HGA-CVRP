/*
 * Population.hpp
 *
 *  Created on: 26 mars 2019
 *      Author: kkeita
 */

#ifndef GENETIC_HPP_
#define GENETIC_HPP_

#include "Solution.hpp"
#include "LocalSearch.hpp"

class Genetic
{
private:
	vector<Solution*> individus; // vector of population
	int nbIndivs; // number of individual in a population
	int maxIter; // maximum number of iteration
	int stuckMaxVal; // maximum number of iteration where dFactor = cdFactor (ceiling of dFactor)
	double dgFactor; // diversity growth factor
	double probaLS; //local search call probability
	DataAP *cvrp_data; // data class

	// two parents to be selected
	Solution *parent1;
	Solution *parent2;

//	// tow offspring to be created
//	Solution *son1;
//	Solution *son2;
public:
	//numInds = number of individual; maxIt = maximum iteration; dgFact = diversity growth factor;
	//probLS = localSearch call probability; stuckMax = maximum number of iteration where dFact = cdFact
	Genetic(DataAP *vrp_dat, int numInds, int maxIt, int stuckMax, double dgFact, double probLS);
	virtual ~Genetic();

	// create a local search class
//	LocalSearch *educate;

	// getter methods
	vector<Solution*> getIndividus();

	//crossover operator
//	Solution* Crossover(Solution *p1, Solution *p2, int start, int end);
	Solution* Crossover(Solution *p1, Solution *p2);

	// education operator
	void Educate(Solution *sol);

	// diversity manager operators
	int EditDistance( string str1, string str2, int len1, int len2);
	int Diversity(Solution *child, int k);

	//solver
	int Solve();

};

#endif /* GENETIC_HPP_ */
