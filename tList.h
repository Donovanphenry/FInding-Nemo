#ifndef CS20A_LIST_H
#define CS20A_LIST_H

#include<iostream>
#include<assert.h>
using std::cout;
using std::endl;

// Linked List object that maintains both head and tail pointers
// and the size of the list.  Note that you have to keep the head,
// tail and size consistent with the intended state of the List 
// otherwise very bad things happen. 

template<typename Type>
class List {
public:

	List();

	List(const List<Type>& other);

	List<Type>& operator=(const List<Type>& other);

	~List();

	void	print() const;
	bool	empty() const;

	void	push_front(const Type &item);
	void	push_back(const Type &item);
	void	add(int index, const Type &item);

	// Note that the user must first ensure the list is not empty
	// prior to calling these functions.
	Type	front() const;
	Type	rear() const;
	Type	get_at(int index) const;

	int		size() const;
	int		find(const Type &item) const;

	bool	pop_front();
	bool	pop_rear();
	bool	pop_at(int index);

	//private:

	// Forward declare the nodes for our List.
	// Will be implemented along with list's
	// member functions
	class Node;

	// We'll have both head and tail points for 
	// Fast insertion/deletion from both ends.
	Node*	m_head;
	Node*	m_tail;

	// Keep track of size of list
	int		m_size;
};


// List Implementation
//
//  Since List is a template class (which is not an actual 
//  class yet, not until we actually instantiate the list) 
//  we need to keep the implementation together with 
//  the definition.  There are ways to simulate having 
//  separate "implementation/definition" with templates, 
//  but they aren't necessary and can be confusing.
//

// Node definition
//		Already implemented, nothing to do here but to use it.
template<typename Type>
class List<Type>::Node {
public:
	Node() :next(nullptr), prev(nullptr) {}
	Node(Type item, Node* p, Node* n) : item(item), next(n), prev(p) {}

	Type  item;
	Node* next;
	Node* prev;
};



// List default constructor
//		Set head and tail pointer to point to nothing, size is zero
//		Already implemented, nothing to do.
template<typename Type>
List<Type>::List() :m_head(nullptr), m_tail(nullptr), m_size(0) {}

// Copy constructor  
//		This should create an entirely new linked list with the same 
//		number of Nodes and the Values stored these Nodes in the same 
//		order as seen the other list’s Nodes.  This should not result 
//		in any memory leaks or aliasing.  
template<typename Type>
List<Type>::List(const List<Type>& other)
{
	m_size = 0;

	m_head = nullptr;
	m_tail = nullptr;

	int i = 0;
	Node * pO = other.m_head;

	while (pO != nullptr)
	{
		add(i, pO->item);
		pO = pO->next;
		i++;
	}


}
// Overloaded assignment operator 
//		Causes the already existing linked list to be identical to the 
//		other linked list without causing any memory leaks or aliasing.
template<typename Type>
List<Type>& List<Type>::operator=(const List<Type>& other)
{
	if (this == &other)
		return *this;
	else
	{
		m_size = 0;

		while (m_head != nullptr)
		{
			Node * p = m_head;
			m_head = p->next;
			delete p;
		}

		m_head = nullptr;
		m_tail = nullptr;

		int i = 0;
		Node * pO = other.m_head;

		while (pO != nullptr)
		{
			add(i, pO->item);
			pO = pO->next;
			i++;
		}
	}

	return *this;
}


// List destructor
//		Traverses the nodes and deletes them without causing memory leaks
template<typename Type>
List<Type>::~List()
{
	while (m_head != nullptr)
	{
		Node * kill = m_head;
		m_head = kill->next;
		delete kill;
	}
}

// List print
//		Traverses list and prints items in list to console
//		Assumes << is overloaded for object Type.
//		

template<typename Type>
void List<Type>::print() const
{
	if (m_head == nullptr)
		return;

	else
	{
		Node * p = m_head;

		while (p != nullptr)
		{
			cout << p->item << " ";

			p = p->next;
		}
	}

}

// List empty
//		returns true if there are no items in the list
//		otherwise returns false.
template<typename Type>
bool List<Type>::empty() const
{
	if (m_head == nullptr)
		return true;
	return false;
}


// List push_front
//		Adds a new item to the front of the list
//		updates head, tail, and size accordingly.
//		
//		Two cases to consider: If the list is empty 
//		and if the list has items already it.
//
template<typename Type>
void List<Type>::push_front(const Type &item) {
	/* TODO */
	if (m_head == nullptr)
	{
		m_head = new Node;
		m_tail = new Node;
		m_head->item = item;
		m_head->prev = nullptr;
		m_head->next = nullptr;
		m_tail = m_head;
	}

	else
	{
		Node *p = new Node;
		p->next = m_head;
		p->prev = nullptr;
		p->item = item;
		m_head->prev = p;
		m_head = p;
	}

	m_size++;
}

// List push_back
//		Adds a new item to the rear of the list
//		updates head, tail, and size accordingly.
//
//		Two cases to consider: If the list is empty 
//		and if the list has items already it.
//
template<typename Type>
void List<Type>::push_back(const Type &item)
{
	if (m_tail == nullptr)
		push_front(item);

	else
	{
		Node *p = new Node;
		m_tail->next = p;
		p->next = nullptr;
		p->prev = m_tail;
		p->item = item;
		m_tail = p;

		m_size++;
	}

}

