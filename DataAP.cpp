/*
 * DataAP.cpp
 *
 *  Created on: 22 nov. 2018
 *      Author: kkeita
 */

#include "DataAP.hpp"
#include "Customer.hpp"

DataAP::DataAP()
{
	 numberNodes = 0;
	 numberVehicle = 0;
	 vehicleCap = 0;
	 depotIndex = 0;
}
DataAP::DataAP( const char* data_vrp, int nbVeh)
{
	readData(data_vrp, nbVeh);
}

DataAP::~DataAP()
{
	for(int i = 1; i < numberNodes+1; i++)
	{
		delete clients[i];
	}
}

void DataAP::readData( const char* data_vrp, int nbVeh)
{
	numberVehicle = nbVeh;

	// lecture des données

	string line;
	string uselessStr;


	ifstream fichier(data_vrp);
	if(fichier.is_open())
	{
		// skip useless lines
		getline(fichier, line);
		getline(fichier, line);
		getline(fichier, line);


		// read number of nodes
		fichier >> uselessStr;
		fichier >> uselessStr;
		fichier >> numberNodes;

		distNodes.resize(numberNodes+1);
		for(int i = 1; i < numberNodes+1; i++)
			distNodes[i].resize(i, 0);

//		cout << "number of nodes = " << nbNodes << endl;
//		exit(-1);

		//skip useless lines
		getline(fichier, line);
		getline(fichier, line);
		getline(fichier, line);
		getline(fichier, line);

		//read vehicle capacity
		fichier >> uselessStr;
		fichier >> uselessStr;
		fichier >> vehicleCap;

		//skip useless line
		getline(fichier, line);
		getline(fichier, line);

		// get distances between nodes in EDGE_WEIGHT_SECTION
		for(int i = 1; i < numberNodes+1; i++)
		{
			for(int j = 1; j < i; j++)
			{
				int d;
				fichier >> d;
				distNodes[i][j] = d;
			}
		}

		//skip useless string
		fichier >> uselessStr;

		//fill vector of customer
		clients = vector<Customer*> (numberNodes+1);

		for(int i = 1; i < numberNodes+1; i++)
		{
			int Idx, Dde;
			fichier >> Idx;
			fichier >> Dde;

			clients[i] = new Customer(Idx, Dde, this);
		}

		for(int i = 1; i < numberNodes+1; i++)
		{
			cout << "custo " << i << " index " << clients[i]->getIndex() << " demand " << clients[i]->getDemand() << endl;
		}

		// skip useless string
		fichier >> uselessStr;

		// get index of depot
		fichier >> depotIndex;
//		string fline = "DEMAND_SECTION" ;

		fichier.close();
	}
	else
	{
	      cout << "Error opening file " << data_vrp << endl;
	      abort();
	}
}

int DataAP::getNumberNodes()
{
	return numberNodes;
}

int DataAP::getNumberVehicle()
{
	return numberVehicle;
}

int DataAP::getVehicleCap()
{
	return vehicleCap;
}

double DataAP::getDistances(int index1, int index2)
{
	if(index1 == 0 || index2 == 0)
	{
		cerr << " index1 and index2 must be superior to 0" << endl;
		exit(-1);
	}

	if(index1 > numberNodes || index2 > numberNodes)
	{
		cerr << " index1 and index2 must be inferior to " << numberNodes << endl;
		exit(-1);
	}

	if( index1 == index2)
	{
		cerr << " index1 and index2 must not be equal " << endl;
		exit(-1);
	}

	if(index1 < index2)
	{
		return distNodes[index2][index1];
	}

	return distNodes[index1][index2];
}

Customer* DataAP::getCustomerByIndex( int index)
{
	if(index == 0)
	{
		cerr << " index must be superior to 0" << endl;
		exit(-1);
	}

	if(index > numberNodes)
	{
		cerr << " index must be inferior to " << numberNodes << endl;
		exit(-1);
	}

	for(int i = 1; i < numberNodes+1; i++)
	{
		Customer *returCusto = clients[i];

		if(index == returCusto->getIndex())
		{
			return returCusto;
		}
	}

	return NULL;
}
