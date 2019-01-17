/*
 * DataAP.cpp
 *
 *  Created on: 22 nov. 2018
 *      Author: kkeita
 */

#include "DataAP.hpp"
#include "Customer.hpp"

string skip_line(ifstream& file, int n){

	string line;
	for(int i =0; i <n; ++i)
		getline(file, line);
	return line;
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
}
DataAP::DataAP( const char* data_vrp, int nbVeh)
{
	readData(data_vrp, nbVeh);
}

DataAP::~DataAP()
{
	for(int i = 0; i < numberNodes-1; i++)
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
#ifdef SPLIT_ALGORITHM_DEBUG
		for(int i = 0; i < numberNodes; i++)
		{
			for(int j = 0; j < i; j++)
			{
				cout << "distNode["<< i<< "][" << j << "] =" << distNodes[i][j] << endl;
			}
		}
#endif
		//skip useless string
		fichier >> uselessStr;
		string skip = skip_line(fichier,2);

		//fill vector of customer
		clients = vector<Customer*> (numberNodes-1); // ici numberNodes = nombre des clients + le dépôt

		for(int i = 0; i < numberNodes-1; i++)
		{
			string Id;
			int Idx;
			int Dde;
			fichier >> Id;
			fichier >> Dde;
			Idx = i;

			clients[i] = new Customer(Id, Idx, Dde, this);
		}

#ifdef SPLIT_ALGORITHM_DEBUG
		for(int i = 0; i < numberNodes-1; i++)
		{
			cout << "custo " << clients[i]->getId() << " index " << clients[i]->getIndex() << " demand " << clients[i]->getDemand() << endl;
		}
#endif

		// skip useless string
		fichier >> uselessStr;

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
#ifdef DEBUG_DataAP
	cout << "index1 =" << index1 << " index2 = " << index2 << endl;
#endif
	if(index1 >= numberNodes || index2 >= numberNodes) //
	{
		cerr << " index1 and index2 must be inferior to " << numberNodes << endl;
		throw string(" Verify index"); //exit(-1);


	}

	if( index1 == index2)
	{
		cerr << " index1 and index2 must not be equal " << endl;
		throw string(" Verify index"); //exit(-1);
	}

	if(index1 < index2)
	{
		return distNodes[index2][index1];
	}

	return distNodes[index1][index2];
}

Customer* DataAP::getCustomerByIndex( int index)
{
	// assert(index > 0 && index <= numnerNodes); precondition
	if(index < 0 || index >= numberNodes)
	{
		cerr << " index must be superior to 0 or lower than " << numberNodes << endl;
		throw string(" Verify index"); //exit(-1); // generer une exception
	}
//	index--;

	return clients[index];

}

vector<Customer*> DataAP::getCustomers()
{
	return clients;
}

DLinkedList* DataAP::getCustomerList()
{

	DLinkedList* encoding = new DLinkedList();
	for(uint i = 0; i < clients.size(); i++)
	{
		Node *node = new Node(clients[i]);
		encoding->push_back(node);
	}

	return encoding;
}
