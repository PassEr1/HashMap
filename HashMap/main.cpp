#include <iostream>
#include <string>
#include "MyGenericHashMap.h"
#include <functional>

using namespace std;

int main()
{
	MyGenericHashMap<int, string, std::hash<size_t>> map;
	cout << to_string(std::numeric_limits<std::size_t>::max() / 100);
	for (size_t i = 0; i < 100; i++)
	{
		map[i] = "number__" + to_string(i);
	}
	
	
	try {
		for (size_t i = 0; i < 101; i++)
		{
			cout << map.get(i) << endl;
		}

	}

	catch (IndexError e)
	{
		cout << e.what();
	}
	map[10] = "NOT 10 ";
	string s = map[10];
	cout << s << endl;

	cout << "Done! \n";
	return 0;
}