/*
 * Customer.hpp
 *
 *  Created on: 23 nov. 2018
 *      Author: kkeita
 */

#ifndef CUSTOMER_HPP_
#define CUSTOMER_HPP_

//#include "DataAP.hpp"
class DataAP;
class Customer
{
private:
	int index;
	int demand;

public:
	Customer(int ind, int dem);
	virtual ~Customer();

	double getDistance(Customer *c);
	int getIndex();
	int getDemand();
};

#endif /* CUSTOMER_HPP_ */
