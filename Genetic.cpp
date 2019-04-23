/*
 * Population.cpp
 *
 *  Created on: 26 mars 2019
 *      Author: kkeita
 */

#include "Genetic.hpp"
#include<limits> // DBL_MAX;
#include<algorithm> // min
#include<time.h>

// to sort a vector of objects solution
struct solCompare{
	bool operator()(const Solution *sol1, const Solution *sol2) const
	{
		return sol1->getObjVal() < sol2->getObjVal();
	}
}solCompare;

// return random double between a and b
double randFloat(double a, double b)
{
	return ((b-a)*((float)rand()/RAND_MAX))+a;
}

// return random integer between a and b
int randomInt(int min, int max){
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

// constructor genetic algorithm
Genetic::Genetic(DataAP *vrp_dat, int numInds, int maxIt, int stuckMax, double dgFact, double probLS)
{
	this->cvrp_data = vrp_dat;
	this->nbIndivs = numInds;
	this->maxIter = maxIt;
	this->stuckMaxVal = stuckMax;
	this->dgFactor = dgFact;
	this->probaLS = probLS;

	this->parent1 = NULL;
	this->parent2 = NULL;

	for(int i = 0; i < nbIndivs; i++)
	{
		Solution *individu = new Solution(cvrp_data, cvrp_data->getCustomerList()) ;

		individu->setRandomSequence();

		// apply a decoding to evaluate the individual
		// false means we use indirect encoding to do local search
		if(individu->Decodage(false))
		{
			individu->CheckSolution(false);
		}

		this->individus.push_back(individu);
	}
}

Genetic::~Genetic(){
	// TODO Auto-generated destructor stub
}

// return population of individual to print infos in main.cpp
vector<Solution*> Genetic::getIndividus()
{
	return this->individus;
}

// crossover operator
Solution* Genetic::Crossover(Solution *p1, Solution *p2) // PIX, OX, etc
{
	Solution *retSol = NULL;

	// We define the beginning and end of the crossover zone
	int start = rand() % p1->getSequence()->getSize() ;
	int end = rand() % p1->getSequence()->getSize() ;
	while (end == start && p1->getSequence()->getSize() > 1)
		end = rand() % p1->getSequence()->getSize() ;

	// tow offspring to be created
	Solution *son1 = new Solution(cvrp_data, new DLinkedList());
	Solution *son2 = new Solution(cvrp_data, new DLinkedList());

	// put the block between start and end in parent1 into son1
	int i = 0;
	for(Node *p1Node = p1->getSequence()->getHead(); p1Node != NULL; p1Node = p1Node->getNext(), i++ )
	{
		// take into account case where start is greater than end ?
		if(i == start)
		{
			Node *copy = new Node(p1Node);
			son1->getSequence()->push_back(copy);
		}
		else if(i > start && i <= end)
		{
			Node *copy = new Node(p1Node);
			son1->getSequence()->push_back(copy);
		}
	}
	
#ifdef DEBUG_Genetic
	cout << "son1_1 = " ; son1->getSequence()->show(); cout << endl;
#endif

	// complete head of son1
	int addBeforeSon1 = 0;
	for(Node *p2Node = p2->getSequence()->getHead(); p2Node != NULL; p2Node = p2Node->getNext())
	{
		if(son1->getSequence()->search(p2Node) == false && addBeforeSon1 < start)
		{
			Node *copy = new Node(p2Node);
			son1->getSequence()->push_front(copy);
			addBeforeSon1++;
		}
	}
	
#ifdef DEBUG_Genetic
	cout << "son1_2 = " ; son1->getSequence()->show(); cout << endl;
#endif

	// complete tail of son1
	int addAfterSon1 = end;
	for(Node *p2Node = p2->getSequence()->getHead(); p2Node != NULL; p2Node = p2Node->getNext())
	{
		if(son1->getSequence()->search(p2Node) == false && addAfterSon1 < (p1->getSequence()->getSize() -1))
		{
			Node *copy = new Node(p2Node);
			son1->getSequence()->push_back(copy);
			addAfterSon1++;
		}
	}

#ifdef DEBUG_Genetic
	cout << "son1_3 = " ; son1->getSequence()->show(); cout << endl;
#endif

	// put the block between start and end in parent2 into son2
	int j = 0;
	for(Node *p2Node = p2->getSequence()->getHead(); p2Node != NULL; p2Node = p2Node->getNext(), j++)
	{
		if(j == start)
		{
			Node *copy = new Node(p2Node);
			son2->getSequence()->push_back(copy);
		}
		else if(j > start && j <= end)
		{
			Node *copy = new Node(p2Node);
			son2->getSequence()->push_back(copy);
		}
	}

#ifdef DEBUG_Genetic
	cout << "son2_1 = " ; son2->getSequence()->show(); cout << endl;
#endif

	// complete head of son2
	int addBeforeSon2 = 0;
	for(Node *p1Node = p1->getSequence()->getHead(); p1Node != NULL; p1Node = p1Node->getNext())
	{
		if(son2->getSequence()->search(p1Node) == false && addBeforeSon2 < start)
		{
			Node *copy = new Node(p1Node);
			son2->getSequence()->push_front(copy);
			addBeforeSon2++;
		}
	}

#ifdef DEBUG_Genetic
	cout << "son2_2 = " ; son2->getSequence()->show(); cout << endl;
#endif

	// complete tail of son2
	int addAfterSon2 = end;
	for(Node *p1Node = p1->getSequence()->getHead(); p1Node != NULL; p1Node = p1Node->getNext())
	{
		if(son2->getSequence()->search(p1Node) == false && addAfterSon2 < (p1->getSequence()->getSize() -1))
		{
			Node *copy = new Node(p1Node);
			son2->getSequence()->push_back(copy);
			addAfterSon2++;
		}
	}

#ifdef DEBUG_Genetic
	cout << "son2_3 = " ; son2->getSequence()->show(); cout << endl;
#endif

	// evaluate the two sons
	if(son1->Decodage(false))
	{
		son1->CheckSolution(false);
	}

	if(son2->Decodage(false))
	{
		son2->CheckSolution(false);
	}

	// return the best one may be we can return one child randomly ? // generate 0 et 1 if 1 first et 0 second
	if(son1->getObjVal() < son2->getObjVal() - 0.0001)
	{
		retSol = son1;
	}
	else
	{
		retSol = son2;
	}

	return retSol;
}

// education operator
void Genetic::Educate(Solution* sol)
{
	if(sol->Decodage(true))
			sol->CheckSolution(true);
	LocalSearch *localAlgo = new LocalSearch(sol, true);
	localAlgo->IterativeSolutionImprovement(true);
	sol->restoreSequence();
	if(sol->Decodage(false))
		sol->CheckSolution(false);
	localAlgo->IterativeSolutionImprovement(false);
}

// compute edit distance between two strings
int Genetic::EditDistance( string str1, string str2, int len1, int len2)
{

	if(len2 == 0)
	{
		return len1;
	}

	if(len1 == 0)
	{
		return len2;
	}

	if(str1[len1-1] == str2[len2-1]) // if les deux derniers sont egaux
	{
		return EditDistance(str1, str2, len1-1, len2-1);
	}
	else
	{
		int x = 1+ EditDistance(str1, str2, len1, len2-1); // enlever un du deuxième string
		int y = 1+ EditDistance(str1, str2, len1-1, len2); // enlever un du premier string
		int z = 1+ EditDistance(str1, str2, len1-1, len2-1); // enlever un de deux strings

		return min(x, min(y,z));
	}
}

// diversity manager operators
int Genetic::Diversity(Solution *child, int k)
{
	vector<int> cDiversity;

	for(uint i = 0; i < individus.size(); i++)
	{
		if(i != k)
		{
			string str1 = child->getSequence()->toString();
			string str2 = individus[i]->getSequence()->toString();
			cDiversity.push_back(EditDistance(str1, str2, str1.length(), str2.length()));
		}
	}

	auto retVal = min_element(begin(cDiversity), end(cDiversity));
	int mn = *retVal;
	return mn;
}

// implementation of genetic algorithm
int Genetic::Solve()
{
	// sort the population
	sort(individus.begin(), individus.end(), solCompare);

	cout << "best individual  = " ; individus.front()->getSequence()->show() ; cout << " obj = " << individus.front()->getObjVal() << endl;

	// parameters initialization
	int numIt = 0; // number of iteration
	int stuck = 0; // number of iteration with dFact = cdFact
	double dFact = 1; // diversity factor
	double mdFact = 0.5; // Maximum distance value factor
	int mdSize = individus[0]->getSequence()->getSize() - 1; // maximum distance value
	double cdFact = mdFact*mdSize ;// ceiling of diversity factor
	int pi = 0; // number of iterations without improving the best solution (P(1)): if pi = 50 then dFact := min(cdFact, dgFact*dFact)

	while(numIt <= maxIter) //stuck <= stuckMaxVal ||
	{
		numIt++;

		// select two parents in population: best one and bad one
		parent1 = individus.front();
		parent2 = individus.back();

#ifdef DEBUG_Genetic
		cout << "parent1  = " ; parent1->getSequence()->show(); cout << " obj = " << parent1->getObjVal() << endl;
		cout << "parent2  = " ; parent2->getSequence()->show(); cout << " obj = " << parent2->getObjVal() << endl;
#endif
		// cross parent1 and parent2 and return the best son
		Solution *selectedSon = Crossover(parent1, parent2);

		// apply selectedSon education randomly
		if( randFloat(0.0, 1.0) < probaLS - 0.0001)
		{
//			double timeDebut = clock();
			Educate(selectedSon);
//			double timeFin = clock();
//			cout << "temps d'execution Education = " << (timeFin-timeDebut)/(CLOCKS_PER_SEC) << endl; //(double) 1000
		}

#ifdef DEBUG_Genetic
		cout << "selectedSon2  = " ; selectedSon->getSequence()->show() ; cout << " obj = " << selectedSon->getObjVal() << endl;
#endif
		// draw k between |nbIndivs/2| and nbIndivs included // elitism
		int k;
		int n1 = nbIndivs-1;
		int n2 = (int) nbIndivs/2;
		k = randomInt(n2, n1);

		if(selectedSon->getObjVal() < individus.front()->getObjVal() - 0.0001) //&& Diversity(selectedSon, k) >= dFact
		{
			individus[k] = selectedSon; // individus.back() = selectedSon; // if modify last element
			sort(individus.begin(), individus.end(), solCompare); // contourner le sort decaler les autres puis placer soit avant
			cout << "best individual  = " ; individus.front()->getSequence()->show() ; cout << " obj = " << individus.front()->getObjVal() << endl;
		}
		else
		{
			pi++;
		}

		if(pi >= 50)
		{
			dFact = min(cdFact, dFact*dgFactor);
			pi = 0;
		}

		if(dFact == cdFact)
		{
			stuck++;
		}
		else
		{
			stuck = 0;
		}
//		cout << "numIt = " << numIt << endl;
	}

	cout << "stuck = " << stuck << endl;
	cout << "numIt = " << numIt << endl;

	return 1;
}


