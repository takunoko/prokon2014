// xのピクセル数 .cols	yのピクセル数 .rows

#include <iostream>	//標準入出力
#include <fstream>	//ファイル入出力
#include <string>		//文字列を扱うため

//stlを用いる
#include <vector>
#include <tuple>

//使ってない？
// openCV関連
// Makefileの方でライブラリとかの場所を指定してある
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// 画像ファイル名と分割数
// 後々、分割数をファイルから読み込む
#define FILENAME "../pic_data/9.ppm"
#define PIECE_X 4
#define PIECE_Y 2

#define ORIGIN_IMG 0
#define LINE_IMG 1

// 2次元を1次元に
#define CONV_XY( x, y)	( x + y*PIECE_X )
// XY座標を1次元に
#define CONV_X(XY) (XY % PIECE_X)
#define CONV_Y(XY) (XY / PIECE_X)

using namespace std;

typedef tuple< int, int, int, int> COST_TUPLE;

// tupleを比較するときのルール
bool my_compare( const COST_TUPLE &lhs, const COST_TUPLE &rhs){
	if (std::get<0>(lhs) != std::get<0>(rhs)) return std::get<0>(lhs) < std::get<0>(rhs);
	if (std::get<1>(lhs) != std::get<1>(rhs)) return std::get<1>(lhs) < std::get<1>(rhs);
	if (std::get<2>(lhs) != std::get<2>(rhs)) return std::get<2>(lhs) < std::get<2>(rhs);
	return std::get<2>(lhs) < std::get<2>(rhs);
}

void disp_img();

class PPMFILE{
	private:
		int part_size_x, part_size_y;	//左右どれだけのピース数があるか?

		string img_name;
		cv::Mat origin_img;
		cv::Mat	line_img;
		vector<cv::Mat>	part_img;

		// cost配列(3次元)
		vector< vector< vector< pair<int,int> > > > cost;
		// cost_t配列(1次元)
		vector<COST_TUPLE> cost_t;

