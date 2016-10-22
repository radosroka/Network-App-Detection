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

	int equal(Entry &e);

	void fetchLine(string protocol, string line);
	void printLineDebug();
	void parseLine();
	string printLineFormat();
	long getInode();
	int getState();
	string getProtocol();

	int wasPrinted();
	void printed();

	enum {
    TCP_ESTABLISHED = 1,
    TCP_SYN_SENT,
    TCP_SYN_RECV,
    TCP_FIN_WAIT1,
    TCP_FIN_WAIT2,
    TCP_TIME_WAIT,
    TCP_CLOSE,
    TCP_CLOSE_WAIT,
    TCP_LAST_ACK,
    TCP_LISTEN,
    TCP_CLOSING,   

    TCP_MAX_STATES
	};

private:
	string line;
	int fetched;
	int parsed;

	int sl;
	string protocol;
	string local_addr;
	int local_port;
	string remote_addr;
	int remote_port;
	int state;
	long inode;

	int print;

	void setDefault();
	string getStateEnum();
};

#endif