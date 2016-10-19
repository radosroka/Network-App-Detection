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

	string options[3];

	enum {
		PARAM_S,
		PARAM_I,
		PARAM_F
	};

	std::vector<string> filters;

	string getProtocol(string file);
	void parseFiles();
	void parseFilter();
};

#endif