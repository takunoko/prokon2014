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

#define DEBUG

// 画像ファイル名と分割数
// 後々、分割数をファイルから読み込む
#define FILENAME "../pic_data/2.ppm"
#define PIECE_X 4
#define PIECE_Y 8

#define ORIGIN_IMG 0
#define LINE_IMG 1

#define DIRE_U 0
#define DIRE_D 1
#define DIRE_R 2
#define DIRE_L 3

// 2次元を1次元に
#define CONV_XY( x, y)	( x + y*PIECE_X )
// XY座標を1次元に
#define CONV_X(XY) (XY % PIECE_X)
#define CONV_Y(XY) (XY / PIECE_X)
// 小さい方を返す
#define MIN_2( A, B) ((A) < (B) ? (A) : (B))
#define BIG_2( A, B) ((A) > (B) ? (A) : (B))

using namespace std;

typedef tuple< int, int, int, int> COST_TUPLE;

// スクラップ(いくつかのパーツの集まり)
typedef struct{
	// int w,h;
	// vector<int> used_part;
	//vector<pair<int,pair<int,int> > > elements; // element< 自分のxy,< baseからのx, baseからのy> >
	map<int,pair<int,int> > elements;	// パーツidから、それが使われているか?
	map<pair<int, int>, int> used_p;// ポジションから、そこに何があるか？
}SCRAP;

// tupleを比較するときのルール
bool my_compare( const COST_TUPLE &lhs, const COST_TUPLE &rhs){
	if (std::get<0>(lhs) != std::get<0>(rhs)) return std::get<0>(lhs) < std::get<0>(rhs);
	if (std::get<1>(lhs) != std::get<1>(rhs)) return std::get<1>(lhs) < std::get<1>(rhs);
	if (std::get<2>(lhs) != std::get<2>(rhs)) return std::get<2>(lhs) < std::get<2>(rhs);
	return std::get<2>(lhs) < std::get<2>(rhs);
}

// 方向を逆にする
int inverse_direction(int dire){
	int inv_dire;
	switch(dire){
		case DIRE_U:
			inv_dire = DIRE_D;
			break;
		case DIRE_D:
			inv_dire = DIRE_U;
			break;
		case DIRE_R:
			inv_dire = DIRE_L;
			break;
		case DIRE_L:
			inv_dire = DIRE_R;
			break;
		default:
			cout << "error direction" << endl;
			break;
	}
	return inv_dire;
}

// 方向に対するpairを作る
pair<int,int> make_direction_pair(int direction){
	pair<int,int> dire_pair;
	switch(direction){
		case DIRE_U:
			dire_pair = make_pair(0,-1);
			break;
		case DIRE_D:
			dire_pair = make_pair(0,1);
			break;
		case DIRE_R:
			dire_pair = make_pair(1,0);
			break;
		case DIRE_L:
			dire_pair = make_pair(-1,0);
			break;
	}

	return dire_pair;
}

void disp_img();

class PPMFILE{
	private:
		int part_size_x, part_size_y;	//左右どれだけのピース数があるか?

		string img_name;
		cv::Mat origin_img;
		cv::Mat	line_img;
		cv::Mat	result_img;

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

			cout << "start_calc" << endl;

