// stlを使ってみる　
#include <iostream>
#include <string>

#include <random>

#include <vector>

using namespace std;

int main(void){
	int size;

	// 乱数をうんぬんするための処理。
	// あまり気にしなくてOK
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dice(1,100);


	//cin >> size;
	//vector<int> v(size);
	
	std::vector<int> v;

	for(int i=0; i<10; i++){
		v.push_back(dice(mt));
	}

	//for(int i; i<10; i++){
	//	cout << "v : " << v[i] << endl;
	//}
	//cout << endl;

	for(vector<int>::iterator i=v.begin(); i!=v.end(); i++){
		cout << "v : " << *i << endl;
	}

	cout << endl;

	std::sort(v.begin(), v.end()); // ソートしてくれる

	for(vector<int>::iterator i=v.begin(); i!=v.end(); i++){
		cout << "v : " << *i << endl;
	}

	// 最初から vector<int>::iterator i=v.begin(); i!=v.end(); i++
	// 後ろから vector<int>::reverse_iterator i=v.rbegin(); i!=v.rend(); i++ (全体的にrがつく)

	//cout << endl;
	//for(int value : v){
	//	cout << "v : " << value << endl;
	//}//ちょっとヤバそうなやり方


	return 0;
}
