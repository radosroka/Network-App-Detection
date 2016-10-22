#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

#include "entrytable.hpp"

using namespace std;

EntryTable::EntryTable() {
	this->table = new vector<Entry*>();
	this->tmp_table = new vector<Entry*>();
}

EntryTable::~EntryTable() {
	for (std::vector<Entry*>::iterator i = this->table->begin(); i != this->table->end(); i++) {
		delete *i;
	}

	for (std::vector<Entry*>::iterator i = this->tmp_table->begin(); i != this->tmp_table->end(); i++) {
		delete *i;
	}

	delete this->table;
	delete this->tmp_table;
}

void EntryTable::addRecord(string protocol, string line){
	this->tmp_table->push_back(new Entry(protocol, line));
}

void EntryTable::swap() {
	this->table->swap(*this->tmp_table);
}

void EntryTable::tmpClear() {
	for (std::vector<Entry*>::iterator i = this->tmp_table->begin(); i != this->tmp_table->end(); i++) delete *i;
	this->tmp_table->clear();
}

void EntryTable::compare() {
	for (std::vector<Entry*>::iterator i = this->tmp_table->begin(); i != this->tmp_table->end(); i++) {
		for (std::vector<Entry*>::iterator j = this->table->begin(); j != this->table->end(); j++) {
			if ((*i)->equal(**j)) {
				if ((*j)->wasPrinted()) (*i)->printed();
				break;
			}
		}
	}
}

void EntryTable::printFormat(std::vector<string> &filter, std::map<long, string> &sockets, string ip) {
	for (Entry * entry : *this->table) {
		map<long, string>::iterator it;
		string app;

		if (entry->wasPrinted()) continue;

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

		if (filter[0] == "*") match = 1;

		if (match) {
			if (entry->getProtocol() != "tcp" || 
				entry->getState() == Entry::TCP_ESTABLISHED ||
				entry->getState() == Entry::TCP_CLOSE_WAIT) {
				string out = entry->printLineFormat() + app;
				cout << out << endl;
				this->sendSyslog(out, ip);
				entry->printed();
			}
		}
	}
}

void EntryTable::printAllDump() {
	for (Entry * entry : *this->table) {
		entry->printLineDebug();
	}
}

void EntryTable::sendSyslog(string msg, string ip) {
	msg += "\n";

	struct sockaddr_in server;
	int sck_fd;
	const int PORT = 514;

	if ((sck_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		throw runtime_error("cannot create socket");

	memset((char *) &server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
     
    if (inet_aton(ip.c_str() , &server.sin_addr) == 0)
    	throw runtime_error("bad ip address");
 
    if (sendto(sck_fd, msg.c_str(), msg.length(), 0, (struct sockaddr*)&server, sizeof(server)) == -1)
    	throw runtime_error("cannot send data to udp server");

    close(sck_fd);

}