			int cost_tmp[4];
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
							cost_tmp[DIRE_U] += abs(part_img[abs_xy].at<cv::Vec3b>( 0, px_x)[c] - part_img[xy].at<cv::Vec3b>( part_img[0].rows-1, px_x)[c]);
							cost_tmp[DIRE_D] += abs(part_img[abs_xy].at<cv::Vec3b>( part_img[0].rows-1, px_x)[c] - part_img[xy].at<cv::Vec3b>( 0, px_x)[c]);
						}
						//各ピクセル左右
						for(int px_y=0; px_y < part_img[0].rows; ++px_y){
							cost_tmp[DIRE_R] += abs(part_img[abs_xy].at<cv::Vec3b>( px_y, part_img[0].cols-1)[c] - part_img[xy].at<cv::Vec3b>( px_y, 0)[c]);
							cost_tmp[DIRE_L] += abs(part_img[abs_xy].at<cv::Vec3b>( px_y, 0)[c] - part_img[xy].at<cv::Vec3b>( px_y, part_img[0].cols-1)[c]);
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
	// 配置
		void placement(void){
			// 使われたかどうかのフラグ
			vector<SCRAP> scraps;
			vector<int> used_part(part_size_x*part_size_y, -1);


			// 目的の場所
			int dif_x, dif_y;

			int part_s, part_l;
			int part_s_x, part_s_y;
			int part_l_x, part_l_y;

			int part_1, part_2;
			int part_1_x, part_1_y;
			int part_2_x, part_2_y;

			for(int i=0; i < cost_t.size(); i++){
				SCRAP scrap_tmp;
				part_1 = used_part[get<1>(cost_t[i])];
				part_2 = used_part[get<2>(cost_t[i])];

				if(part_1 == -1){
					if(part_2 == -1){ // まだ使われてなかったら
						// どちらもまだ使われていない
						used_part[get<1>(cost_t[i])] = used_part[get<2>(cost_t[i])] = scraps.size();	// 追加したscrap番号を保持
						scrap_tmp.elements[get<1>(cost_t[i])] = make_pair( 0, 0);
						scrap_tmp.used_p[make_pair(0,0)] = get<1>(cost_t[i]);
						scrap_tmp.elements[get<2>(cost_t[i])] = make_direction_pair(get<3>(cost_t[i]));
						scrap_tmp.used_p[make_direction_pair(get<3>(cost_t[i]))] = get<2>(cost_t[i]);
						scraps.push_back(scrap_tmp);
					}
					else{						// 2だけつかわれてる
						part_2_x = scraps[part_2].elements[get<2>(cost_t[i])].first;
						part_2_y = scraps[part_2].elements[get<2>(cost_t[i])].second;
						// 目的の場所(2が基準だから左右が逆になる)
						switch(get<3>(cost_t[i])){
							case DIRE_U:
								--part_2_y;
								break;
							case DIRE_D:
								++part_2_y;
								break;
							case DIRE_R:
								--part_2_x;
								break;
							case DIRE_L:
								++part_2_x;
								break;
						}
						if(scraps[part_2].used_p.find(make_pair(part_2_x,part_2_y)) == scraps[part_2].used_p.end()){
							used_part[get<1>(cost_t[i])] = part_2;
							scraps[part_2].elements[get<1>(cost_t[i])] = make_pair( part_2_x, part_2_y);
							scraps[part_2].used_p[make_pair( part_2_x, part_2_y)] = get<1>(cost_t[i]);
						}else{
							// すでに要素が入っていた場合
						}
					}
				}else{
					if(part_2 == -1){	// 1だけ使われてる
						part_1_x = scraps[part_1].elements[get<1>(cost_t[i])].first;
						part_1_y = scraps[part_1].elements[get<1>(cost_t[i])].second;
						// 目的の場所
						switch(get<3>(cost_t[i])){
							case DIRE_U:
								++part_1_y;
								break;
							case DIRE_D:
								--part_1_y;
								break;
							case DIRE_R:
								++part_1_x;
								break;
							case DIRE_L:
								--part_1_x;
								break;
						}
						// 目撃の場所に要素が入っていなかった場合
						if(scraps[part_1].used_p.find(make_pair(part_1_x,part_1_y)) == scraps[part_1].used_p.end()){
							used_part[get<2>(cost_t[i])] = part_1;
							scraps[part_1].elements[get<2>(cost_t[i])] = make_pair( part_1_x, part_1_y);
							scraps[part_1].used_p[make_pair( part_1_x, part_1_y)] = get<2>(cost_t[i]);
						}else{
							// 既に要素が入っていた場合
						}
					}else{	// 両方ともつかわれている -> スクラップの添字が小さい方に大きい方をくっつける
						if( part_1 != part_2){  // 同じスクラップでない場合
							part_s = MIN_2( part_1, part_2);
							part_l = BIG_2( part_1, part_2);
							cout << "part_1 : " << part_1 << endl;
							cout << "marg : " << part_s << " to " << part_l << endl;

							cout << "PP : (" << CONV_X(get<1>(cost_t[i])) << "," << CONV_Y(get<1>(cost_t[i])) << ")" << endl;
							cout << "PP : (" << CONV_X(get<2>(cost_t[i])) << "," << CONV_Y(get<2>(cost_t[i])) << ")" << endl;
							cout << "discription : " << get<3>(cost_t[i]) << endl;

							if(part_s == part_1){
								cout << "type:A" << endl;
								part_s_x = scraps[part_1].elements[get<1>(cost_t[i])].first;
								part_s_y = scraps[part_1].elements[get<1>(cost_t[i])].second;
								part_l_x = scraps[part_2].elements[get<2>(cost_t[i])].first;
								part_l_y = scraps[part_2].elements[get<2>(cost_t[i])].second;

								cout << "p_s_x : " << part_s_x << " p_s_y : " << part_s_y << endl;
								switch(get<3>(cost_t[i])){
									case DIRE_U:
										--part_s_y;
										break;
									case DIRE_D:
										++part_s_y;
										break;
									case DIRE_R:
										++part_s_x;
										break;
									case DIRE_L:
										--part_s_x;
										break;
								}
							}else{
								// part_2 を基準とする　
								cout << "type:B" << endl;
								part_l_x = scraps[part_1].elements[get<1>(cost_t[i])].first;
								part_l_y = scraps[part_1].elements[get<1>(cost_t[i])].second;
								part_s_x = scraps[part_2].elements[get<2>(cost_t[i])].first;
								part_s_y = scraps[part_2].elements[get<2>(cost_t[i])].second;
								switch(get<3>(cost_t[i])){
									case DIRE_U:
										++part_s_y;
										break;
									case DIRE_D:
										--part_s_y;
										break;
									case DIRE_R:
										--part_s_x;
										break;
									case DIRE_L:
										++part_s_x;
										break;
								}
							}

							cout << "p_s_x : " << part_s_x << " p_s_y : " << part_s_y << endl;

							dif_x = part_s_x - part_l_x;
							dif_y = part_s_y - part_l_y;

							cout << "dif_x : " << dif_x << " dif_y : " << dif_y << endl;

							// scrapが小さい方に統合(何も考えずに)
							for(map<int, pair<int,int> >::iterator j = scraps[part_l].elements.begin(); j != scraps[part_l].elements.end(); j++){
								int key = j->first;
								used_part[key] = part_s;
								pair<int, int> pos = j->second;
								scraps[part_s].elements[key] = make_pair( pos.first + dif_x, pos.second + dif_y);
								scraps[part_s].used_p[make_pair( pos.first + dif_x, pos.second + dif_y)] = key;
							}
							scraps[part_l].elements.clear();
#ifdef DEBUG
							for(int i=0; i < scraps.size(); i++){
								cout << "pair : " << i << endl;
								for(map<int, pair<int,int> >::iterator j = scraps[i].elements.begin(); j != scraps[i].elements.end(); j++){
									int key = j->first;
									pair<int, int> pos = j->second;
									cout << "  (" << CONV_X(key) << "," << CONV_Y(key) << ") || (" << pos.first << "," << pos.second << ")" << endl;
								}
							}
#endif
						}
					}
				}
			}

#ifdef DEBUG
			for(int i=0; i < scraps.size(); i++){
				cout << "pair : " << i << endl;
				for(map<int, pair<int,int> >::iterator j = scraps[i].elements.begin(); j != scraps[i].elements.end(); j++){
					int key = j->first;
					pair<int, int> pos = j->second;
					cout << "  (" << CONV_X(key) << "," << CONV_Y(key) << ") || (" << pos.first << "," << pos.second << ") " << endl;
				}
			}
#endif
		}

		// xy_1のdire方向のxy_2とのcost
		int get_cost(int xy_1, int xy_2, int dire){
			int pair_cost;
			if(xy_1 < xy_2){
				pair_cost = cost[xy_1][dire][xy_2].first;
			}else if(xy_1 > xy_2){
				// 比較順が変わると上下左右が変わる
				dire = inverse_direction(dire);
				pair_cost = cost[xy_2][dire][xy_1].first;
			}else{
				pair_cost = -1;
			}
			return pair_cost;
		}
};

int main(void){
	PPMFILE *img1 = new PPMFILE(FILENAME);

	img1->calc_cost();
	img1->disp_cost_list();

	// 指定した座標のcostを取得する
	// cout << "get (3,1),(1,1),2 : " << img1->get_cost( CONV_XY(3,1), CONV_XY(1,1), 2) << endl;

	img1->placement();

	// img1->write_line();
	// img1->disp_img(LINE_IMG);
	// img1->disp_img(ORIGIN_IMG);

	// cv::waitKey(0);	//waitKey(0)で何か入力するまで処理を停止

	return 0;
}
