#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "board.h"

class Game {
public:
	Game(bool, int, std::istream&);
	void parseCommand(const std::string&);
	
private:
	enum CommandType {
		CONTROL_LEFT = 0,
		CONTROL_RIGHT,
		CONTROL_DOWN,
		CONTROL_CLOCKWISE,
		CONTROL_CC,
		CONTROL_DROP,
		CONTROL_LEVELUP,
		CONTROL_LEVELDOWN,
		CONTROL_RND,
		CONTROL_NORND,
		CONTROL_SEQUENCE,
		CONTROL_RESTART,
		CONTROL_HINT,
		// DEBUG_REPLACE,
		DEBUG_REPLACE_I,
		DEBUG_REPLACE_J,
		DEBUG_REPLACE_L,
		DEBUG_REPLACE_O,
		DEBUG_REPLACE_S,
		DEBUG_REPLACE_Z,
		DEBUG_REPLACE_T,
		COMMAND_RENAME
	};
	
	// Node class for the compressed prefix tree
	class StateNode {
	private:
		StateNode();
	};
	
	std::istream& in;
	std::unique_ptr<Board> game;
	std::map<std::string, CommandType> command;
	std::map<std::string, std::vector<std::string>> macro;
};
