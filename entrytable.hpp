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

	void swap();
	void tmpClear();
	void compare();

	void printFormat(std::vector<string> &filter, std::map<long, string> &sockets, string ip);
	void printAllDump();

	void sendSyslog(string msg, string ip);
private:
	vector<Entry*> *table;
	vector<Entry*> *tmp_table;
};

#endif