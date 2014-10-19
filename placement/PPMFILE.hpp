#include <vector>
#include <tuple>
#include <string> // 文字列の変換に使う
#include <sstream>

#include <opencv2/core/core.hpp>

#include "../sort/PosData.h"
#define DEBUG

#define ORIGIN_IMG 0
#define LINE_IMG 1
#define RESULT_IMG 2
#define NUM_IMG 3

#define DIRE_U 0
#define DIRE_D 1
#define DIRE_R 2
#define DIRE_L 3

#define COST_DEF 0
#define COST_MARU 1
#define COST_ALL 2

#define BORDER_WEIGHT 1 // 境界線の重要度
#define DEEP_SERACH 5 // たどる近似度(n^3の情報量になっていくため注意)

#define MIN_2( A, B) ((A) < (B) ? (A) : (B))
#define BIG_2( A, B) ((A) > (B) ? (A) : (B))

using namespace std;

typedef tuple< int, int, int, int> COST_TUPLE;

// スクラップ(いくつかのパーツの集まり)
typedef struct{
	int width, height;
	map<int,pair<int,int> > elements;	// パーツidから、それが使われているか?
	map<pair<int, int>, int> used_p;// ポジションから、そこに何があるか？
}SCRAP;

typedef struct{
	int width, height;
	map<int,pair<int,int> > elements;	// パーツidから、それが使われているか?
	map<pair<int, int>, int> used_p;// ポジションから、そこに何があるか？
}CORE_SCRAP;

typedef struct{
	map<int,pair<int,int> > elements;
	map<pair<int, int>,int> used;
}SCRAP_4;

class PPMFILE{
	private:
		int part_size_x, part_size_y;	//左右どれだけのピース数があるか?
		int part_px_x, part_px_y;

		cv::Mat origin_img;
		cv::Mat	line_img;
		cv::Mat num_img;
		cv::Mat	result_img;

		cv::Mat	for_manual_img;

		vector<cv::Mat>	part_img;

		// cost配列(3次元)
		vector< vector< vector< pair<int,int> > > > cost;
		vector< vector< vector< pair<int,int> > > > cost_maru;
		vector< vector< vector< pair<int,int> > > > cost_all;
		vector< vector< vector< pair<int,int> > > > cost_all_def; // 絶対にソートしない
		// cost_t配列(1次元)
		vector<COST_TUPLE> cost_t;
		vector<COST_TUPLE> cost_t_maru;
		vector<COST_TUPLE> cost_t_all;
		// 配置の位置を示す配列
		map<int,pair<int,int> > placement_pos;

	public:
		// 2次元を1次元に
		inline int CONV_XY(int x,int y){
			return x+y * part_size_x;
		}
		// XY座標を1次元に
		inline int CONV_X(int XY){
			return XY % part_size_x;
		}
		inline int CONV_Y(int XY){
			return XY/part_size_x;
		}
		PPMFILE(cv::Mat origin_img_tmp, int piece_x, int piece_y);
		//画像を表示する
		void disp_img(int type);
		//境界線を引く
		void write_line(void);
		// 番号を画像にいれる
		void create_num_img(void);
		// それぞれのピースに分割
		void create_partition(void);
		// 配列の近似値を計算ｎする。
		void calc_cost(void);
		// まるさん方式　
		void calc_cost_maru(void);

		// コストリストの中身を表示(引数にどのリストを表示するか選択)
		void disp_cost_list(int);

		// 配置
		void placement(void);

		void disp_placement(void);

		// 配置(4ピース集合バージョン)
		void placement_4(void);

		// 配置(4ピース集合 逆バージョン)
		void placement_5(void);

		// 配列(4ピース集合 ランダムバージョン)
		void placement_6(void);

		//PosDataにデータを挿入
		void set_PosData(PosData *data);

		// 配置してみた時の表示
		void create_result_img(void);

		// xy_1のdire方向のxy_2とのcost
		int get_cost(int xy_1, int xy_2, int dire);

		// すべての方向に対してコストを計算する
		void calc_cost_all();

		// 回答が形式を満たしているか？
		bool chk_result(void);

		// 正しい領域のみの生成した画像作成
		void create_correct_area_result_img();
		void disp_for_manual(const string & winname);
		void fix_manual(const string & winname);
		void fix_pic_to_square();
};
