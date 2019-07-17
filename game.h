#ifndef __GAME_H__
#define __GAME_H__

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "board.h"

class Window;
class WindowBoard;

class Game {
	friend class Window;
	friend class WindowBoard;
public:
	Game(int, int, const std::string&);
	~Game();
	void printBoard();
	bool parseCommand(std::istream&);
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
		COMMAND_RENAME,
		COMMAND_AMP
	};
	
	// Node class for the uncompressed prefix tree
	class StateNode {
	public:
		StateNode(const std::string&);
	private:
		//size_t idx;
		std::string strAccept;
		std::vector<std::shared_ptr<StateNode>> next;
		std::map<char, size_t> path;
		
		friend class Game;
	};
	
	// Context variable, used by game main loop
	int seed;
	int startLevel;
	bool isHint;
	const std::string& script;
	
	std::unique_ptr<Board> game;
	std::map<std::string, CommandType> command;
	std::map<std::string, std::vector<std::string>> macro;
	std::shared_ptr<StateNode> prefixTree;
	
	bool ifDuplicated(const std::string&);
	void addCommandPrefixLookup(const std::string&);
	std::string getCommandByPrefix(const std::string&);
	void debugPrintTree(const std::shared_ptr<StateNode>& root, int k=0);
	
	bool test(const std::string&);
	bool perform(const std::vector<std::string>&, int&);
	void splitToken(const std::string&, std::string&, int&);
	void reset();
};

#endif /* __GAME_H__ */
