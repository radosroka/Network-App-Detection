#include <iostream>
#include <string>
#include <stdexcept>

#include "entry.hpp"

using namespace std;

Entry::Entry() {
	this->line = "";
	this->fetched = 0;
	this->sl = "";
	string local_addr = "";
	string local_port = "";
	int inode = 0;
}

Entry::~Entry() {

}

void Entry::fetchLine(string line) {
	this->line = line;
	this->fetched = 1;
}

void Entry::parseLine(){
	if(!this->fetched) runtime_error(string(__func__) + string(": already fetched"));
}