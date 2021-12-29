#pragma once
#include <iostream>

template<class T>
class Vector
{
private:
	T* vector;
	size_t size;
	size_t capacity;
	void erase();
	void copy(const Vector<T>&);
	void resize();
public:
	Vector();
	Vector(const Vector<T>&);
	Vector<T>& operator=(const Vector<T>&);
	~Vector();

	size_t get_size() const;
	void push_back(const T&);
	void pop_back();
	void push_front(const T&);
	void pop_front();
	void push_at(const size_t&, const T&);
	void pop_at(const size_t&);
	void clear();
	void print() const;
	void reverse();
	bool empty() const;
	bool contains_another(const Vector<T>&) const;

	T& operator[](const size_t&) const;
	bool operator!=(const char*) const;

	friend std::ostream& operator<<(std::ostream& out, const Vector<T>& vector)
	{
		for (size_t i = 0; i < vector.size; i++)
			out << vector.vector[i] << std::endl;
		return out;
	}
};

template<class T>
void Vector<T>::erase()
{
	delete[] this->vector;
}

template<class T>
void Vector<T>::copy(const Vector<T>& other)
{
	this->size = other.size;
	this->capacity = other.capacity;
	this->vector = new T[this->capacity];
	for (size_t i = 0; i < this->size; i++)
		this->vector[i] = other.vector[i];
}

template<class T>
void Vector<T>::resize()
{
	this->capacity *= 2;

	T* biggerVector = new T[this->capacity];
	for (size_t i = 0; i < this->size; i++)
		biggerVector[i] = this->vector[i];

	delete[] this->vector;
	this->vector = biggerVector;
}

template<class T>
Vector<T>::Vector()
{
	this->size = 0;
	this->capacity = 10;
	this->vector = new T[this->capacity];
}

template<class T>
Vector<T>::Vector(const Vector<T>& other)
{
	this->copy(other);
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this != &other)
	{
		this->erase();
		this->copy(other);
	}
	return *this;
}

template<class T>
Vector<T>::~Vector()
{
	this->erase();
}

template<class T>
size_t Vector<T>::get_size() const
{
	return this->size;
}

template<class T>
void Vector<T>::push_back(const T& element)
{
	if (this->size >= this->capacity)
		this->resize();

	this->vector[this->size++] = element;
}

template<class T>
void Vector<T>::pop_back()
{
	this->pop_at(this->size - 1);
}

template<class T>
void Vector<T>::push_front(const T& element)
{
	this->push_at(0, element);
}

template<class T>
void Vector<T>::pop_front()
{
	this->pop_at(0);
}

template<class T>
void Vector<T>::push_at(const size_t& index, const T& element)
{
	if (index > this->size)
		return;

	if (this->size >= this->capacity)
		this->resize();

	for (size_t i = this->size; i > index; i--)
		this->vector[i] = this->vector[i - 1];

	this->vector[index] = element;
	this->size++;
}

template<class T>
void Vector<T>::pop_at(const size_t& index)
{
	if (index >= this->size)
		return;

	T* temp = new T[this->size - 1];
	for (size_t i = index; i < this->size; i++)
		if (i + 1 < this->size)
			this->vector[i] = this->vector[i + 1];

	for (size_t i = 0; i < this->size - 1; i++)
		temp[i] = this->vector[i];

	delete[] this->vector;
	this->vector = temp;
	this->size--;
}

template<class T>
void Vector<T>::clear()
{
	this->size = 0;
	this->capacity = 10;
	this->erase();
	this->vector = new T[capacity];
}

template<class T>
void Vector<T>::print() const
{
	for (size_t i = 0; i < this->size; ++i)
		std::cout << this->vector[i] << " ";
}

template<class T>
void Vector<T>::reverse()
{
	if (this->size == 0)
		return;

	T* temp = new T[this->size];
	size_t k = 0;
	for (int i = size - 1; i >= 0; i--)
		temp[k++] = this->vector[i];

	this->erase();
	this->vector = temp;
}

template<class T>
bool Vector<T>::empty() const
{
	return this->size == 0;
}

template<class T>
bool Vector<T>::contains_another(const Vector<T>& other) const
{
	size_t sizeOther = other.get_size();
	bool flag;
	for (size_t i = 0; i < sizeOther; i++)
	{
		for (size_t j = 0; j < this->size; j++)
		{
			if (other[i] == this->vector[j])
			{
				flag = true;
				break;
			}
			else
				flag = false;
		}
		if (!flag)
			return false;
	}
	return true;
}

template<class T>
T& Vector<T>::operator[](const size_t& index) const
{
	if (index >= this->size)
		return this->vector[this->size - 1];

	return this->vector[index];
}

template<class T>
bool Vector<T>::operator!=(const char* name) const
{
	if (this->size != strlen(name))
		return true;

	for (size_t i = 0; i < this->size; i++)
		if (this->vector[i] == name[i])
			return false;
	return true;
}