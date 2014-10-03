#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>

// C++11のタプルをvectorに突っ込んでソート

typedef std::tuple<int, std::string, double> mytuple;

// タプルの最初の要素から順番に比較　もしその要素が同じなら次の要素で比較
bool mycompare( const mytuple &lhs, const mytuple &rhs )
{
	if (std::get<0>(lhs) != std::get<0>(rhs)) return std::get<0>(lhs) < std::get<0>(rhs);
	if (std::get<1>(lhs) != std::get<1>(rhs)) return std::get<1>(lhs) < std::get<1>(rhs);
	return std::get<2>(lhs) < std::get<2>(rhs);
}

void print_vector( std::vector<mytuple>& vec )
{
	for( auto& element : vec ) {
		std::cout << std::get<0>(element) << ", " << std::get<1>(element) << ", " << std::get<2>(element) << std::endl;
	}
	std::cout << std::endl;
}

int main() {
	std::vector<mytuple> vec{
		std::make_tuple(50, "ccc", 7.5),
			std::make_tuple(30, "kkk", 8.5),
			std::make_tuple(20, "ddd", 9.5),
			std::make_tuple(50, "aaa", 7.5),
			std::make_tuple(20, "ddd", 3.5),
			std::make_tuple(50, "ccc", 2.5),
			std::make_tuple(30, "zzz", -1.5)
	};

	// 比較関数を使ってソートする場合
	sort(vec.begin(), vec.end(), mycompare);
	print_vector(vec);

	// いったんシャッフル
	std::random_shuffle(std::begin(vec), std::end(vec));

	// ラムダ式を使ってソートする場合
	sort(vec.begin(),
			vec.end(),
			[](mytuple const &lhs, mytuple const &rhs) {
			if (std::get<0>(lhs) != std::get<0>(rhs)) return std::get<0>(lhs) < std::get<0>(rhs);
			if (std::get<1>(lhs) != std::get<1>(rhs)) return std::get<1>(lhs) < std::get<1>(rhs);
			return std::get<2>(lhs) < std::get<2>(rhs);
			}
			);
	print_vector(vec);

	return 0;
}
