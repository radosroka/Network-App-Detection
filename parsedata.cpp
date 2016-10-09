#include <iostream>
#include <fstream>
#include <string>

#include "entrytable.hpp"
#include "parsedata.hpp"

using namespace std;

string getProtocol(string file) {
	if (file.length() < 5) runtime_error(string(__func__) + string(": short string"));
	int start = file.find_last_of('/', file.length()) + 1;
	return file.substr(start, 3);
}

void parseFiles(string files[], size_t size, EntryTable& table) {
	for (size_t i = 0; i < size; i++) {
		ifstream file;
		char buffer[BUFFER_SIZE];
		file.open(files[i]);

		int header = 1;
		while (1) {
			file.getline(buffer, BUFFER_SIZE);
			if (header == 1) {
				header = 0;
				continue;
			}

			if (file.eof()) break;
			table.addRecord(getProtocol(files[i]), string(buffer));
		}
		file.close();
	}
}