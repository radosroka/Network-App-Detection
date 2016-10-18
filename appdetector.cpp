#include <iostream>
#include <fstream>
#include <stdexcept>

#include <map>

#include "entry.hpp"
#include "entrytable.hpp"
#include "appdetector.hpp"

using namespace std;

#define INPUT_FILES_COUNT 4
string input_files [] = {
	"/proc/net/udp",
	"/proc/net/udp6",
	"/proc/net/tcp",
	"/proc/net/tcp6"
};

#define ARGS 7

AppDetector::AppDetector(int argc, char **argv, uint num_files, string files[]) {
	this->files = files;
	this->num_files = num_files;

	this->parseFiles();

	if (argc != ARGS)
		throw runtime_error(string(__func__) + string(": args problem"));

	int sFlag = 0;
	int iFlag = 0;
	int fFlag = 0;

	for (int i = 1; i < argc; i = i + 2) {
		if (argv[i] == "-s") {
			this->sParam = argv[i + 1];
			sFlag = 1;
		} else if (argv[i] == "-i") {
			this->iParam = argv[i + 1];
			iFlag = 1;
		} else if (argv[i] == "-f") {
			this->fParam = argv[i + 1];
			fFlag = 1;
		}
	}

	if (!sFlag || !iFlag || !fFlag)
		throw runtime_error(string(__func__) + string(": some parameter is missing"));

	

}

AppDetector::~AppDetector(){

}

string AppDetector::getProtocol(string file) {
	if (file.length() < 5) throw runtime_error(string(__func__) + string(": short string"));
	int start = file.find_last_of('/', file.length()) + 1;
	return file.substr(start, 3);
}

void AppDetector::parseFiles() {
	for (size_t i = 0; i < this->num_files; i++) {
		ifstream file;
		char buffer[BUFFER_SIZE];
		file.open(this->files[i]);

		int header = 1;
		while (1) {
			file.getline(buffer, BUFFER_SIZE);
			if (header == 1) {
				header = 0;
				continue;
			}

			if (file.eof()) break;
			this->table.addRecord(getProtocol(this->files[i]), string(buffer));
		}
		file.close();
	}
}

void AppDetector::printAllDump() {
	this->table.printAllDump();
}

int main(int argc, char **argv){

	try {
		AppDetector appdetector(argc, argv, INPUT_FILES_COUNT, input_files);
		appdetector.printAllDump();
	} catch (const runtime_error &error) {
		cout << error.what() << endl;
		return 1;
	}
	return 0;
}
