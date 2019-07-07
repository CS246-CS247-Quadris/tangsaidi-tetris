#include "game.h"
using namespace std;

void Game::debugPrintTree(const shared_ptr<StateNode>& root, int k) {
	if(root!=nullptr) {
		for(int i=0;i<k;i++) cout<<" ";
		cout<<"Node("<<root->strAccept<<")"<<endl;
		for(int i=0;i<root->next.size();i++) {
			debugPrintTree(root->next[i], k+1);
		}
	}
}

Game::Game(bool isTextMode, int level, istream& is): 
	in{is}, 
	game{make_unique<Board>(level)}, 
	prefixTree{make_shared<Game::StateNode>("")},
	command{
		{"left", CONTROL_LEFT},
		{"right", CONTROL_RIGHT},
		{"down", CONTROL_DOWN},
		{"clockwise", CONTROL_CLOCKWISE},
		{"counterclockwise", CONTROL_CC},
		{"drop", CONTROL_DROP},
		{"levelup", CONTROL_LEVELUP},
		{"leveldown", CONTROL_LEVELDOWN},
		{"norandom", CONTROL_NORND},
		{"random", CONTROL_RND},
		{"sequence", CONTROL_SEQUENCE},
		{"restart", CONTROL_RESTART},
		{"hint", CONTROL_HINT},
		{"I", DEBUG_REPLACE_I},
		{"J", DEBUG_REPLACE_J},
		{"L", DEBUG_REPLACE_L},
		{"O", DEBUG_REPLACE_O},
		{"S", DEBUG_REPLACE_S},
		{"Z", DEBUG_REPLACE_Z},
		{"T", DEBUG_REPLACE_T},
		{"rename", COMMAND_RENAME}}
{
	addCommandPrefixLookup("left");
	addCommandPrefixLookup("right");
	addCommandPrefixLookup("down");
	addCommandPrefixLookup("clockwise");
	addCommandPrefixLookup("counterclockwise");
	addCommandPrefixLookup("drop");
	addCommandPrefixLookup("levelup");
	addCommandPrefixLookup("leveldown");
	addCommandPrefixLookup("norandom");
	addCommandPrefixLookup("random");
	addCommandPrefixLookup("sequence");
	addCommandPrefixLookup("restart");
	addCommandPrefixLookup("hint");
	addCommandPrefixLookup("I");
	addCommandPrefixLookup("J");
	addCommandPrefixLookup("L");
	addCommandPrefixLookup("O");
	addCommandPrefixLookup("S");
	addCommandPrefixLookup("Z");
	addCommandPrefixLookup("T");
	addCommandPrefixLookup("rename");
	
//	cout<<"Debug Tree:"<<endl;
//	debugPrintTree(prefixTree);
}

// new node will always have "accepted" state
Game::StateNode::StateNode(const string& str):strAccept{str}{}

// check if a lookup pathway already exists is the tree
bool Game::ifDuplicated(const string& command) {
	shared_ptr<StateNode> cur = prefixTree;
	for(int i=0; i<command.length(); i++) {
		if(cur->path.count(command[i]) <= 0)
			return false;
	}
	return true;
}

void Game::addCommandPrefixLookup(const string& command) {
	shared_ptr<StateNode> cur = prefixTree;
	
	if(ifDuplicated(command)) {
		return;
	}
	
	for(int i=0; i<command.length(); i++) {
		// root node already created by Game ctor
		//cur = cur->append(command, command[i]);
		
		char c = command[i];
		// lookup first
		if(cur->path.count(c) > 0) {
			cur = cur->next[cur->path.at(c)];
			// for any new command: if it shares same states with others,
			// those states will be no longer accepted (except last one)
			cur->strAccept.clear();
		}
		// create new node otherwise
		else {
			shared_ptr<StateNode> tmp = make_shared<StateNode>(command);
			cur->next.emplace_back(tmp);
			cur->path[c] = cur->next.size()-1;
			cur = tmp;
			
			if(cur->next.size() > 1) {
				cur->strAccept.clear();
			}
		}
	}
	
	// fix last node's state into "accepted"
	/*
	ex. 
	root->a->ab->abc is initial tree. All nodes except root are accepted
	Now add new command "ab":
	Tree will remain the same, but node "a" and "ab" will be set to 
	"not accepted" during the traversal.
	That is when the following statement comes in handy.
	*/
	cur->strAccept = command;
}

std::string Game::getCommandByPrefix(const std::string& prefix) {
	shared_ptr<StateNode> cur = prefixTree;
	string::const_iterator it = prefix.begin();
	while (it != prefix.end()) {
		if(cur->path.count(*it) > 0) {
			cur = cur->next[cur->path.at(*it)];
		}
		else {
			return "";
		}
		++it;
	}
	
	return cur->strAccept;
}

void Game::parseCommand(const std::string& cmd) {
	// split number and command
	int pos=-1, cnt;
	string arg;
	
	for(int i=0;i<cmd.length();i++) {
		if(isdigit(cmd[i])) {
			pos = i;
		}
		else {
			break;
		}
	}
	
	if(pos == -1) {
		cnt = 1;
	}
	else {
		cnt = stoi(cmd.substr(0, pos+1));
	}
	
	arg = cmd.substr(pos+1, cmd.length());
	
//	cout << cnt << " " << arg << endl;
//	cout << "Command lookup: " << endl;
//	cout << "  Command map: ";
//	if(command.count(arg) > 0) cout << "pass" << endl;
//	else cout << "fail" << endl;
//	cout << "  Macro map: ";
//	if(macro.count(arg) > 0) cout << "pass" << endl;
//	else cout << "fail" << endl;
//	cout << "  Prefix lookup: ";
//	if(!getCommandByPrefix(arg).empty()) cout << "pass(" << getCommandByPrefix(arg) << ")" << endl;
//	else cout << "fail" << endl;
}
