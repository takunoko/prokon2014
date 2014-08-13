// stlを使ってみる2
#include <iostream>
#include <string>

#include <random>

//#include <vector>
#include <utility>

using namespace std;

int main(void){
	typedef pair<int, int> tmp;
	vector<tmp> test_pair;

	//乱数うんうぬん
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dice(1,100);


	// 2項目の構造体みたいな感じのを作る。
	// ソートは1個目の要素、2個目の要素の潤に行われる。
	for(int i=0; i<10; i++){
		test_pair.push_back( tmp(dice(mt), i));
	}

	for(int i=0; i<test_pair.size(); i++){
		cout << "f: " << test_pair[i].first << "  s: " << test_pair[i].second << endl;
	}

	cout << endl;

	sort(test_pair.begin(), test_pair.end());

	for(int i=0; i<test_pair.size(); i++){
		cout << "f: " << test_pair[i].first << "  s: " << test_pair[i].second << endl;
	}

	return 0;
}
