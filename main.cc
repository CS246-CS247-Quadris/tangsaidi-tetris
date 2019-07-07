#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "game.h"
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
// DO NOT MODIFY !!!
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

void printErrorMessage(string option, string msg, bool needHelp=true) {
	cout << "Error at option \"" << option << "\": " << endl;
	cout << "  " << msg << endl;
	
	if(needHelp)
		printHelp(false);
}

bool isNumber(const string& s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int main(int argc, char *argv[]) {
	// Argument parsing
	string cmd;
	map<ArgumentType, string> argument = {
		{ARG_SEED, "0"},
		{ARG_LEVEL, "0"}
	};
	// Configuration variables
	int startLevel=0;
	ifstream fs;
	bool enableTextMode=false; 
	// Game interface
	unique_ptr<Game> game;
	
	for(int opt=1; opt<argc; opt++) {
		cmd = argv[opt];
		
		if(mode.count(cmd) <= 0) {
			printErrorMessage(cmd, "Unrecognized argument.");
			return 1;
		}
		
		switch(mode.at(cmd)) {
			case ARG_TEXT:
				argument[ARG_TEXT] = "";
				break;
			case ARG_SEED:
				if(opt != argc-1) {
					cmd = argv[++opt];
					argument[ARG_SEED] = cmd;
				}
				else {
					printErrorMessage(cmd, "Missing argument, expect number.");
					return 2;
				}
				break;
			case ARG_SCRIPT:
				if(opt != argc-1) {
					cmd = argv[++opt];
					argument[ARG_SCRIPT] = cmd;
				}
				else {
					printErrorMessage(cmd, "Missing argument, expect string.");
					return 2;
				}
				break;
			case ARG_LEVEL:
				if(opt != argc-1) {
					cmd = argv[++opt];
					argument[ARG_LEVEL] = cmd;
				}
				else {
					printErrorMessage(cmd, "Missing argument, expect number.");
					return 2;
				}
				break;
			case ARG_HELP:
				argument[ARG_HELP] = "";
				break;
			default:
				// deprecated arguments will be ignored (i.e. do nothing)
				cout << "Deprecated argument: " << cmd << endl;
				break;
		}
	}
	
	/* Now process configurations */
	// -help option takes highest priority and discard rest if there is any
	if(argument.count(ARG_HELP) > 0) {
		printHelp(true);
		return 0;
	}
	else {
		string str_seed = argument.at(ARG_SEED);
		string str_level = argument.at(ARG_LEVEL);
		string str_file;
		
		if(!isNumber(str_seed)) {
			printErrorMessage("-seed", str_seed + " is not a number.");
			return 3;
		}
		if(!isNumber(str_level)) {
			printErrorMessage("-startlevel", str_level + " is not a number.");
			return 3;
		}
		
		srand((unsigned int)stoi(str_seed));
		startLevel = stoi(str_level);
		
		if(argument.count(ARG_TEXT) > 0) {
			enableTextMode = true;
		}
		
		if(argument.count(ARG_SCRIPT) > 0) {
			str_file = argument.at(ARG_SCRIPT);
			fs.open(str_file.c_str(), ifstream::in);
			
			if(fs.fail()) {
				printErrorMessage("-scriptfile", "Cannot open script file \""+str_file+"\".", false);
				// TODO: should we terminate?
				return 4;
			}
		}
	}
	
	// Now start the game
//	cout << "Text mode: " << (enableTextMode?"true":"false") << endl;
//	cout << "Start Level: " << startLevel << endl;
//	cout << "Stream: " << (argument.count(ARG_SCRIPT)>0 ? "file" : "standard") << endl;
	game = make_unique<Game>(enableTextMode, startLevel, argument.count(ARG_SCRIPT)>0 ? fs : cin);
	
//	game->parseCommand("123left");
//	game->parseCommand("1left");
//	game->parseCommand("0left");
//	game->parseCommand("left");
//	game->parseCommand("3ri");
//	game->parseCommand("counterc");
//	game->parseCommand("co");
	game->parseCommand();
	
	
	return 0;
}
