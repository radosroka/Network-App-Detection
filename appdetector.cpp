#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include <map>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

	if (argc < ARGS)
		throw runtime_error(string(__func__) + string(": args problem"));

	for (int i = 0; i < 3; i++) this->options[i] = "";

	string options [] {
		"-s",
		"-i",
		"-f"
	};

	int test = 0;
	for (int i = 1; i < argc; i++) {
		test = 0;
		for (int j = 0; j < 3; j++) {
			if ((i < argc - 1) && (options[j] == argv[i])) {
				this->options[j] = argv[++i];
				test = 1;
				if (j == this->PARAM_F) {
					for (i++; i < argc; i++) {
						if (argv[i][0] == '-') {
							i--;
							break;
						}
						this->options[j] += argv[i];
					}
				}
				
			}
		}

		if (!test)
			throw runtime_error(string(__func__) + string(": bad params"));
	}

	for (int i = 0; i < 3; i++)
		if (this->options[i] == "")
	 		throw runtime_error(string(__func__) + string(": some parameter is missing"));

	//for (int i = 0; i < 3; i++) cout << this->options[i] << endl;	

	this->parseFilter();
	this->fillMap();
	this->printSockets();

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

void AppDetector::parseFilter() {	

	string delim = ",";
	string str = this->options[this->PARAM_F];
	vector<string> tokens;

	size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    this->filters = tokens;
}

void AppDetector::printAllDump() {
	this->table.printAllDump();
}

void AppDetector::fillMap() {
	string root = "/proc";
	const unsigned BUFF = 2048;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(root.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			size_t pos = 0;
			string name = ent->d_name;
			unsigned long num;
			try {
					num = stol(name, &pos, 10);
				
				if (num != name.npos) {
					DIR * subdir;
					string path = root + "/" + name + "/fd";
					if ((subdir = opendir (path.c_str())) != NULL) {
						struct dirent *fd;
						while ((fd = readdir (subdir)) != NULL) {
							if (string(fd->d_name) == "." || string(fd->d_name) == "..") continue;
							string path_ = path + "/" + string(fd->d_name);
							char buff[BUFF];
							readlink(path_.c_str(), buff, BUFF);
							string b = buff;
							size_t ret = b.find("socket");
							if (ret == b.npos) continue;
							long inode = stol(b.substr(8), nullptr, 10);

							string cmdline;
							fstream f;
							f.open(root + "/" + name + "/" + "cmdline", std::fstream::in);
							f >> cmdline;
							f.close();
							string cmd;
							size_t p = cmdline.find_last_of("/");
							if (p != cmdline.npos)
								cmd = cmdline.substr(p+1);
							else
								cmd = cmdline;

							this->sockets.insert(std::pair<long, string>(inode, cmd));
						}
						closedir (subdir);
					}
				}

			} catch(const exception &e) {
				continue;
			}
			
		}
		closedir (dir);
	}
}

void AppDetector::printSockets() {

	for (std::map<long, string>::iterator i = this->sockets.begin(); i != this->sockets.end(); i++) {
		cout << i->first << " -> " << i->second << endl;
	}
}

int main(int argc, char **argv){

	try {
		AppDetector appdetector(argc, argv, INPUT_FILES_COUNT, input_files);
		//appdetector.printAllDump();
	} catch (const runtime_error &error) {
		cout << error.what() << endl;
		return 1;
	}
	return 0;
}
