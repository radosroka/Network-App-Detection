#ifndef ENTRYTABLE_HPP
#define ENTRYTABLE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "entry.hpp"

using namespace std;

class EntryTable {
public:
	EntryTable();
	~EntryTable();

	void addRecord(string protocol, string line);
	void printFormat(std::vector<string> &filter, std::map<long, string> &sockets);
	void printAllDump();
private:
	vector<Entry*> table;
};

#endif