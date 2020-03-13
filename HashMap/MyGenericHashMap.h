#pragma once
#include <vector>
#include <array>
#include "HashFunctors.h"


using namespace std;

template <typename K, class V, class HashCalculator>
class contextOfMapByKey;

class IndexError :public std::exception
{
public:
	const char* what() const throw ()
	{
		return "Index Error!";
	}

};


template<typename Key, class Value>
class PairOfHashResult {
public:
	PairOfHashResult(Key _key, Value _val)
		:key(_key)
	{
		value = _val;
		nextPairOfSameHash = nullptr;
	}

	const Key key;
	Value value;
	PairOfHashResult* nextPairOfSameHash;
};


template <typename K, class V, class HashCalculator = myHash::MyHashCalculator<K>>
class MyGenericHashMap
{

public:
	MyGenericHashMap()
	{
		//_hashTable = new vector<PairOfHashResult<K, V>*>(myHash::TABLE_SIZE);
		for (size_t i = 0; i < _hashTable.size(); i++)
		{
			_hashTable[i] = nullptr;
		}
	}

	~MyGenericHashMap()
	{
		for (size_t i = 0; i < myHash::TABLE_SIZE; i++)
		{
			PairOfHashResult<K, V>* previous;
			PairOfHashResult<K, V>* pToHash = _hashTable[i];
			while (pToHash != nullptr)
			{
				previous = pToHash;
				pToHash = pToHash->nextPairOfSameHash;
				delete previous;
			}
			
		}
		
	}

	auto operator[](K key)
	{
		return contextOfMapByKey<K, V, HashCalculator>(this, key);
	}

	void put(K key, V value)
	{

		size_t hashResult = _hashCalculator(key); // result is certintly in range of: 0 to TABLE_SIZE
		PairOfHashResult<K, V>* pToListOfColisions;
		PairOfHashResult<K, V>* previousPairWithSameHash = nullptr;
		
		pToListOfColisions = _hashTable[hashResult];
		while ((pToListOfColisions != nullptr)
			&& (pToListOfColisions->key != key))
		{
			previousPairWithSameHash = pToListOfColisions;
			pToListOfColisions = pToListOfColisions->nextPairOfSameHash;
		}

		//cheking what the scan has found
		if (pToListOfColisions == nullptr) //the key is not exists or the list is empty(firs insert for key 'key')
		{
			if (previousPairWithSameHash == nullptr) //it never entered the scan - the list is empty
			{
				_hashTable[hashResult] = new PairOfHashResult<K, V>(key, value);
			}

			else //we got the end
			{
				previousPairWithSameHash->nextPairOfSameHash = new PairOfHashResult<K, V>(key, value);
			}
		}

		else if (pToListOfColisions->key == key) //this key is already exists, we just gonna update the value
		{
			pToListOfColisions->value = value;
		}
	}

	V get(K key)
	{
		size_t hashResult = _hashCalculator(key);
		PairOfHashResult<K, V>* pToListOfColisionKeyHash = _hashTable[hashResult];
		while (pToListOfColisionKeyHash != nullptr)
		{
			if (pToListOfColisionKeyHash->key == key) {
				return pToListOfColisionKeyHash->value;
			}
			else {
				pToListOfColisionKeyHash = pToListOfColisionKeyHash->nextPairOfSameHash;
			}
		}

		throw IndexError();
	}

private:
	std::array<PairOfHashResult<K, V>*, myHash::TABLE_SIZE> _hashTable;
	HashCalculator _hashCalculator;
};



template <typename K, class V, class HashCalculator>
class contextOfMapByKey {
public:
	contextOfMapByKey(MyGenericHashMap< K, V, HashCalculator>* mapToInsertTo, K key)
	{
		_mapContext = mapToInsertTo;
		_key = key;

	}
	void operator= (V valueToInsert)
	{
		_mapContext->put(_key, valueToInsert);
	}

	operator V ()
	{
		return _mapContext->get(_key);
	}

private:
	MyGenericHashMap< K, V, HashCalculator>* _mapContext;
	K _key;

};