// List add
//		Adds a new item at the index indicated
//		updates head, tail, and size accordingly.
//
//		If the index less than or equal to zero add to the front.
//		If the index is greater than or equal to
//			the size of the list, add to the rear.
//		All other cases, add the item to the appropriate
//			location
//
template<typename Type>
void List<Type>::add(int index, const Type &item)
{
	if (m_size > 0)
	{
		if (index <= 0)
			push_front(item);

		else if (index >= m_size)
			push_back(item);

		else
		{
			Node * p = m_head;
			int i = 0;

			while (i < index - 1)
			{
				p = p->next;
				i++;
			}

			Node * addedNode = new Node;
			addedNode->item = item;

			if (p->next != nullptr)
			{
				addedNode->next = p->next;
				p->next->prev = addedNode;
				p->next = addedNode;
				addedNode->prev = p;
			}

			else
			{
				p->next = addedNode;
				addedNode->prev = p;
				m_tail = addedNode;
			}

			m_size++;
		}
	}
	else
	{
		Node * p = new Node;
		p->item = item;
		p->prev = nullptr;
		p->next = nullptr;
		m_head = p;
		m_tail = p;
		m_size++;
	}
}

// 8. List front
//		returns the first item in the list.
//
template<typename Type>
Type List<Type>::front() const {
	// Force check that the head is pointing to a Node
	// Typical solution for deployement code is to throw
	// exceptions, but since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(m_head != nullptr);

	/* TODO */
	Type dummyReturnValue_itShouldBeClearThatYouReplaceThis = m_head->item;
	return dummyReturnValue_itShouldBeClearThatYouReplaceThis;
}

// List rear
//		returns the last item in the list
//
template<typename Type>
Type List<Type>::rear() const {

	// Force check that the tail is pointing to a Node
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(m_tail != nullptr);

	/* TODO */
	Type dummyReturnValue_itShouldBeClearThatYouReplaceThis = m_tail->item;
	return dummyReturnValue_itShouldBeClearThatYouReplaceThis;

}

// List get_at
//		returns the item at index
//
template<typename Type>
Type List<Type>::get_at(int index) const {

	// Force index to be correct before getting the Item
	// Typical solution for deployement code is to throw
	// exceptions, since we haven't covered that yet
	// we'll make due with assert, which is used for testing
	assert(index >= 0 && index < m_size);

	Node * p = m_head;
	int i = 0;

	while (i < index)
	{
		p = p->next;
		i++;
	}

	/* TODO */
	Type dummyReturnValue_itShouldBeClearThatYouReplaceThis = p->item;
	return dummyReturnValue_itShouldBeClearThatYouReplaceThis;
}

// List size
//		return the size of the list
template<typename Type>
int List<Type>::size() const {
	/* TODO */
	return m_size;
}

// List find
//		Traverses the list to see if the item is in the list
//		if it is return the index of that item, otherwise
//		return -1. Assumes the == operator is overloaded for
//		object Type.
template<typename Type>
int List<Type>::find(const Type &item) const {

	Node * p = m_head;
	int i = 0;
	bool found = false;

	while (p != nullptr)
	{
		if (p->item == item)
		{
			found = true;
			break;
		}
		p = p->next;
		i++;
	}

	if (found)
		return i;
	else
		return -1;

}

// List pop_front
//		Removes the first item in the list returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly.
//
//		Must consider at least two cases where the list: is empty, 
//		has one or more items.
//		
template<typename Type>
bool List<Type>::pop_front()
{
	if (m_head != nullptr)
	{
		if (m_head->next != nullptr)
		{
			Node * p = m_head;
			m_head = p->next;
			m_head->prev = nullptr;
			m_size--;
			delete p;

			return true;
		}
		else
		{
			Node * p = m_head;
			m_head = nullptr;
			m_tail = nullptr;
			delete p;
			m_size--;

			return true;
		}
	}

	/* TODO */
	return false;
}
// List pop_rear
//		Removes the last item in the list returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly.
//
//		Must consider at least three cases where the list: is empty, 
//		has one item, has two or more items.
//	
template<typename Type>
bool List<Type>::pop_rear()
{
	if (m_tail != nullptr)
	{
		if (m_tail->prev != nullptr)
		{
			Node * p = m_tail;
			m_tail = p->prev;
			m_tail->next = nullptr;
			m_size--;
			delete p;
		}
		else
		{
			Node *p = m_tail;
			m_tail = nullptr;
			m_head = nullptr;
			m_size--;
			delete p;
		}

		return true;
	}

	return false;
}

// List pop_at
//		Removes the item at the index returns true
//		if the item was deleted, false otherwise. Updates 
//		head, tail, and size accordingly. If the index is
//		out of bounds return false.
//
template<typename Type>
bool List<Type>::pop_at(int index)
{

	if (index < 0 || index >= m_size)
		return false;

	else
	{
		if (m_size > 1)
		{
			if (index > 0 && index <= m_size - 1)
			{
				Node * p = m_head;
				int i = 0;

				while (i < index - 1)
				{
					p = p->next;
					i++;
				}

				Node * kill = p->next;

				if (kill->next != nullptr)
				{
					Node * temp = kill->next;
					p->next = kill->next;
					temp->prev = p;
				}
				else
				{
					p->next = nullptr;
					m_tail = p;
				}

				delete kill;
				m_size--;
			}

			else
			{
				Node * p = m_head;
				m_head = p->next;
				m_head->prev = nullptr;
				delete p;
				m_size--;

			}
		}

		else
		{
			Node * p = m_head;
			m_head = nullptr;
			m_tail = nullptr;
			delete p;
			m_size--;
		}

		return true;
	}


}

#endif
