/*
 * main.cpp
 *
 *  Created on: 21 nov. 2018
 *      Author: kkeita
 */

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<algorithm>
#include<random>
#include <boost/program_options.hpp>

#include "DataAP.hpp"
#include "Customer.hpp"
#include "SplitBellman.hpp"
#include "Solution.hpp"
#include "Node.hpp"
#include "DLinkedList.hpp"
#include "Genetic.hpp"
#include "LocalSearch.hpp"
#include "ILS.hpp"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char** argv)
{
	string name_data = "/home/kkeita/git/algosplit/Data/E-n101-k14.vrp";
	const char* data_vrp = name_data.c_str();

	// genetic paraterms initialisation
	int numInds = 5; // number of individual
	int maxIt = 100; // maximum number of iteration
	int stuckMax = 3; // maximum number of iteration where dFactor = cdFactor (ceiling of dFactor)
	double dgFactor = 1.08; // diversity growth factor
	double probLS = 0.5; // Local search call probability

	try{

		po::options_description desc("Allowed options");
		desc.add_options()
		("help", "produce help message")
		("name_data", po::value< string>(), "instance used")
		("numInds", po::value<int>(), "number of individual")
		("maxIt", po::value<int>(), "maximum iteration")
		("stuckMax", po::value<int>(), "stuck iteration")
		("dgFactor", po::value<double>(), "diversity growth factor")
		("probLS", po::value<double>(), "localSearch probability");


		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		if (vm.count("help"))
		{
			cout << desc << "\n";
			return 1;
		}

		if(vm.count("name_data"))
		{
			name_data = vm["name_data"].as< string >();
		}
		cout << "name_data was set to : " << " " << name_data << endl;

		if(vm.count("numInds"))
		{
			numInds = vm["numInds"].as<int>();
		}
		cout << "number of individual was set to : " << " " << numInds << endl;

		if(vm.count("maxIt"))
		{
			maxIt = vm["maxIt"].as<int>();
		}
		cout << "number of individual was set to : " << " " << maxIt << endl;

		if(vm.count("stuckMax"))
		{
			stuckMax = vm["stuckMax"].as<int>();
		}
		cout << "stuckMax was set to : " << " " << stuckMax << endl;

		if(vm.count("dgFactor"))
		{
			dgFactor = vm["dgFactor"].as<double>();
		}
		cout << "diversity growth factor was set to : " << " " << dgFactor << endl;

		if(vm.count("probLS"))
		{
			probLS = vm["probLS"].as<double>();
		}
		cout << "localSearch call probability was set to : " << " " << probLS << endl;

	}
	catch(exception& e)
	{
		cerr << "error :" << e.what() << "\n";
		return 1;
	}
	catch(...)
	{
		cerr << "Exception of unknown type!\n";
	}

	DataAP * data_instance;
	data_instance = new DataAP(data_vrp);
#ifdef DEBUG_Main
	cout << " affichage d'information d'une instance " << endl;

	cout<< " number of nodes = "<< data_instance->getNumberNodes() << endl;
	cout << " vehicle capacity = " << data_instance->getVehicleCap() << endl;

	cout <<  endl;

#endif
	Solution *solution_tsp;
	solution_tsp = new Solution(data_instance, data_instance->getCustomerList());

//	srand(unsigned (time(0)) );
//	solution_tsp->setRandomSequence();

#ifdef DEBUG_Main

	for(Node *custoNode = solution_tsp->getSequence()->getHead(); custoNode != NULL; custoNode = custoNode->getNext())
	{
		cout << "custo " << custoNode->getClient()->getId() << " index " << custoNode->getClient()->getIndex() << " demand " << custoNode->getClient()->getDemand() << endl;
	}

#endif

//	exit(-1);

	solution_tsp->getSequence()->show(); cout << "Head--Tail : " << solution_tsp->getSequence()->getHead()->getClient()->getId() << "--" << solution_tsp->getSequence()->getTail()->getClient()->getId() << endl;

//	cout << "list as " << solution_tsp->getSequence()->toString() << endl;


//	ILS *localSolver = new ILS(solution_tsp);
//
//	if(localSolver->Solve())
//	{
//		 solution_tsp->CheckSolution(false);
//		 solution_tsp->PrintSolution(false);
//	}
//
//	exit(-1);
	// test de la classe Genetic
	Genetic *geneticAlgo = new Genetic(data_instance, numInds, maxIt, stuckMax, dgFactor, probLS);


	for(uint i = 0; i < geneticAlgo->getIndividus().size(); i++)
	{
		cout << "individu[" << i << "] " ; geneticAlgo->getIndividus()[i]->getSequence()->show(); cout << "cost = " << geneticAlgo->getIndividus()[i]->getObjVal() << endl;
	}

	double timeDebut = clock();
	geneticAlgo->Solve();
	double timeFin = clock();

	cout << "temps d'execution HGA = " << (timeFin-timeDebut)/(CLOCKS_PER_SEC) << endl; //(double) 1000

	for(uint i = 0; i < geneticAlgo->getIndividus().size(); i++)
	{
		cout << "individu[" << i << "] " ; geneticAlgo->getIndividus()[i]->getSequence()->show(); cout << "cost = " << geneticAlgo->getIndividus()[i]->getObjVal() << endl;
	}

	geneticAlgo->getIndividus().front()->PrintSolution(false);

	exit(-1);

	// true means we use direct encoding to do local search
	if(solution_tsp->Decodage(true))
	{
        solution_tsp->CheckSolution(true);
		solution_tsp->PrintSolution(true);
	}

	LocalSearch* localAlgo = new LocalSearch(solution_tsp, true);

	localAlgo->IterativeSolutionImprovement(true);

	solution_tsp->CheckSolution(true);
	solution_tsp->PrintSolution(true);

	localAlgo->InterRoute2Opt();

	solution_tsp->PrintSolution(true);
	solution_tsp->CheckSolution(true);

	localAlgo->IterativeSolutionImprovement(true);

	solution_tsp->CheckSolution(true);
	solution_tsp->PrintSolution(true);


	exit(-1);

	solution_tsp->restoreSequence();

	if(solution_tsp->Decodage(false))
		solution_tsp->CheckSolution(false);

	localAlgo->IterativeSolutionImprovement(false);

	solution_tsp->CheckSolution(false);
	solution_tsp->PrintSolution(false);




//	delete localSolver;
//	delete geneticAlgo;
	delete localAlgo;
	delete data_instance;
	delete solution_tsp;


	return 0;
}