	public:
		PPMFILE(string filename){
			//file.push_back(filename);
			img_name = filename;
			origin_img = cv::imread(img_name, -1);
			part_size_x = PIECE_X;
			part_size_y = PIECE_Y;
			// costのリサイズ

			//それぞれのピースに分割
			this->create_partition();
		}
		//画像を表示する
		void disp_img(int type){
			cv::namedWindow("image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
			switch (type){
				case ORIGIN_IMG:	cv::imshow("image", origin_img);
								break;
				case LINE_IMG: cv::imshow("image", line_img);
								break;
				default:
								break;
			}
		}
		//境界線を引く
		void write_line(void){
			line_img = origin_img.clone(); // origin_img をコピー　
			//縦向きの線
			for(int x=1; x < part_size_x; x++){
				cv::line( line_img, cv::Point( x*(line_img.cols/part_size_x), 0), cv::Point( x*(line_img.cols/part_size_x), line_img.rows), cv::Scalar( 200, 0, 0), 2, 0);
			}
			//横向きの線
			for(int y=1; y < part_size_y; y++){
				cv::line( line_img, cv::Point( 0, y*(line_img.rows/part_size_y)), cv::Point( line_img.cols, y*(line_img.rows/part_size_y)), cv::Scalar( 0, 0, 200), 2, 0);
			}
		}
		// それぞれのピースに分割
		void create_partition(void){
			int part_width = origin_img.cols/part_size_x;
			int part_height = origin_img.rows/part_size_y;

			//pair_img[n]にそれぞれのピースを代入
			for(int y=0; y < part_size_y; y++){
				for(int x=0; x < part_size_x; x++){
					cv::Mat tmp(origin_img, cv::Rect(x*part_width, y*part_height, part_width, part_height));
					part_img.push_back(tmp);
				}
			}
		}

		// 配列の近似値を計算ｎする。
		void calc_cost(void){
			// costのサイズ変更
			cost.resize( part_size_x*part_size_y);
			for(int i=0; i<part_size_x*part_size_y; i++){
				cost[i].resize(4);
				for(int j=0; j<4; j++){
					cost[i][j].resize(part_size_x*part_size_y);
				}
			}

			//上下左右のcostを保持
			// 0:上 1:下 2:左 3:右
			int cost_tmp[4];


			cout << "start_calc" << endl;

			// 差分積算を求める
			// 効率が悪い
			// for(int abs_y=0; abs_y < part_size_y; ++abs_y){
			// 	for(int abs_x=0; abs_x < part_size_x; ++abs_x){
			// 		{
			// 			//自分と他の比較
			// 			// 無駄な比較を防ぐ abs_x+1が正しく動作してるかは微妙
			// 			for(int y=0; y < part_size_y; ++y){
			// 				for(int x=0; x < part_size_x; ++x){
			// 					//コスト初期化
			// 					for(int i=0; i<4; i++){
			// 						cost_tmp[i] = 0;
			// 					}
			// 					//カラーの数だけ繰り返す
			// 					for(int c=0; c < part_img[CONV_XY( abs_x, abs_y)].channels(); ++c){
			// 						//各ピクセル上下
			// 						for(int px_x=0; px_x < part_img[0].cols; ++px_x){
			// 							cost_tmp[0] += abs(part_img[CONV_XY( abs_x, abs_y)].at<cv::Vec3b>( 0, px_x)[c] - part_img[CONV_XY( x, y)].at<cv::Vec3b>( part_img[0].rows-1, px_x)[c]);
			// 							cost_tmp[1] += abs(part_img[CONV_XY( abs_x, abs_y)].at<cv::Vec3b>( part_img[0].rows-1, px_x)[c] - part_img[CONV_XY( x, y)].at<cv::Vec3b>( 0, px_x)[c]);
			// 						}
			// 						//各ピクセル左右
			// 						for(int px_y=0; px_y < part_img[0].rows; ++px_y){
			// 							cost_tmp[2] += abs(part_img[CONV_XY( abs_x, abs_y)].at<cv::Vec3b>( px_y, part_img[0].cols-1)[c] - part_img[CONV_XY( x, y)].at<cv::Vec3b>( px_y, 0)[c]);
			// 							cost_tmp[3] += abs(part_img[CONV_XY( abs_x, abs_y)].at<cv::Vec3b>( px_y, 0)[c] - part_img[CONV_XY( x, y)].at<cv::Vec3b>( px_y, part_img[0].cols-1)[c]);
			// 						}
			// 					}

			// 					// cost_t [ コスト, 自分の座標, 相手の座標, 方向]
			// 					for(int k=0; k<4; k++){
			// 						cost_t.push_back(make_tuple( cost_tmp[k], CONV_XY( abs_x, abs_y), CONV_XY( x, y), k));
			// 						cost[CONV_XY( abs_x, abs_y)][k][CONV_XY( x, y)] = make_pair( cost_tmp[k], CONV_XY( x, y));
			// 					}
			// 				}
			// 			}
			// 		}

			// 効率が良い
			// 差分積算量は数が少ない方から多い方にじゃないと調べられない
			for(int abs_xy=0; abs_xy < part_size_x * part_size_y; ++abs_xy){
				// 無駄を省く。
				for(int xy=abs_xy+1; xy < part_size_x * part_size_y; ++xy){
					//コスト初期化
					for(int i=0; i<4; i++){
						cost_tmp[i] = 0;
					}
					//カラーの数だけ繰り返す
					for(int c=0; c < part_img[abs_xy].channels(); ++c){
						//各ピクセル上下
						for(int px_x=0; px_x < part_img[0].cols; ++px_x){
							cost_tmp[0] += abs(part_img[abs_xy].at<cv::Vec3b>( 0, px_x)[c] - part_img[xy].at<cv::Vec3b>( part_img[0].rows-1, px_x)[c]);
							cost_tmp[1] += abs(part_img[abs_xy].at<cv::Vec3b>( part_img[0].rows-1, px_x)[c] - part_img[xy].at<cv::Vec3b>( 0, px_x)[c]);
						}
						//各ピクセル左右
						for(int px_y=0; px_y < part_img[0].rows; ++px_y){
							cost_tmp[2] += abs(part_img[abs_xy].at<cv::Vec3b>( px_y, part_img[0].cols-1)[c] - part_img[xy].at<cv::Vec3b>( px_y, 0)[c]);
							cost_tmp[3] += abs(part_img[abs_xy].at<cv::Vec3b>( px_y, 0)[c] - part_img[xy].at<cv::Vec3b>( px_y, part_img[0].cols-1)[c]);
						}
					}

					// cost_t [ コスト, 自分の座標, 相手の座標, 方向]
					// cost [自分の座標][方向][相手の座標]
					for(int k=0; k < 4; ++k){
						cost_t.push_back(make_tuple( cost_tmp[k], abs_xy, xy, k));
						cost[abs_xy][k][xy] = make_pair( cost_tmp[k], xy);
					}
				}
			}
			cout << "channels : " << part_img[0].channels() << endl;

			cout << "start_sort" << endl;

			// 独自のルールでCOST_TUPLEをソート
			sort( cost_t.begin(), cost_t.end(), my_compare);

			cout << "end calc" << endl;
		}
		void disp_cost_list(void){
			// 表示
			cout << "score : my_pos : pair_pos : direction " << endl;
			for(int i=0; i < cost_t.size(); i++){
				cout << i << " |cost: " << get<0>(cost_t[i]) << ", (" << CONV_X( get<1>(cost_t[i])) << "," << CONV_Y( get<1>(cost_t[i])) << "), (" << CONV_X(get<2>(cost_t[i])) << "," << CONV_Y(get<2>(cost_t[i])) << "), " << get<3>(cost_t[i]) << ", " << endl;
			}
		}
};

int main(void){
	PPMFILE *img1 = new PPMFILE(FILENAME);

	img1->calc_cost();

	img1->disp_cost_list();
	// img1->write_line();
	// img1->disp_img(LINE_IMG);

	// cv::waitKey(0);	//waitKey(0)で何か入力するまで処理を停止

	return 0;
}
