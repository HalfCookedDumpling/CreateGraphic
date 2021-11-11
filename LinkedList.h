#ifndef __LINKED_LIST__
#define __LINKED_LIST__

template <class T> 
class Node {

	public:

		Node ();
	
		Node (T data);

		~Node ();

		Node * getNext () const;

		T getData () const;

		void setNext (Node *next);
			
		void setData (T info);

	private: 

		Node *next;

		T *data;

};

template <class T>
class LinkedList {

	public:

		LinkedList ();

		void addNode (T data);

		void insertNode (T data, int index);

		void deleteNode (int index);

		void delete ();

		Node * getNode (int index) const;

		T get (int index) const;

		int getLength () const;

	private:

		Node *head;

		int length;

};


#endif