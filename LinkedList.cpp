#include "LinkedList.h"

Node::Node () : next(NULL), data(NULL) { };

Node::Node (T data) : next(NULL) {

	this.data = new T();

}

Node::~Node () {

	delete data;

}

Node * Node::getNext () const {

	return next;

}

T Node::getData () const {

	return *data;

}

void Node::setNext (Node *next) {

	this->next = next;

}

void Node::setData (T info) {

	this->data = new T(info);

}
