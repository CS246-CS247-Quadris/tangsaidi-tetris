#include <iostream>
#include <string>
#include <map>
using namespace std;

enum ArgumentType {
	ARG_TEXT,
	ARG_SEED,
	ARG_SCRIPT,
	ARG_LEVEL
};

const map<string, ArgumentType> mode = {
	{"-text", ARG_TEXT},
	{"-seed", ARG_SEED},
	{"-scriptfile", ARG_SCRIPT},
	{"-startlevel", ARG_LEVEL}
};

int main(int argc, char *argv[]) {
	string cmd;
	
	for(int opt=1; opt<argc; opt++) {
		cmd = argv[opt];
		
		switch(mode[cmd]) {
			case ARG_TEXT:
				// TODO: set to text mode
				break;
			case ARG_SEED:
				// TODO: read one more, set rand seed
				if(opt != argc-1) {
					
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
			default:
				// error: unrecognized command
				break;
		}
	}
	
	return 0;
}
