#include <cstdlib> 
#include <iostream>
#include <string>
#include <map>
using namespace std;

enum ArgumentType {
	ARG_TEXT,
	ARG_SEED,
	ARG_SCRIPT,
	ARG_LEVEL,
	ARG_HELP
};

const map<string, ArgumentType> mode = {
	{"-text", ARG_TEXT},
	{"-seed", ARG_SEED},
	{"-scriptfile", ARG_SCRIPT},
	{"-startlevel", ARG_LEVEL},
	{"-help", ARG_HELP}
};

const string gameExecName = "quadris";

/* Overview
   ___                        _          _       
  / _ \   _   _    __ _    __| |  _ __  (_)  ___ 
 | | | | | | | |  / _` |  / _` | | '__| | | / __|
 | |_| | | |_| | | (_| | | (_| | | |    | | \__ \
  \__\_\  \__,_|  \__,_|  \__,_| |_|    |_| |___/
                                                 
*/

// formatted with special character fix: "\" => "\\"
// DO NOT CHANGE !!!
const string gameSymbol = 
"   ___                        _          _       \n"
"  / _ \\   _   _    __ _    __| |  _ __  (_)  ___ \n"
" | | | | | | | |  / _` |  / _` | | '__| | | / __|\n"
" | |_| | | |_| | | (_| | | (_| | | |    | | \\__ \\\n"
"  \\__\\_\\  \\__,_|  \\__,_|  \\__,_| |_|    |_| |___/\n";

void printHelp(bool verbose) {
	cout << endl;
	
	if(verbose) {
		cout << gameSymbol << endl;
		cout << "\nWelcome to Quadris game\n" << endl;
		cout << "Created by Saidi Tang, Lucy Wu, and Haoyang Qi\n"
		     << "University of Waterloo, 2019\n"
		     << endl;
	}
	
	cout << "Usage:\n"
	     << "  " << gameExecName << " [-option] [<argument>]\n"
	     << endl;
	
	cout << "Options:\n"
	     << "  -text                 Start game in text-only mode\n"
	     << "  -seed <number>        Set random seed for random generator\n"
	     << "  -scriptfile <file>    Load a script to drive the game\n"
	     << "  -startlevel <number>  Set the start level [default: 0]\n"
	     << "  -help                 Print verbose help message\n"
	     << endl;
}

int main(int argc, char *argv[]) {
	string cmd;
	
	for(int opt=1; opt<argc; opt++) {
		cmd = argv[opt];
		
		if(mode.count(cmd) <= 0) {
			// error: Unrecognized commands
			cout << "Error: unrecognized argument: \"" << cmd << "\"." << endl;
			printHelp(false);
			return 1;
		}
		
		switch(mode.at(cmd)) {
			case ARG_TEXT:
				// TODO: set to text mode
				break;
			case ARG_SEED:
				// TODO: read one more, set rand seed
				if(opt != argc-1) {
					srand((unsigned)0);
				}
				// error otherwise: missing argument
				break;
			case ARG_SCRIPT:
				// TODO: read one more, set istream from cin to ifstream
				if(opt != argc-1) {
					
				}
				// error otherwise: missing argument
				break;
			case ARG_LEVEL:
				// TODO: read one more, set level
				if(opt != argc-1) {
					
				}
				// error otherwise: missing argument
				break;
			case ARG_HELP:
				printHelp(true);
				break;
			default:
				// deprecated arguments will be ignored (i.e. do nothing)
				cout << "Deprecated argument: " << cmd << endl;
				break;
		}
	}
	
	return 0;
}
