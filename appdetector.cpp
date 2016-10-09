#include <iostream>

#include "entry.hpp"
#include "entrytable.hpp"
#include "parsedata.hpp"

using namespace std;

#define INPUT_FILES_COUNT 4
string input_files [] = {
	"/proc/net/udp",
	"/proc/net/udp6",
	"/proc/net/tcp",
	"/proc/net/tcp6"
};

int main(){
	EntryTable table;
	parseFiles(input_files, INPUT_FILES_COUNT, table);
	table.printAllDump();
	return 0;
}
