/*
 * DLinkedList.cpp
 *
 *  Created on: 12 déc. 2018
 *      Author: kkeita
 */

#include "DLinkedList.hpp"

DLinkedList::DLinkedList()
{
    this->head = NULL;
    this->tail = NULL;
    size = 0;
}

DLinkedList::DLinkedList(Node *hd, Node *tl)
{
    this->head = hd;
    this->tail = tl;
    size = 0;

}

DLinkedList::~DLinkedList() {
    // TODO Auto-generated destructor stub
}


void DLinkedList::push_front(Node *node) // add an element to the front
{
    if(!node)
	throw invalid_argument(" Non-empty list pointer can't be null");

    if( head == NULL)
    {
    	head = node;
    	tail = node;
    	head->setNext(NULL);
    }
    else
    {
    	node->setNext(head);
    	head->setPrevious(node);
    	head = node;
    }

    head->setPrevious(NULL);
    size++;
}

void DLinkedList::push_back(Node *node) // add an element to the end
{
    if(!node)
	throw invalid_argument(" Non-empty list pointer can't be null");

    if( head==NULL )
    {
    	head = node;
    	tail = node;
    	tail->setPrevious(NULL);
    }
    else
    {
    	 node->setPrevious(tail);
    	 tail->setNext(node);
    	 tail = node;
    }

    tail->setNext(NULL);

    size++;
}

void DLinkedList::insert(int position, Node *node)
{
	if(!node)
		throw invalid_argument("Non-empty list pointer can't be null");

	// kairaba : normalement, avec une boucle bien construite, cela suffira

	// kairaba : on peut imaginer que quand c'est négatif, on veuille l'insérer à la fin ( on l'insère valeur_absolue(position) en partant de la fin)
	if(position <= 0)
		push_front(node);
	// kairaba : on peut imaginer que quand c'est supérieur à la taille, on veuille l'insérer au début (on l'insère valeur_absolue(position-size) en partant du début)
	else if(position > size)
		push_back(node);
	else
	{

		Node *tmp = head;
		int i = 1;

		while(tmp != NULL && i <= position)
		{
			if( i == position)
			{
				if(tmp->getPrevious() == NULL)
					push_front(node);
				else
				{

					tmp->getPrevious()->setNext(node);
					tmp->setPrevious(node);
					node->setPrevious(tmp->getPrevious());
					node->setNext(tmp);
					size++;

				}
			}
			else
			{
				tmp = tmp->getNext();
			}

			i++;
		}
	}
}

Node* DLinkedList::pop_front()
{

    // kairaba : on peut imaginer aussi que quand la tête est NULL,
    // kairaba : on retourne NULL. C'est une question de goût
    if(!head)
	throw out_of_range("Can't delete from empty list");

    Node *tmp = NULL;
    tmp = head;
    head = tmp->getNext();
    head->setPrevious(NULL);
    size--;

    return tmp;

}

Node* DLinkedList::pop_back()
{
    if(!tail)
	throw string("Cannot delete from empty queue");

    Node *tmp = NULL;
    tmp = tail;
    tail = tmp->getPrevious();
    tail->setNext(NULL);

    size--;

    return tmp;
}

void DLinkedList::delete_list()
{
	Node *tmp = head;
	while(tmp)
	{
		Node *current = tmp;
		tmp = tmp->getNext();

		delete current;
	}

	head = NULL;
	tail = NULL;
}

void DLinkedList::show()
{
	cout << " Afficher la liste : " << endl;

	Node *tmp = head;
	int num = 0;

	while(tmp)
	{
		cout << "Element " << num  << " de la liste = " << tmp->getData() << endl;
		tmp = tmp->getNext();
		num++;
	}
}


