#include <sstream>
#include <iterator>
#include <algorithm>
#include <fstream>
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

Game::Game(bool isTextMode, int level, int rndSeed, const string& script/*, istream& is*/): 
	seed{rndSeed},
	/*in{is}, */
	game{make_unique<Board>(level, script)}, 
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
	string s;
	int c;
	splitToken(token, s, c);
	if(command.count(s) > 0) {
		return true;
	}
	else if(macro.count(s) > 0) {
		return true;
	}
	else {
		s = getCommandByPrefix(s);
		return s.empty() ? false : true;
	}
}

/**
 * Game::perform()
 * 
 * Perform i^th command, mutate index if necessary
 * Return false if game over
 **/
bool Game::perform(const vector<string>& tokens, int& index) {
	CommandType type;
	string token;
	int rept;
	
	// // check if game is over
	// if(game->checkEnd())
	// 	return false;
	
	splitToken(tokens.at(index), token, rept);
	
	// fix the token first
	if(command.count(token)<=0 && macro.count(token)<=0) {
		token = getCommandByPrefix(token);
		if(token.empty()) {
			cout<<"Error: "<<tokens.at(index)<<" not found."<<endl;
			return true;
		}
	}
	
	if(command.count(token) > 0) {
		type = command.at(token);
	}
	else /*if(macro.count(token) > 0)*/ {
		vector<string> m = macro.at(token);
		for(int i=0; i<m.size(); i++) {
			if(!perform(m, i))
				return true;
		}
	}
	
	switch(type) {
		case CONTROL_LEFT:
			// left
			cout<<"DEBUG: left "<<rept<<endl;
			game->move('l', rept);
			break;
		case CONTROL_RIGHT:
			// right
			cout<<"DEBUG: right "<<rept<<endl;
			game->move('r', rept);
			break;
		case CONTROL_DOWN:
			// down
			cout<<"DEBUG: down "<<rept<<endl;
			game->move('d', rept);
			break;
		case CONTROL_CLOCKWISE:
			// clockwise
			cout<<"DEBUG: clockwise "<<rept<<endl;
			for(int c=0;c<rept;c++) {
				game->rotate(true);
			}
			break;
		case CONTROL_CC:
			// counterclockwise
			cout<<"DEBUG: counterclockwise "<<rept<<endl;
			for(int c=0;c<rept;c++) {
				game->rotate(false);
			}
			break;
		case CONTROL_DROP:
			// drop
			cout<<"DEBUG: drop "<<rept<<endl;
			for(int c=0;c<rept;c++) {
				game->drop();
				if(game->checkEnd()) return false;
			}
			break;
		case CONTROL_LEVELUP:
			// levelup
			cout<<"DEBUG: levelup "<<rept<<endl;
			game->changeLevel(rept);
			break;
		case CONTROL_LEVELDOWN:
			// leveldown
			cout<<"DEBUG: leveldown "<<rept<<endl;
			game->changeLevel(-rept);
			break;
		case CONTROL_RND:
			// random
			// ignore multiplier
			cout<<"DEBUG: random"<<endl;
			game->norand(false);
			game->setSeed(seed);
			break;
		case CONTROL_NORND:
		{
			// norandom <file>
			// ignore multiplier
			cout<<"DEBUG: norandom "<<rept<<endl;
			string fileName="";
			if(index < tokens.size()-1) {
				fileName = tokens.at(++index);
			}
			else {
				cout<<"Error: file name not specified for 'norandom'."<<endl;
				break;
			}
			game->norand(true, fileName);
			break;
		}
		case CONTROL_SEQUENCE:
		{
			// TODO: sequence <file>
			cout<<"DEBUG: sequence "<<rept<<endl;
			string fileName;
			ifstream ifs;
			if(index >= tokens.size()-1) {
				cout<<"Error: file name not specified for 'sequence'."<<endl;
				break;
			}
			fileName = tokens.at(++index);
			
			ifs.open(fileName.c_str());
			if(ifs.fail()) {
				cout<<"Error: fail to open dequence file '"<<fileName<<"'."<<endl;
				break;
			}
			
			for(int c=0;c<rept;c++) {
				while(parseCommand(ifs));
				ifs.clear();
				ifs.seekg(0, ios::beg);
			}
			break;
		}
		case CONTROL_RESTART:
			// TODO: restart
			// ignore multiplier
			cout<<"DEBUG: restart "<<rept<<endl;
			break;
		case CONTROL_HINT:
			// hint
			// ignore multiplier
			cout<<"DEBUG: hint "<<rept<<endl;
			game->hint();
			break;
		case DEBUG_REPLACE_I:
			// I
			// actually, repeat this is meaningless
			cout<<"DEBUG: I "<<rept<<endl;
			game->replaceCurrentBlock('I');
			break;
		case DEBUG_REPLACE_J:
			// J
			cout<<"DEBUG: J "<<rept<<endl;
			game->replaceCurrentBlock('J');
			break;
		case DEBUG_REPLACE_L:
			// L
			cout<<"DEBUG: L "<<rept<<endl;
			game->replaceCurrentBlock('L');
			break;
		case DEBUG_REPLACE_O:
			// O
			cout<<"DEBUG: O "<<rept<<endl;
			game->replaceCurrentBlock('O');
			break;
		case DEBUG_REPLACE_S:
			// S
			cout<<"DEBUG: S "<<rept<<endl;
			game->replaceCurrentBlock('S');
			break;
		case DEBUG_REPLACE_Z:
			// Z
			cout<<"DEBUG: Z "<<rept<<endl;
			game->replaceCurrentBlock('Z');
			break;
		case DEBUG_REPLACE_T:
			// T
			cout<<"DEBUG: T "<<rept<<endl;
			game->replaceCurrentBlock('T');
			break;
		case COMMAND_RENAME:
		{
			// rename <original> <new>
			// repeating is also meaningless
			if(tokens.size()-index-1<2) {
				cout<<"Error: Missing argument of 'rename'."<<endl;
				//return true;
				break;
			}
			string src = tokens.at(++index);
			string dest = tokens.at(++index);
			
			cout<<"DEBUG: rename "<<src<<" "<<dest<<endl;
			
			if(test(dest)) {
				cout<<"Error: "<<"'"<<dest<<"' already exists."<<endl;
				//return true;
				break;
			}
			
			// fix the src name first
			if(command.count(src)<=0 && macro.count(src)<=0) {
				src = getCommandByPrefix(src);
				if(src.empty()) {
					cout<<"Error: "<<tokens.at(index-1)<<" not found."<<endl;
					//return true;
					break;
				}
			}
			
			cout<<"  DEBUG: rename "<<src<<" "<<dest<<endl;
			
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
	// check if game is after each command, in case debug statement switch to an invalid block
	if(game->checkEnd()) return false;
	
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
		// TODO: might cause exception
		rept = stoi(token.substr(0, pos+1));
	}
	cmd = token.substr(pos+1, token.length());
}

/**
 * Game::parseCommand()
 *
 * return false if reaching EOF
 **/
bool Game::parseCommand(istream& in) {
	string s;
	vector<string> tokens;
	int cnt;
	
	// fetch a line and tokenize it
	if(!getline(in, s)) {
		return false;
	}
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
				return true;
			}
		}
		macro[s] = tokens;
		addCommandPrefixLookup(s);
		
		cout<<"DEBUG: Add macro '"<<s<<"' => ";
		for(auto it : tokens)
			cout<<it<<" ";
		cout<<endl;
	}
	else {
		for(int i=0;i<tokens.size();i++) {
			if(!perform(tokens, i))
				return false;
		}
	}
	
	return true;
}

void Game::printBoard() {
	game->print();
}
