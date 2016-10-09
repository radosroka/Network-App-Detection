#ifndef PARSEDATA_HPP
#define PARSEDATA_HPP

#include <iostream>
#include <fstream>
#include <string>

#include "entrytable.hpp"

using namespace std;

#define BUFFER_SIZE 2048

string getProtocol(string file);
void parseFiles(string files[], size_t size, EntryTable& table);

#endif