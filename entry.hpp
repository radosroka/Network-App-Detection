#include <iostream>
#include <string>

using namespace std;

class Entry {
public:
	Entry();
	~Entry();

	void fetchLine(string line);
	void parseLine();
private:
	string line;
	int fetched;
	int parsed;

	string sl;
	string local_addr;
	string local_port;
	int inode;
};