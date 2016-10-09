#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <iostream>
#include <string>

using namespace std;

class Entry {
public:
	Entry();
	Entry(string protocol, string line);

	~Entry();

	void fetchLine(string protocol, string line);
	void printLineDebug();
	void parseLine();

private:
	string line;
	int fetched;
	int parsed;

	int sl;
	string protocol;
	string local_addr;
	string local_port;
	string remote_addr;
	string remote_port;
	int inode;

	void setDefault();
};

#endif