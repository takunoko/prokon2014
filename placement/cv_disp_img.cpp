// xのピクセル数 .cols	yのピクセル数 .rows

// stlを用いた画像の配置
// openCVの場所
#include <iostream>	//標準入出力
#include <fstream>	//ファイル入出力
#include <string>		//文字列を扱うため

#include <vector>

#include <stdlib.h>
#include <math.h> /* abs 絶対値 */

// openCV関連
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define FILENAME "../pic_data/9.ppm"
#define PIECE_X 16
#define PIECE_Y 16

#define CONV_XY( x, y)	( x + y*PIECE_X )

using namespace std;

void disp_img();

class PPMFILE{
	private:
		string img_name;
		cv::Mat origin_img;
		int part_size_x, part_size_y;	//左右どれだけのピース数があるか?

		vector<cv::Mat>	part_img;
		//vector<string> file;
	public:
		PPMFILE(string filename){
			//file.push_back(filename);
			img_name = filename;
			origin_img = cv::imread(img_name, -1);
			part_size_x = PIECE_X;
			part_size_y = PIECE_Y;
			//それぞれのピースに分割
			this->create_partition();
		}
		//画像を表示する
		void disp_img(void){
			cv::namedWindow("image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
			cv::imshow("image", origin_img);
			cv::waitKey(0);	//waitKey(0)で何か入力するまで処理を停止
		}
		//境界線を引く
		void write_line(void){
			cv::Mat line_img = origin_img.clone(); // origin_img をコピー　
			//縦向きの線
			for(int x=1; x < part_size_x; x++){
				cv::line( line_img, cv::Point( x*(line_img.cols/part_size_x), 0), cv::Point( x*(line_img.cols/part_size_x), line_img.rows), cv::Scalar( 200, 0, 0), 2, 0);
			}
			//横向きの線
			for(int y=1; y < part_size_y; y++){
				cv::line( line_img, cv::Point( 0, y*(line_img.rows/part_size_y)), cv::Point( line_img.cols, y*(line_img.rows/part_size_y)), cv::Scalar( 0, 0, 200), 2, 0);
			}
			cv::namedWindow("line_image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
			cv::imshow("line_image", line_img);
			cv::waitKey(0);	//waitKey(0)で何か入力するまで処理を停止
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
		void calc_cost(void){
			vector< vector< vector< pair<int,int> > > > cost( part_size_x*part_size_y, vector<vector<pair<int,int> > >(4, vector<pair<int,int> >( part_size_x*part_size_y)));
			//上下左右のcostを保持
			int cost_tmp[4];

			//自分の座標 abs_*
			for(int abs_y=0; abs_y < part_size_y; ++abs_y){
				for(int abs_x=0; abs_x < part_size_x; ++abs_x){
					//自分と他の比較
					for(int y=0; y < part_size_y; ++y){
						for(int x=0; x < part_size_x; ++x){
							//コスト初期化
							for(int i=0; i<4; i++){
								cost_tmp[i] = 0;
							}
							//カラーの数だけ繰り返す
							for(int c=0; c < part_img[CONV_XY( abs_x, abs_y)].channels(); ++c){
								//各ピクセル上下
								for(int px_x=0; px_x < part_img[0].cols; ++px_x){
									cost_tmp[0] += abs(part_img[CONV_XY( abs_x, abs_y)].at<cv::Vec3b>( 0, px_x)[c] - part_img[CONV_XY( x, y)].at<cv::Vec3b>( part_img[0].rows-1, px_x)[c]);
									cost_tmp[1] += abs(part_img[CONV_XY( abs_x, abs_y)].at<cv::Vec3b>( part_img[0].rows-1, px_x)[c] - part_img[CONV_XY( x, y)].at<cv::Vec3b>( 0, px_x)[c]);
								}

								//各ピクセル左右
								for(int px_y=0; px_y < part_img[0].rows; ++px_y){
									cost_tmp[2] += abs(part_img[CONV_XY( abs_x, abs_y)].at<cv::Vec3b>( px_y, part_img[0].cols-1)[c] - part_img[CONV_XY( x, y)].at<cv::Vec3b>( px_y, 0)[c]);
									cost_tmp[3] += abs(part_img[CONV_XY( abs_x, abs_y)].at<cv::Vec3b>( px_y, 0)[c] - part_img[CONV_XY( x, y)].at<cv::Vec3b>( px_y, part_img[0].cols-1)[c]);
								}
							}

							cost[CONV_XY( abs_x, abs_y)][0][CONV_XY( x, y)] = make_pair( cost_tmp[0], CONV_XY( x, y));
							cost[CONV_XY( abs_x, abs_y)][1][CONV_XY( x, y)] = make_pair( cost_tmp[1], CONV_XY( x, y));
							cost[CONV_XY( abs_x, abs_y)][2][CONV_XY( x, y)] = make_pair( cost_tmp[2], CONV_XY( x, y));
							cost[CONV_XY( abs_x, abs_y)][3][CONV_XY( x, y)] = make_pair( cost_tmp[3], CONV_XY( x, y));
						}
					}
				}
			}


			for(int abs_y=0; abs_y < part_size_y; ++abs_y){
				for(int abs_x=0; abs_x < part_size_x; ++abs_x){
					sort( cost[CONV_XY( abs_x, abs_y)][0].begin(), cost[CONV_XY( abs_x, abs_y)][0].end());
					sort( cost[CONV_XY( abs_x, abs_y)][1].begin(), cost[CONV_XY( abs_x, abs_y)][1].end());
					sort( cost[CONV_XY( abs_x, abs_y)][2].begin(), cost[CONV_XY( abs_x, abs_y)][2].end());
					sort( cost[CONV_XY( abs_x, abs_y)][3].begin(), cost[CONV_XY( abs_x, abs_y)][3].end());
				}
			}

			cout << "sort" << endl;

			for(int abs_y=0; abs_y < part_size_y; ++abs_y){
				for(int abs_x=0; abs_x < part_size_x; ++abs_x){
					// 優先度順
					cout << "x: " << abs_x << "y: " << abs_y << endl;
					cout << " u d r l" << endl;
					for(int i=0; i < part_size_x * part_size_y; i++){
						cout << " sc:" << cost[CONV_XY( abs_x, abs_y)][0][i].first << " in: " << cost[CONV_XY( abs_x, abs_y)][0][i].second;
						cout << "| sc:" << cost[CONV_XY( abs_x, abs_y)][1][i].first << " in: " << cost[CONV_XY( abs_x, abs_y)][1][i].second;
						cout << "| sc:" << cost[CONV_XY( abs_x, abs_y)][2][i].first << " in: " << cost[CONV_XY( abs_x, abs_y)][2][i].second;
						cout << "| sc:" << cost[CONV_XY( abs_x, abs_y)][3][i].first << " in: " << cost[CONV_XY( abs_x, abs_y)][3][i].second;
						cout << endl;
					}
				}
			}
		}
};

int main(void){
	PPMFILE *img1 = new PPMFILE(FILENAME);

	img1->calc_cost();
	img1->write_line();

	//cv::waitKey(0);	//waitKey(0)で何か入力するまで処理を停止

	return 0;
}