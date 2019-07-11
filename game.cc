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

// perform i^th command, mutate index if necessary
bool Game::perform(const vector<string>& tokens, int& index/*, const int& rept*/) {
	CommandType type;
	string token;
	int rept;
	
	splitToken(tokens.at(index), token, rept);
	
	// fix the token first
	if(command.count(token)<=0 && macro.count(token)<=0) {
		token = getCommandByPrefix(token);
		if(token.empty()) {
			cout<<"Error: "<<tokens.at(index)<<" not found."<<endl;
			return false;
		}
	}
	
	if(command.count(token) > 0) {
		type = command.at(token);
	}
	else /*if(macro.count(token) > 0)*/ {
		vector<string> m = macro.at(token);
		for(int i=0; i<m.size(); i++) {
			if(!perform(m, i))
				return false;
		}
	}
	
	switch(type) {
		case CONTROL_LEFT:
			// TODO: left
			cout<<"DEBUG: left "<<rept<<endl;
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_RIGHT:
			// TODO: right
			cout<<"DEBUG: right "<<rept<<endl;
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_DOWN:
			// TODO: down
			cout<<"DEBUG: down "<<rept<<endl;
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_CLOCKWISE:
			// TODO: clockwise
			cout<<"DEBUG: clockwise "<<rept<<endl;
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_CC:
			// TODO: counterclockwise
			cout<<"DEBUG: counterclockwise "<<rept<<endl;
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_DROP:
			// TODO: drop
			cout<<"DEBUG: drop "<<rept<<endl;
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_LEVELUP:
			// TODO: levelup
			cout<<"DEBUG: levelup "<<rept<<endl;
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_LEVELDOWN:
			// TODO: leveldown
			cout<<"DEBUG: leveldown "<<rept<<endl;
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_RND:
			// TODO: random
			// ignore multiplier
			cout<<"DEBUG: random "<<rept<<endl;
			break;
		case CONTROL_NORND:
			// TODO: norandom <file>
			// TODO: level 3 and 4 only
			// ignore multiplier
			cout<<"DEBUG: norandom "<<rept<<endl;
			break;
		case CONTROL_SEQUENCE:
			// TODO: sequence <file>
			// TODO: set istream to the file, call 
			// parseCommand, then restore istream
			cout<<"DEBUG: sequence "<<rept<<endl;
			for(int c=0;c<rept;c++) {
			}
			break;
		case CONTROL_RESTART:
			// TODO: restart
			// ignore multiplier
			cout<<"DEBUG: restart "<<rept<<endl;
			break;
		case CONTROL_HINT:
			// TODO: hint
			// ignore multiplier
			cout<<"DEBUG: hint "<<rept<<endl;
			break;
		case DEBUG_REPLACE_I:
			// TODO: I
			// actually, repeat this is meaningless
			cout<<"DEBUG: I "<<rept<<endl;
			break;
		case DEBUG_REPLACE_J:
			// TODO: J
			cout<<"DEBUG: J "<<rept<<endl;
			break;
		case DEBUG_REPLACE_L:
			// TODO: I
			cout<<"DEBUG: L "<<rept<<endl;
			break;
		case DEBUG_REPLACE_O:
			// TODO: O
			cout<<"DEBUG: O "<<rept<<endl;
			break;
		case DEBUG_REPLACE_S:
			// TODO: S
			cout<<"DEBUG: S "<<rept<<endl;
			break;
		case DEBUG_REPLACE_Z:
			// TODO: Z
			cout<<"DEBUG: Z "<<rept<<endl;
			break;
		case DEBUG_REPLACE_T:
			// TODO: T
			cout<<"DEBUG: T "<<rept<<endl;
			break;
		case COMMAND_RENAME:
		{
			// TODO: rename <original> <new>
			// repeating is also meaningless
			if(tokens.size()-index-1<2) {
				cout<<"Error: Missing argument of 'rename'."<<endl;
				return false;
			}
			string src = tokens.at(++index);
			string dest = tokens.at(++index);
			
			cout<<"DEBUG: rename "<<src<<" "<<dest<<endl;
			
			if(test(dest)) {
				cout<<"Error: "<<"'"<<dest<<"' already exists."<<endl;
				return false;
			}
			
			// fix the src name first
			if(command.count(src)<=0 && macro.count(src)<=0) {
				src = getCommandByPrefix(src);
				if(src.empty()) {
					cout<<"Error: "<<tokens.at(index-1)<<" not found."<<endl;
					return false;
				}
			}
			
			if(command.count(src) > 0) {
				command[dest] = command.at(src);
			}
			else /*if(macro.count(src) > 0)*/ {
				macro[dest] = macro.at(src);
			}
			
			addCommandPrefixLookup(dest);
			break;
		}
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

bool Game::parseCommand() {
	string s;
	vector<string> tokens;
	int cnt;

	getline(in, s);
	istringstream iss(s);
	copy(istream_iterator<string>(iss), 
			istream_iterator<string>(), 
			back_inserter(tokens));
	
	// test if the first token is used for defining macro
	splitToken(tokens[0], s, cnt);
	if(s.back() == ':') {
		s.pop_back();
		tokens.erase(tokens.begin());
		for(auto& it : tokens) {
			if(!test(it)) {
				cout<<"Error: "<<it<<" not found."<<endl;
				return false;
			}
		}
		macro[s] = tokens;
		addCommandPrefixLookup(s);
	}
	else {
		for(int i=0;i<tokens.size();i++) {
			if(!perform(tokens, i))
				return false;
		}
	}
}
