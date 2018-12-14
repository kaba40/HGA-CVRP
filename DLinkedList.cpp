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
    // kairaba: il faut supprimer cette condition et son contenu. C'est inutile. Si tu gardes cela,
    // kairaba: tu auras des problèmes avec l'instruction plus bas : head->setPrevious(node);
    // kairaba: tu construis une liste circulaire
    if(!head) // if head is null
    {
	head = node;
	size = 1;
	// why tail = node; ?
    }

    //  why this instruction "node->getNext() = head;" lvalue required as left operand of assignment error ?
    // kairaba : l'instruction ne marche pas car le membre gauche d'une affectation doit être une variable
    
    node->setNext(head);
    // kairaba : il faut tester que head n'est pas null avant d'appeler l'instruction suivante 
    head->setPrevious(node);
    // kairaba : head = node : tu fais pointer la tête sur node pour que node devienne la nouvelle tête
    node = head; // node become head
    // kairaba: pour être cohéerent avec ce qui est dans le constructeur, je mettrai : node->setPrevious(NULL);
    node->setPrevious(nullptr);
    size++;
}

void DLinkedList::push_back(Node *node) // add an element to the end
{
    if(!node)
	throw invalid_argument(" Non-empty list pointer can't be null");

    // kairaba : mes remarques dans push_front sont similaires ici aussi
    if(!tail) // if list is empty
    {
	// why head = node;
	tail = node;
	size = 1;
    }

    node->setPrevious(tail);
    tail->setNext(node);
    node = tail ; // node become tail
    node->setNext(nullptr);
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
    else if(position >= size)
	push_back(node);
    else
    {
	if(!node)
	    throw invalid_argument("Non-empty list pointer can't be null");

	Node *tmp = head;
	// kairaba : ce n'est pas bon car à chaque itération, tu modifies les liens
	// kairaba : tu dois parcourir la liste pour chercher le noeud qui va précéder ou succéder node.
	// kairaba : dès que tu as trouver ce noeud, tu t'arrêtes
	// kairaba : ensuite tu construis les liens avec node
	for(int i = 0; i < position-1; i++)
	{
	    tmp = tmp->getNext();
	    node->setNext(tmp->getNext());
	    tmp->setNext(node);
	    size++;
	}
    }
}

Node* DLinkedList::pop_front()
{

    // kairaba : on peut imaginer aussi que quand la tête est NULL,
    // kairaba : on retourne NULL. C'est une question de goût
    if(!head)
	throw out_of_range("Can't delete from empty list");

    // kairaba : je mettrai NULL et pas nullptr pour respecter ce que tu as mis dans le constructeur.
    // kairaba : ou tu utilises nullptr dans le constructeur
    Node *tmp = nullptr;
    tmp = head;

    // kairaba : le code suivant ne fonctionne pas.
    // kairaba : après l'appel de cette méthode, head devra pointer sur son suivant
    // kairaba : il ne faut jamais supprimer une variable contenant un pointeur NULL.
    // kairaba : tu ne fais ici qu'enlever de la liste la première case
    head = nullptr;
    delete head;

    // kairaba : ok
    size--;

    return tmp;

}

Node* DLinkedList::pop_back()
{
    if(!tail)
	throw string("Cannot delete from empty queue");

    Node *tmp = nullptr;
    tmp = tail;
    // kairaba : ce que j'ai dit pour pop_front est également valable
    tail = nullptr;
    delete tail;

    // kairaba : ok
    size--;

    return tmp;
}
