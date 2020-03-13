#pragma once
#include <stdlib.h>
#include <string>
#include <exception>

namespace myHash
{
	constexpr size_t TABLE_SIZE = 10;

	template<typename Key> //, size_t size_of_array_in_map=10>
	struct MyHashCalculator {
	public:
		size_t operator()(Key key)
		{
			return key % TABLE_SIZE;//size_of_array_in_map;
		}
	};

	template<>
	struct MyHashCalculator<std::string>
	{
	public:
		size_t operator()(std::string key)
		{
			throw std::exception("Not Implemented!");

		}
	};
}