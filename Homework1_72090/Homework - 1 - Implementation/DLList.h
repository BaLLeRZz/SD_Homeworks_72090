#pragma once
#include<iostream>

template<class T>
struct ListBox
{
	T data{};
	ListBox* next{};
	ListBox* previous{};
	ListBox() = default;
	ListBox(const T& data) : data(data), next(nullptr), previous(nullptr) {};
};

template<class T>
class DLList
{
private:
	ListBox<T>* first{};
	ListBox<T>* last{};
	size_t size{};
	void copy(const DLList<T>& other);
	void erase();
public:
	DLList() = default;
	DLList(const DLList<T>& other);
	DLList<T>& operator=(const DLList<T>& other);
	~DLList();

	void push_back(const T& element);
	void pop_back();
	void push_front(const T& element);
	void pop_front();
	void push_at(int index, const T& element);
	void pop_at(size_t index);
	T& get_first() const;
	T& get_last() const;
	size_t get_size() const;

	T& operator[](const size_t index) const;

	friend std::ostream& operator<<(std::ostream& out, const DLList<T>& list)
	{
		ListBox<T>* current = list.first;
		while (current)
		{
			std::cout << current->data << " ";
			current = current->next;
		}
		return out;
	}
};

template<class T>
void DLList<T>::copy(const DLList<T>& other)
{
	if (!other.first)
		return;

	this->size = other.size;
	this->first = new ListBox<T>(other.first->data);
	ListBox<T>* current = new ListBox<T>(other.first);
	while (current)
	{
	    this->last = current;
	    this->last->next = current;
		current = current->next;
	}
}

template<class T>
void DLList<T>::erase()
{
	while (this->first)
	{
		ListBox<T>* temp = this->first;
		this->first = this->first->next;
		delete temp;
	}
}

template<class T>
DLList<T>::DLList(const DLList<T>& other)
{
	this->copy(other);
}

template<class T>
DLList<T>& DLList<T>::operator=(const DLList<T>& other)
{
	if (this != &other)
	{
		this->erase();
		this->copy(other);
	}
	return *this;
}

template<class T>
DLList<T>::~DLList()
{
	this->erase();
}

template<class T>
void DLList<T>::push_back(const T& element)
{
	this->size++;
	if (!this->first)
	{
		this->first = this->last = new ListBox<T>(element);
		return;
	}

	ListBox<T>* newElement = new ListBox<T>(element);
	newElement->previous = this->last;
	this->last->next = newElement;
	this->last = newElement;
}

template<class T>
void DLList<T>::pop_back()
{
	if (this->size == 0)
		return;

	if (this->size == 1)
	{
		this->size--;
		delete this->last;
		this->last = this->first = nullptr;
		return;
	}

	this->size--;
	this->last->next = this->last;
	this->last = this->last->previous;
	delete this->last->next;
	this->last->next = nullptr;
}

template<class T>
void DLList<T>::push_front(const T& element)
{
	this->size++;
	if (!this->first)
	{
		this->first = this->last = new ListBox<T>(element);
		return;
	}

	ListBox<T>* newElement = new ListBox<T>(element);
	newElement->next = this->first;
	this->first = newElement;
	ListBox<T>* current = this->first;
	while (current->next)
	{
		ListBox<T>* temp = current->next;
		if (!temp->next)
		{
			this->last->previous = current;
			this->last = temp;
			this->last->next = nullptr;
			return;
		}
		current = current->next;
	}
}

template<class T>
void DLList<T>::pop_front()
{
	if (this->size == 0)
		return;
	
	this->size--;
	ListBox<T>* temp = this->first;
	this->first = this->first->next;
	delete temp;
}

template<class T>
void DLList<T>::push_at(int index, const T& element)
{
	if (index < 0 || index > this->size)
		return;

	if (index == this->size)
	{
		this->push_back(element);
		return;
	}

	if (index == 0)
	{
		this->push_front(element);
		return;
	}

	ListBox<T>* current = this->first;
	while (index > 0)
	{
		current = current->next;
		index--;
	}
	ListBox<T>* newElement = new ListBox<T>(element);
	newElement->previous = current->previous;
	newElement->next = current;
	current->previous = newElement;
	ListBox<T>* temp = newElement->previous;
	temp->next = newElement;
}

template<class T>
void DLList<T>::pop_at(size_t index)
{
	if (index > this->size - 1)
		return;

	if (index == 0)
	{
		this->pop_front();
		return;
	}

	if (index == this->size - 1)
	{
		this->pop_back();
		return;
	} 

	ListBox<T>* current = this->first;
	while (index > 0)
	{
		current = current->next;
		index--;
	}
	ListBox<T>* temp = current->previous;
	temp->next = current->next;
	temp = current->next;
	temp->previous = current->previous;
	temp = current;
	delete temp;
}

template<class T>
T& DLList<T>::get_first() const
{
	return this->first->data;
}

template<class T>
T& DLList<T>::get_last() const
{
	return this->last->data;
}

template<class T>
size_t DLList<T>::get_size() const
{
	return this->size;
}

template<class T>
T& DLList<T>::operator[](const size_t index) const
{
	ListBox<T>* current = this->first;
	for (size_t i = 0; i < index; i++)
		current = current->next;

	return current->data;
}
