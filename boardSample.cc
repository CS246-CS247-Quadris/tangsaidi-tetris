#include "row.h"
#include <vector>
#include <utility>
using namespace std;

//used to create shared_ptr and the settler, could be modified
void createSettler(vector<Row> & rows, vector<pair<int, int>> & coord, char blockType, int level, Score & score) {
	shared_ptr<Settler> s = make_shared<Settler>(level, score);
	for (auto &i : coord) {
		rows.at(i.first).setRowAt(i.second, blockType, s);
	}
}

int main() {

	vector<Row> rows;
	Score score;
	const int RowNo = 18;

	for(int i = 0; i < RowNo; ++i){
		Row r;
		rows.emplace_back(r);
	}

	cout << "------------------" << endl;
	cout << "Score: " << score.getCurrentScore() << endl;
	cout << "Hi Score: " << score.getHighestScore() << endl;
	cout << "------------------" << endl;

	for (auto &i : rows) {
		cout << i;
	}

	cout << "Is last row removable? " << rows.at(17).isRemovable() << endl;

	//rows.at(y).setRowAt(x)
	vector<pair<int, int>> b1;
	b1.emplace_back(make_pair(17,0));
	b1.emplace_back(make_pair(17,1));
	b1.emplace_back(make_pair(17,2));
	b1.emplace_back(make_pair(17,3));
	createSettler(rows, b1, 'I', 0, score);

	cout << "------------------" << endl;
	cout << "Score: " << score.getCurrentScore() << endl;
	cout << "Hi Score: " << score.getHighestScore() << endl;
	cout << "------------------" << endl;

	for (auto &i : rows) {
		cout << i;
	}

	cout << "Is last row removable? " << rows.at(17).isRemovable() << endl;
	
	vector<pair<int, int>> b2;
	b2.emplace_back(make_pair(17,4));
	b2.emplace_back(make_pair(17,5));
	b2.emplace_back(make_pair(17,6));
	b2.emplace_back(make_pair(17,7));
	createSettler(rows, b2, 'J', 1, score);

	cout << "------------------" << endl;
	cout << "Score: " << score.getCurrentScore() << endl;
	cout << "Hi Score: " << score.getHighestScore() << endl;
	cout << "------------------" << endl;

	for (auto &i : rows) {
		cout << i;
	}

	cout << "Is last row removable? " << rows.at(17).isRemovable() << endl;

	vector<pair<int, int>> b3;
	b3.emplace_back(make_pair(17,8));
	b3.emplace_back(make_pair(17,9));
	b3.emplace_back(make_pair(17,10));
	b3.emplace_back(make_pair(16,10));
	createSettler(rows, b3, 'L', 2, score);

	cout << "------------------" << endl;
	cout << "Score: " << score.getCurrentScore() << endl;
	cout << "Hi Score: " << score.getHighestScore() << endl;
	cout << "------------------" << endl;

	for (auto &i : rows) {
		cout << i;
	}

	cout << "Is last row removable? " << rows.at(17).isRemovable() << endl;

	rows.pop_back();
	Row rEmpty;
	rows.emplace(rows.begin(), rEmpty);

	cout << "------------------" << endl;
	cout << "Score: " << score.getCurrentScore() << endl;
	cout << "Hi Score: " << score.getHighestScore() << endl;
	cout << "------------------" << endl;

	for (auto &i : rows) {
		cout << i;
	}

	cout << "Is last row removable? " << rows.at(17).isRemovable() << endl;

	// rows.pop_back();
	// Row r2Empty;
	// rows.emplace(rows.begin(), r2Empty);

	// cout << "------------------" << endl;
	// cout << "Score: " << score.getCurrentScore() << endl;
	// cout << "Hi Score: " << score.getHighestScore() << endl;
	// cout << "------------------" << endl;

	// for (auto &i : rows) {
	// 	cout << i;
	// }

	// cout << "Is last row removable? " << rows.at(17).isRemovable() << endl;

	//must be called before deallocating all variables
	score.endGame();

	return 0;
}