#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "entrytable.hpp"

using namespace std;

EntryTable::EntryTable() {}

EntryTable::~EntryTable() {}

void EntryTable::addRecord(string protocol, string line){
	this->table.push_back(new Entry(protocol, line));
}

void EntryTable::printAllDump() {
	for (Entry * entry : this->table) {
		entry->printLineDebug();
	}
}