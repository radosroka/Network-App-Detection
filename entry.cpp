#include <iostream>
#include <string>
#include <stdexcept>

#include <cctype>

#include "entry.hpp"

using namespace std;

Entry::Entry() {
	this->setDefault();
}

Entry::Entry(string protocol, string line) {
	this->setDefault();
	this->fetchLine(protocol, line);
}

Entry::~Entry() {

}

void Entry::fetchLine(string protocol, string line) {
	this->line = line;
	this->protocol = protocol;
	this->fetched = 1;
	this->parseLine();
}

string Entry::getStateEnum(){

	if (this->protocol == "udp") return "";

	switch(this->state) {
		case TCP_ESTABLISHED:
			return "ESTABLISHED";
		case TCP_SYN_SENT:
			return "SYN_SENT";
		case TCP_SYN_RECV:
			return "SYN_RECV";
		case TCP_FIN_WAIT1:
			return "FIN_WAIT1";
		case TCP_FIN_WAIT2:
			return "FIN_WAIT2";
		case TCP_TIME_WAIT:
			return "TIME_WAIT";
		case TCP_CLOSE:
			return "CLOSE";
		case TCP_CLOSE_WAIT:
			return "CLOSE_WAIT";
		case TCP_LAST_ACK:
			return "LAST_ACK";
		case TCP_LISTEN:
			return "LISTEN";
		case TCP_CLOSING:
			return "CLOSING";
		default:
			return "";
	}
}

void Entry::printLineDebug() {
	cout << this->protocol << " : " << this->line << endl;
	if (this->parsed) {
		cout << "\t" << "selector: " << this->sl << endl;
		cout << "\t" << "local_addr: " << this->local_addr << endl;
		cout << "\t" << "local_port: " << this->local_port << endl;
		cout << "\t" << "remote_addr: " << this->remote_addr << endl;
		cout << "\t" << "remote_port: " << this->remote_port << endl;
		cout << "\t" << "state: " << this->getStateEnum() << endl;
		cout << "\t" << "inode: " << this->inode << endl;
	}
}

#define IPV6_HEXLEN 32
#define IPV4_HEXLEN 8

void reverseIPHexString(string& str) {
	for (size_t i = 0; i < str.length()/2; i += 2)	{
		char tmp = str[i];
		str[i] = str[str.length()-1 - 1 - i];
		str[str.length()-1 - 1 - i] = tmp;

		tmp = str[i+1];
		str[i+1] = str[str.length()-1 - i];
		str[str.length()-1 - i] = tmp;
	}
}

void ipv4HexaToDec(string hexa, string& ipdec) {
	ipdec = "";
	for (int i = 0; i < 4; i++){
		string oct = hexa.substr(2*i, 2);
		int dec = stoi(oct, nullptr, 16);
		ipdec.append(to_string(dec) + ".");
	}
	ipdec.pop_back();
}

void formatIPV6(string ipv6, string& ip) {
	ip = "";
	for (size_t i = 0; i < ipv6.length(); i++) {
		if (((i % 4) == 0) && i != 0) ip.push_back(':');
		ip.push_back(ipv6[i]);
	}
}

void Entry::parseLine(){
	if (!this->fetched) throw runtime_error(string(__func__) + string(": already fetched"));
	this->parsed = 1;

	size_t pos = 0;
	size_t end = 0;
	string hexa = "";

	this->sl = stoi(this->line, &pos);
	pos += 2;

	end = this->line.find_first_of(':', pos);
	hexa = this->line.substr(pos, end - pos);

	reverseIPHexString(hexa);

	if (hexa.length() == IPV4_HEXLEN)
		ipv4HexaToDec(hexa, this->local_addr);
	else if (hexa.length() == IPV6_HEXLEN)
		formatIPV6(hexa, this->local_addr);
	else
		runtime_error(string(__func__) + string(": bad ip"));

	pos = end + 1;

	this->local_port = stoi(this->line.substr(pos, 4), &end, 16);
	pos += 5;

	end = this->line.find_first_of(':', pos);
	hexa = this->line.substr(pos, end - pos);

	reverseIPHexString(hexa);

	if (hexa.length() == IPV4_HEXLEN)
		ipv4HexaToDec(hexa, this->remote_addr);
	else if (hexa.length() == IPV6_HEXLEN)
		formatIPV6(hexa, this->remote_addr);
	else
		throw runtime_error(string(__func__) + string(": bad ip"));

	pos = end + 1;

	this->remote_port = stoi(this->line.substr(pos, 4), &end, 16);
	pos += 5;

	this->state = stoi(this->line.substr(pos, 2), &end, 16);
	pos += 57;

	this->inode = stoi(this->line.substr(pos, 10), &end, 10);

}

string Entry::printLineFormat() {
	string result;

	result =	this->protocol + " " + this->local_addr + " " +
				to_string(this->local_port) + " " + this->remote_addr + " " +
				to_string(this->remote_port) + " ";
	return result;
}

long Entry::getInode() {
	return this->inode;
}

int Entry::getState() {
	return this->state;
}

string Entry::getProtocol() {
	return this->protocol;
}

int Entry::wasPrinted() {
	return this->print;
}

void Entry::printed() {
	this->print = 1;
}

int Entry::equal(Entry &e) {
	if (this->local_addr == e.local_addr &&
		this->local_port == e.local_port &&
		this->remote_addr == e.remote_addr &&
		this->remote_port == e.remote_port) {

		return 1;
	}

	return 0;
}

void Entry::setDefault(){
	this->line = "";
	this->fetched = 0;
	this->parsed = 0;
	this->sl = 0;
	this->local_addr = "";
	this->local_port = 0;
	this->remote_addr = "";
	this->remote_port = 0;
	this->state = -1;
	this->inode = 0;
	this->print = 0;
}