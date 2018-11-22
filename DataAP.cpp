/*
 * DataAP.cpp
 *
 *  Created on: 22 nov. 2018
 *      Author: kkeita
 */

#include "DataAP.hpp"

DataAP::DataAP( const char* data_vrp)
{

	readData(data_vrp);
}

DataAP::~DataAP(){
	// TODO Auto-generated destructor stub
}

void DataAP::readData( const char* data_vrp)
{
	// lecture des données
	ifstream fichier;
	string line;
	string uselessStr;

	fichier.open(data_vrp);
	if(fichier.is_open())
	{
		// sauter les lignes inutiles
		getline(fichier, line);
		getline(fichier, line);
		fichier >> uselessStr;
		fichier >> uselessStr;

		// read number of vehicle
		fichier >> nbNodes;

		//skip useless string

		getline(fichier, line);
		getline(fichier, line);
		getline(fichier, line);
		fichier >> uselessStr;
		fichier >> uselessStr;

		//read vehicle capacity
		fichier >> vehicleCap;

		//skip useless line
		getline(fichier, line);


		// get Edge_Weight_Section three lines
		int nbLines = 0;
		for(int i = 1; i < nbNodes; i++)
			nbLines+=i;
		cout << "nbLines = " << nbLines << endl;
		arcCost = vector<double>(nbLines+1);
		arcCost[0] = 0;
		for(int i = 1; i < nbLines; i++)
		{

		}
		int i = 0;
		while( i < nbLines)
		{
			i++;
			fichier >> arcCost[i];
		}

		//skip useless string
		fichier >> uselessStr;

		//fill vector of customer
		clients = vector<customer> (nbNodes+1);

		clients[0].demand = 0;
		clients[0].index = 0;

		for(int i = 1; i < nbNodes; i++)
		{
			fichier >> clients[i].index;
			fichier >> clients[i].demand;
		}

		// skip useless string
		fichier >> uselessStr;

		// get index of depot
		fichier >> depotIndex;
//		string fline = "DEMAND_SECTION" ;


	}
}

