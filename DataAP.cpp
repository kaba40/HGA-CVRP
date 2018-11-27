/*
 * DataAP.cpp
 *
 *  Created on: 22 nov. 2018
 *      Author: kkeita
 */

#include "DataAP.hpp"
#include "Customer.hpp"

void skip_line(ifstream& file, int n){

	string line;
	for(int i =0; i <n; ++i)
		getline(file, line);
}

void skip_uselessStr_storeVal(ifstream& file, int& var){
	string uselessStr;
	file >> uselessStr;
	file >> uselessStr;
	file >> var;
}

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
		skip_line(fichier, 3);


		// read number of nodes
		skip_uselessStr_storeVal(fichier, numberNodes);

		distNodes.resize(numberNodes);
		/*
		for(int i = 0; i < numberNodes; i++)
			distNodes[i].resize(i, 0);
		//*/

		//skip useless lines
		skip_line(fichier, 4);

		//read vehicle capacity
		skip_uselessStr_storeVal(fichier, vehicleCap);

		//skip useless line
		skip_line(fichier, 2);

		// get distances between nodes in EDGE_WEIGHT_SECTION
		for(int i = 0; i < numberNodes; i++)
		{
			for(int j = 0; j < i; j++) // fill a complete matrix
			{
				int d;
				fichier >> d;
				distNodes[i].push_back(d);
			}
		}
#ifdef DEBUG
		for(int i = 0; i < numberNodes; i++)
		{
			for(int j = 0; j < i; j++)
			{
				cout << "distNode["<< i+1<< "][" << j+1 << "] =" << distNodes[i][j] << endl;
			}
		}
#endif
		//skip useless string
		fichier >> uselessStr;

		//fill vector of customer
		clients = vector<Customer*> (numberNodes);

		for(int i = 0; i < numberNodes; i++)
		{
			int Idx, Dde;
			fichier >> Idx;
			fichier >> Dde;

			clients[i] = new Customer(Idx, Dde, this);
		}
		/*
		for(int i = 0; i < numberNodes; i++)
		{
			cout << "custo " << i+1 << " index " << clients[i]->getIndex() << " demand " << clients[i]->getDemand() << endl;
		}
		//*/
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

int DataAP::getDepotIndex()
{
	return depotIndex;
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
		index1--;
		index2--;
		return distNodes[index2][index1];
	}

	index1--;
	index2--;
	return distNodes[index1][index2];
}

Customer* DataAP::getCustomerByIndex( int index)
{
	// assert(index > 0 && index <= numnerNodes); precondition
	if(index < 0 || index > numberNodes)
	{
		cerr << " index must be superior to 0 or lower than numberNodes" << endl;
		exit(-1); // generer une exception
	}

	if(index > 0)
		index--;
	return clients[index];

}
