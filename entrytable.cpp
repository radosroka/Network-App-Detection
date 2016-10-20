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

void EntryTable::printFormat(std::vector<string> &filter, std::map<long, string> &sockets) {
	for (Entry * entry : this->table) {
		map<long, string>::iterator it;
		string app;

		it = sockets.find(entry->getInode());
		if (it == sockets.end())
			continue;

		app = it->second;

		int match = 0;
		for (std::vector<string>::iterator i = filter.begin(); i != filter.end(); i++) {
			if (*i == app) {
				match = 1;
				break;
			}
		 }

		if (match) {
			if (entry->getProtocol() != "tcp" || 
				entry->getState() == Entry::TCP_ESTABLISHED ||
				entry->getState() == Entry::TCP_CLOSE_WAIT) {
				string out = entry->printLineFormat() + app;
				cout << out << endl;
				//syslog
			}
		}
	}
}

void EntryTable::printAllDump() {
	for (Entry * entry : this->table) {
		entry->printLineDebug();
	}
}