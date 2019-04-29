/*
 * ILS.cpp
 *
 *  Created on: 24 avr. 2019
 *      Author: kkeita
 */

#include "ILS.hpp"

ILS::ILS(Solution *initSol)
{
	this->incumbent = initSol;

}

ILS::~ILS() {
	// TODO Auto-generated destructor stub
}

bool ILS::SteepestDescent()
{
	bool retVal = false;
	if(this->incumbent->Decodage(false))
	{
		this->incumbent->CheckSolution(false);
	}

	double initObj = this->incumbent->getObjVal();
	LocalSearch *localAlgo = new LocalSearch(this->incumbent, false);
	localAlgo->IterativeSolutionImprovement(false);
	double exitObj = this->incumbent->getObjVal();

	if(exitObj < initObj - 0.0001)
	{
		retVal = true;
	}
	return retVal;
}

// try to improve perturbation
bool ILS::Perturbation()
{
	bool retVal = false;
	double initObj = this->incumbent->getObjVal();

	// apply split to the first local optimum
	if(this->incumbent->Decodage(true))
	{
		this->incumbent->CheckSolution(true);
	}
	LocalSearch *localAlgo = new LocalSearch(this->incumbent, true);

	localAlgo->InterRoute2Opt();

	this->incumbent->restoreSequence();
	if(this->incumbent->Decodage(false))
		this->incumbent->CheckSolution(false);

	double exitObj = this->incumbent->getObjVal();
	if(exitObj < initObj - 0.0001)
	{
		retVal = true;
	}
	return retVal;
}

bool ILS::Accept(Solution* lastLocalOptimum, Solution* firstLocalOptimum )
{
	if(lastLocalOptimum->getObjVal() < firstLocalOptimum->getObjVal() - 0.0001)
	{
		return true;
	}
	return false;
}

int ILS::Solve()
{
	// get the first local optimum of incumbent solution
	SteepestDescent();

#ifdef DEBUG_ILS
	cout << "objVal after steepestDescent = " << this->incumbent->getObjVal() << endl;
#endif
	// copy the first local optimum of incumbent solution
	Solution *firstLocalOptim = new Solution(this->incumbent);

#ifdef DEBUG_ILS
	cout << " firstLocalOptim objVal  = " << firstLocalOptim->getObjVal() << endl;
#endif

	time_t  startILS;
	double endILS, totILS = 0;

	while(totILS < 120.00)
	{
		startILS = time(0);

		// perturb the first local optimum of incumbent solution
		Perturbation();
#ifdef DEBUG_ILS
		cout << "objVal after perturbation = " << this->incumbent->getObjVal() << endl;
#endif
		SteepestDescent();

		// copy the second local optimum issued from the perturbation
		Solution *lastLocalOptim = new Solution(this->incumbent);
#ifdef DEBUG_ILS
		cout << " lastLocalOptim objVal  = " << lastLocalOptim->getObjVal() << endl;
#endif
		// accept or not last local optimum
		if(Accept(lastLocalOptim, firstLocalOptim))
		{
			cout << "Accepting new incumbent solution "; lastLocalOptim->getSequence()->show(); cout << "objVal = " << lastLocalOptim->getObjVal() << endl;
		}
		endILS = difftime(time(0), startILS);
		totILS += endILS;
#ifdef DEBUG_ILS
		cout << "totILS = " << totILS << endl;
#endif
	}

	return 1;
}
