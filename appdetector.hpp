#ifndef APPDETECTOR_HPP
#define APPDETECTOR_HPP

#define BUFFER_SIZE  2048

class AppDetector {
public:
	AppDetector(int argc, char **argv, uint num_files, string files[]);
	~AppDetector();

	void printAllDump();
	void startDetection();

private:
	EntryTable table;
	string * files;
	uint num_files;

	string sParam;
	string iParam;
	string fParam;

	string getProtocol(string file);
	void parseFiles();
};

#endif