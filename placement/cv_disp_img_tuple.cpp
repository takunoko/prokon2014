// xのピクセル数 .cols	yのピクセル数 .rows

#include <iostream>	//標準入出力
#include <fstream>	//ファイル入出力
#include <string>		//文字列を扱うため

//stlを用いる
#include <vector>
#include <tuple>

// openCV関連
// Makefileの方でライブラリとかの場所を指定してある
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "PPMFILE.hpp"
#include "../sort/PosData.h"

// 画像ファイル名と分割数
// 後々、分割数をファイルから読み込む

#define FILENAME "../pic_data/problem/prob01.ppm"
using namespace std;
#define PIECE_X 16
#define PIECE_Y 16

int main(void){
	// 引数で渡されるべき部分
	cv::Mat origin_img_tmp = cv::imread(FILENAME, -1);

	// 本番環境では、PPMFILE( cv::Mat オリジナルのイメージ, int 分割数X, int 分割数Y, PosData);
	PPMFILE *img1 = new PPMFILE( origin_img_tmp, PIECE_X, PIECE_Y);

	// ----- このへん計算 -----
	// img1->calc_cost();
	// img1->calc_cost_maru();
	img1->calc_cost_all();
	img1->disp_cost_list(COST_ALL);  // 消すと結構時間が良くなる

	// img1->get_left_top();
	// img1->disp_cost_list(COST_ALL);
	// img1->get_left_top();

	// ----- この編並び替え -----
	// img1->placement();
	// img1->new_placement();
	img1->placement_4();

	// ----- このへん描画 -----
	// img1->write_line();
	// img1->disp_img(LINE_IMG);
	// img1->disp_img(ORIGIN_IMG);
	// img1->disp_placement();

	// ナンバー入り画像
	img1->create_num_img();
	img1->disp_img(NUM_IMG);
	// 回答画像
	img1->create_result_img();
	img1->disp_img(RESULT_IMG);

	cv::waitKey(0);	//waitKey(0)で何か入力するまで処理を停止

	return 0;
}
