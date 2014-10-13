#include <vector>
#include <tuple>

#include <opencv2/core/core.hpp>

#include "../sort/PosData.h"
#define DEBUG

#define ORIGIN_IMG 0
#define LINE_IMG 1
#define RESULT_IMG 2

#define DIRE_U 0
#define DIRE_D 1
#define DIRE_R 2
#define DIRE_L 3

#define COST_DEF 0
#define COST_MARU 1
#define COST_ALL 2


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


class PPMFILE{
	private:
		int part_size_x, part_size_y;	//左右どれだけのピース数があるか?

		cv::Mat origin_img;
		cv::Mat	line_img;
		cv::Mat	result_img;

		vector<cv::Mat>	part_img;

		// cost配列(3次元)
		vector< vector< vector< pair<int,int> > > > cost;
		vector< vector< vector< pair<int,int> > > > cost_maru;
		vector< vector< vector< pair<int,int> > > > cost_all;
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

		// 配置(バージョン2)
		void new_placement(void);

		void disp_placement(void);
		//PosDataにデータを挿入
		void set_PosData(PosData *data);

		// 配置してみた時の表示
		void create_result_img(void);

		// xy_1のdire方向のxy_2とのcost
		int get_cost(int xy_1, int xy_2, int dire);

		// 左上を取得してみる
		void get_left_top();

		// すべての方向に対してコストを計算する
		void calc_cost_all();

		// 横方向に画像を追加
		void add_side( SCRAP &scrap, int, int, int, int);
		// 縦方向に画像を追加
		void add_ud( SCRAP &scrap, int, int, int, int);
};
