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
	static const string DIMENSION = "DIMENSION";
	static const string DEMAND_SECTION = "DEMAND_SECTION";
	static const string DEPOT_SECTION = "DEPOT_SECTION";
	static const string EDGE_WEIGHT_TYPE = "EDGE_WEIGHT_TYPE";
	static const string EUC_2D = "EUC_2D";
	static const string EXPLICIT = "EXPLICIT";
	static const string LOWER_DIAG_ROW = "LOWER_DIAG_ROW";
	static const string EDGE_WEIGHT_FORMAT = "EDGE_WEIGHT_FORMAT";
	static const string EDGE_WEIGHT_SECTION = "EDGE_WEIGHT_SECTION";
	static const string NODE_COORD_SECTION = "NODE_COORD_SECTION";
	static const string CAPACITY = "CAPACITY";

	ifstream fichier(data_vrp);

	if(!fichier)
	{
		cerr << "cannot open file " << data_vrp << endl;
		return ;
	}

	string edge_weight_type = "";
	string edge_weight_format = "";
	vector<int> x;
	vector<int> y;

	while(fichier)
	{
		//--------------------
		// Read keyword.
		//--------------------

		string key;
		string dummy;
		fichier >> key;

		if(key == DIMENSION)
		{
			fichier >> dummy;
			fichier >> numberNodes;
			distNodes.resize(numberNodes); /*lint !e732 !e747*/
		}

		if(key == CAPACITY)
		{
			fichier >> dummy;
			fichier >> vehicleCap;
		}
		else if( key == EDGE_WEIGHT_TYPE)
		{
			fichier >> dummy;
			fichier >> edge_weight_type;
			if( edge_weight_type != EUC_2D && edge_weight_type != EXPLICIT)
			{
				cerr << "Wrong " << EDGE_WEIGHT_TYPE << " " << edge_weight_type << endl;
				return ;
			}
			if( edge_weight_type == EUC_2D )
			{
				x.resize(numberNodes);
				y.resize(numberNodes);
			}
		}
		else if ( key == EDGE_WEIGHT_FORMAT )
		{
			fichier >> dummy;
			fichier >> edge_weight_format;
		}
		else if ( key == EDGE_WEIGHT_FORMAT + ":" )
		{
			fichier >> edge_weight_format;
		}
		else if ( key == EDGE_WEIGHT_SECTION )
		{
			if ( edge_weight_type != EXPLICIT || edge_weight_format != LOWER_DIAG_ROW )
			{
				cerr << "Error. Unsupported edge length type." << endl;
				return ;
			}
			for (int i = 0; i < numberNodes; i++)
			{
				for (int j = 0; j < i; j++)
				{
					int l;
					fichier >> l;
					//						distNodes[i][j] = l; /*lint !e732 !e747*/
					distNodes[i].push_back(l); /*lint !e732 !e747*/
				}
			}
#ifdef DEBUG_Distance
			for(int i = 0; i < numberNodes; i++)
			{
				for(int j = 0; j < i; j++)
				{
					cout << "distNode["<< i<< "][" << j << "] =" << distNodes[i][j] << endl;
				}
			}
#endif
		}
		else if ( key == NODE_COORD_SECTION )
		{
			if ( edge_weight_type != EUC_2D )
			{
				cerr << "Error. Data file contains " << EDGE_WEIGHT_TYPE << " " << edge_weight_type << " and " << NODE_COORD_SECTION << endl;
				return ;
			}
			for (int i = 0; i < numberNodes; i++)
			{
				int j, xi, yi;
				fichier >> j;
				fichier >> xi;
				fichier >> yi;
				if ( j != i+1 )
				{
					cerr << "Error reading " << NODE_COORD_SECTION << endl;
					return ;
				}
				x[i] = xi; /*lint !e732 !e747*/
				y[i] = yi; /*lint !e732 !e747*/
			}
			for (int i = 0; i < numberNodes; i++)
			{
				for (int j = 0; j < i; j++)
				{
					int dx = x[i] - x[j]; /*lint !e732 !e747 !e864*/
					int dy = y[i] - y[j]; /*lint !e732 !e747 !e864*/
					int d = int( sqrt((double)dx*dx + dy*dy) + 0.5 ); /*lint !e732 !e747 !e790*/
					//					  distNodes[i][j] = int( sqrt((double)dx*dx + dy*dy) + 0.5 ); /*lint !e732 !e747 !e790*/
					distNodes[i].push_back(d);
				}
			}
#ifdef DEBUG_Distance
			for(int i = 0; i < numberNodes; i++)
			{
				for(int j = 0; j < i; j++)
				{
					cout << "distNode["<< i<< "][" << j << "] =" << distNodes[i][j] << endl;
				}
			}
#endif
		}
		else if ( key == DEMAND_SECTION )
		{
			string skip = skip_line(fichier,2);
			//			  cout << "skip = " << skip << endl;
			//fill vector of customer
			clients = vector<Customer*> (numberNodes-1); // ici numberNodes = nombre des clients + le dépôt
			for (int i = 0; i < numberNodes-1; i++)
			{
				string Id;
				int Idx;
				int Dde;
				fichier >> Id;
				fichier >> Dde;
				Idx = i;

				clients[i] = new Customer(Id, Idx, Dde, this);

			}

			//#ifdef DEBUG_DataAP
			for(int i = 0; i < numberNodes-1; i++)
			{
				cout << "custo " << clients[i]->getId() << " index " << clients[i]->getIndex() << " demand " << clients[i]->getDemand() << endl;
			}
			//#endif

		}
		else if ( key == DEPOT_SECTION )
		{
			for (int i = 0; i != -1 ;)
			{
				fichier >> i;
				if ( i != -1 && i != 1 )
				{
					cerr << "Error: This file specifies other depots than 1." << endl;
					return ;
				}
			}
		}
		else
		{
			(void) getline(fichier, dummy);
		}
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
	if(index1 >= numberNodes || index2 >= numberNodes)
	{
		cerr << " index1 and index2 must be inferior to " << numberNodes << endl;
		throw string(" Verify index");
	}

	if( index1 == index2)
	{
		cerr << " index1 and index2 must not be equal " << endl;
		throw string(" Verify index");
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
