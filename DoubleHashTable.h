#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

#include "Exception.h"

enum state { EMPTY, OCCUPIED, DELETED };

template<typename T>
class DoubleHashTable {
private:
	int count;
	int power;
	int array_size;
	T *array;
	state *array_state;
	int M;

	int h1(T const &) const; // first hash function
	int h2(T const &) const; // second hash function
	int hT(T const &) const; // h1+ih2 hash function
	int findObjectIndex(T const &) const;

public:
	DoubleHashTable(int = 5);
	~DoubleHashTable();
	int size() const;
	int capacity() const;
	bool empty() const;
	bool member(T const &) const;
	T bin(int) const;

	void print() const;

	void insert(T const &);
	bool remove(T const &);
	void clear();
};

template<typename T >
DoubleHashTable<T >::DoubleHashTable(int m) :
	count(0), power(m),
	array_size(1 << power),
	array(new T[array_size]),
	array_state(new state[array_size]) {
	M = array_size;

	for (int i = 0; i < array_size; ++i) {
		array_state[i] = EMPTY;
	}
}

template<typename T >
DoubleHashTable<T >::~DoubleHashTable() {
	delete[] array;
	delete[] array_state;
}

template<typename T >
int DoubleHashTable<T >::size() const {
	return count;
}

template<typename T >
int DoubleHashTable<T >::capacity() const {
	return array_size;
}


template<typename T >
bool DoubleHashTable<T >::empty() const {
	if (count != 0) {
		return false;
	}
	else {
		return true;
	}
}

template<typename T >
int DoubleHashTable<T >::h1(T const &obj) const {
	//h1=kmodm
	//int foo = (int) obj;
	//double foo= obj+0.5;
	//int foo2 = (int)foo;
	//int result = (int)obj%M;
	int result = (int)obj%M;
	return result;
}

template<typename T >
int DoubleHashTable<T >::h2(T const &obj) const {
	if (obj < 0) {
		result += M;
	}
	if (result % 2 == 0) {//if h2 even
		result++;
	}
	return result;
}

template<typename T >
int DoubleHashTable<T >::hT(T const &obj) const {
	int index = h1(obj);
	int i = 1;
	int j;
	while ((array_state[index]==OCCUPIED) && (i<=array_size)) {
		index = (h1(obj) + i*h2(obj))%M;
		i++;
		j = i;
		if ((i == array_size)&& (array_state[index] == OCCUPIED)) {
			throw overflow();
		}
	}
	return index;
}

//Finds the index of the 
template<typename T >
int DoubleHashTable<T >::findObjectIndex(T const &obj) const {
	int index = h1(obj);
	int i = 1;
	if (array[index] != obj) {//If h1(obj) does not return the correct index
		while ((array[index]!= obj)&&(i<=array_size)) {
			index = (h1(obj) + i*h2(obj))%M;
			i++;
			if (array_state[index] == EMPTY) {
				break;
			}
		}
		if (array[index] != obj) {
			return -1;
		}
	}
	return index;
}

template<typename T >
bool DoubleHashTable<T >::member(T const &obj) const {
	int index = h1(obj);
	int i = 1;
	if (array[index] != obj) {//If h1(obj) does not return the correct index
		while ((array[index] != obj)&&(i <= M)) {
			index = (h1(obj) + i*h2(obj))%M;
			i++;
		}
		//The loop did not find obj in t<m hence object does not exist
		if (array[index] != obj) {
			return false;
		}
	}
	if (array_state[index] == OCCUPIED) {
		return true;
	}
	else {
		return false;
	}
}

template<typename T >
T DoubleHashTable<T >::bin(int n) const {
	return array[n];
}

template<typename T >
void DoubleHashTable<T >::insert(T const &obj) {
	int index = h1(obj);
	if (array_state[index]!=OCCUPIED) {
		array[index] = obj;
		array_state[index]=OCCUPIED;
		count++;
	}
	//If h1 occupied, use index of hT=h1+ih2
	else if(count<array_size) {
		index = hT(obj);
		if (index < 0) {
			throw overflow();
		}
		array[index] = obj;
		array_state[index]=OCCUPIED;
		count++;
	}
	//If array is full
	else {
		throw overflow();
	}
	return;
}

template<typename T >
bool DoubleHashTable<T >::remove(T const &obj) {
	int index;
	index = findObjectIndex(obj);
	if (index != -1) {
		if (array_state[index] == OCCUPIED) {
			array_state[index] = DELETED;
			count--;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

template<typename T >
void DoubleHashTable<T >::clear() {
	for (int i=0; i<M; i++) {
		if (array_state[i] == OCCUPIED) {
			array_state[i] = DELETED;
		}
	}
	count = 0;
	return;
}

template<typename T >
void DoubleHashTable<T >::print() const {
	std::cout <<"EMPTY=0  ::  OCCUPIED=1 :: DELETED=2"<< std::endl;
	for (int i = 0; i < array_size; i++) {
		std::cout << i << ":|" << array[i] << "|" << array_state[i] << std::endl;
	}
	return;
}

#endif