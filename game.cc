#include <sstream>
#include <iterator>
#include <algorithm>
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
		{"rename", COMMAND_RENAME},
		{"&&", COMMAND_AMP}}
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
	addCommandPrefixLookup("&&");
	
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

std::string Game::getCommandByPrefix(const string& prefix) {
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

bool Game::test(const string& token) {
	if(command.count(token) > 0) {
		return true;
	}
	else if(macro.count(token) > 0) {
		return true;
	}
	else {
		string s = getCommandByPrefix(token);
		return s.empty() ? false : true;
	}
}

bool Game::perform(const vector<string>& tokens, size_t& index, const int& rept) {
	CommandType type;
	string token = tokens.at(index);
	
	if(command.count(token) > 0) {
		type = command.at(token);
	}
	else if(macro.count(token) > 0) {
		vector<string> m = macro.at(token);
		for(auto &v : m) {
			perform(tokens, index, rept);
		}
	}
	else {
		string s = getCommandByPrefix(token);
		if(!s.empty()) {
			type = command.at(s);
		}
		else {
			cout<<"Error: "<<token<<" not found."<<endl;
			return false;
		}
	}
	
	switch(type) {
		case CONTROL_LEFT:
			// TODO: left
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_RIGHT:
			// TODO: right
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_DOWN:
			// TODO: down
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_CLOCKWISE:
			// TODO: clockwise
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_CC:
			// TODO: counterclockwise
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_DROP:
			// TODO: drop
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_LEVELUP:
			// TODO: levelup
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_LEVELDOWN:
			// TODO: leveldown
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_RND:
			// TODO: random
			// ignore multiplier
			break;
		case CONTROL_NORND:
			// TODO: norandom <file>
			// TODO: level 3 and 4 only
			// ignore multiplier
			break;
		case CONTROL_SEQUENCE:
			// TODO: sequence <file>
			// TODO: set istream to the file, call 
			// parseCommand, then restore istream
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_RESTART:
			// TODO: restart
			// ignore multiplier
			break;
		case CONTROL_HINT:
			// TODO: hint
			// ignore multiplier
			break;
		case DEBUG_REPLACE_I:
			// TODO: I
			// actually, repeat this is meaningless
			break;
		case DEBUG_REPLACE_J:
			// TODO: J
			
			break;
		case DEBUG_REPLACE_L:
			// TODO: I
			
			break;
		case DEBUG_REPLACE_O:
			// TODO: O
			
			break;
		case DEBUG_REPLACE_S:
			// TODO: S
			break;
		case DEBUG_REPLACE_Z:
			// TODO: Z
			break;
		case DEBUG_REPLACE_T:
			// TODO: T
			break;
		case COMMAND_RENAME:
			// TODO: rename <original> <new>
			// repeating is also meaningless
			break;
		case COMMAND_AMP:
		default:
			break;
	}
	
	return true;
}

// split number out of a token
void Game::splitToken(const string& token, string& cmd, int& rept) {
	int pos=-1;
	
	for(int i=0;i<token.length();i++) {
		if(isdigit(token[i])) {
			pos = i;
		}
		else {
			break;
		}
	}
	
	if(pos == -1) {
		rept = 1;
	}
	else {
		rept = stoi(token.substr(0, pos+1));
	}
	cmd = token.substr(pos+1, token.length());
}

void Game::parseCommand() {
	string s;
	vector<string> tokens;
	int cnt;
//	while(true) {
//		getline(in, s);
//	}
	//cout<<"Command: ";
	getline(in, s);
	istringstream iss(s);
	copy(istream_iterator<string>(iss), 
			istream_iterator<string>(), 
			back_inserter(tokens));
	
	splitToken(tokens[0], s, cnt);
	
	// test if the first token is used for defining macro
	if(s.back() == ':') {
		s.pop_back();
		tokens.erase(tokens.begin());
		for(auto& it : tokens) {
			if(!test(it)) {
				cout<<"Error: "<<it<<" not found."<<endl;
				return;
			}
		}
		macro[s] = tokens;
		addCommandPrefixLookup(s);
	}
	else {
		// perform first one
		
		for(int i=1;i<tokens.size();i++) {
			splitToken(tokens[i], s, cnt);
		}
	}
}
