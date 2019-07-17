#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>

#include <QtWidgets>
#include <QScopedPointer>
#include <QCoreApplication>
#include <QApplication>
// #include <QSocketNotifier>


#include "window.h"

#include "game.h"
#include "textGame.h"
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
	     << "  -text                 Start game in text-only mode [default: false]\n"
	     << "  -seed <number>        Set random seed for random generator [default: 0]\n"
	     << "  -scriptfile <file>    Load a block sequence to drive the game [default: sequence.txt]\n"
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

// Need unsigned integer for seed and level (0~4), so checking in this way will suffice
bool isNumber(const string& s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

// create a version based on args
QCoreApplication* createApplication(int &argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
        if (!qstrcmp(argv[i], "-text"))
            return new QCoreApplication(argc, argv);
    return new QApplication(argc, argv);
}

int main(int argc, char *argv[]) {
	// Argument parsing
	string cmd, str_seed="0", str_level="0";
	// Configuration variables
	int startLevel=0;
	bool enableTextMode=false; 
	string fScript;
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
				enableTextMode = true;
				break;
			case ARG_SEED:
				if(opt != argc-1) {
					str_seed = argv[++opt];
					if(!isNumber(str_seed)) {
						printErrorMessage("-seed", str_seed + " is not a legal number.");
						return 3;
					}
				}
				else {
					printErrorMessage(cmd, "Missing argument, expect number.");
					return 2;
				}
				break;
			case ARG_SCRIPT:
				if(opt != argc-1) {
					fScript = argv[++opt];
				}
				else {
					printErrorMessage(cmd, "Missing argument, expect string.");
					return 2;
				}
				break;
			case ARG_LEVEL:
				if(opt != argc-1) {
					str_level = argv[++opt];
					if(!isNumber(str_level)) {
						printErrorMessage("-startlevel", str_level + " is not a legal number.");
						return 3;
					}
				}
				else {
					printErrorMessage(cmd, "Missing argument, expect number [0~4].");
					return 2;
				}
				break;
			case ARG_HELP:
				// -help option takes highest priority and discard rest if there is any
				printHelp(true);
				return 0;
			default:
				// deprecated arguments will be ignored (i.e. do nothing)
				cout << "Deprecated argument: " << cmd << endl;
				break;
		}
	}

    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));

	/* Now process configurations */
	// TODO: might cause exception
	startLevel = stoi(str_level);
	// Now start the game
	game = std::make_unique<Game>(startLevel, stoi(str_seed), fScript);

	TextGame textGame(game.get());
	textGame.run();
	QObject::connect(&textGame, SIGNAL(quit()), &(*app), SLOT(quit()));

	unique_ptr<Window> window;
	if (!enableTextMode) {
		window = make_unique<Window>(game.get());
		QObject::connect(&textGame, SIGNAL(updateWindow()), window.get(), SLOT(updateWindow()));
		window->show();
	}
	
	return app->exec();
}
