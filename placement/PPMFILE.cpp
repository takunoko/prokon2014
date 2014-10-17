#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "PPMFILE.hpp"
#include "../sort/PosData.h"

// すべて、宣言されていたらその機能を使う
#define USE_DONT_CONFRICT
#define MERGE_CHK_POS
#define MERGE_ONE // 残っているピースをマージ

// tupleを比較するときのルール
bool my_compare( const COST_TUPLE &lhs, const COST_TUPLE &rhs){
	if (std::get<0>(lhs) != std::get<0>(rhs)) return std::get<0>(lhs) < std::get<0>(rhs);
	if (std::get<1>(lhs) != std::get<1>(rhs)) return std::get<1>(lhs) < std::get<1>(rhs);
	if (std::get<2>(lhs) != std::get<2>(rhs)) return std::get<2>(lhs) < std::get<2>(rhs);
	if (std::get<3>(lhs) != std::get<3>(rhs)) return std::get<3>(lhs) < std::get<3>(rhs);
	return std::get<3>(lhs) < std::get<3>(rhs);
}

// tupleを比較するときのルール
bool my_compare_2( const COST_TUPLE &lhs, const COST_TUPLE &rhs){
	if (std::get<3>(lhs) != std::get<3>(rhs)) return std::get<3>(lhs) > std::get<3>(rhs);
	if (std::get<0>(lhs) != std::get<0>(rhs)) return std::get<0>(lhs) < std::get<0>(rhs);
	if (std::get<1>(lhs) != std::get<1>(rhs)) return std::get<1>(lhs) < std::get<1>(rhs);
	if (std::get<2>(lhs) != std::get<2>(rhs)) return std::get<2>(lhs) < std::get<2>(rhs);
	return std::get<2>(lhs) < std::get<2>(rhs);
}

// scrap_4をソートするためのルール
bool sort_scrap_4( const tuple<int,int,int> &lhs, const tuple<int,int,int> &rhs){
	if (std::get<0>(lhs) != std::get<0>(rhs)) return std::get<0>(lhs) < std::get<0>(rhs);
	if (std::get<1>(lhs) != std::get<1>(rhs)) return std::get<1>(lhs) < std::get<1>(rhs);
	if (std::get<2>(lhs) != std::get<2>(rhs)) return std::get<2>(lhs) < std::get<2>(rhs);
	return std::get<2>(lhs) < std::get<2>(rhs);
}

// scrap_4をstrongを含めてソートするためのルール
bool sort_scrap_4_strong( const tuple<int,int,int,int,int,int,int> &lhs, const tuple<int,int,int,int,int,int,int> &rhs){
	if (std::get<6>(lhs) != std::get<6>(rhs)) return std::get<6>(lhs) > std::get<6>(rhs);
	if (std::get<0>(lhs) != std::get<0>(rhs)) return std::get<0>(lhs) < std::get<0>(rhs);
	if (std::get<1>(lhs) != std::get<1>(rhs)) return std::get<1>(lhs) < std::get<1>(rhs);
	if (std::get<2>(lhs) != std::get<2>(rhs)) return std::get<2>(lhs) < std::get<2>(rhs);
	if (std::get<3>(lhs) != std::get<3>(rhs)) return std::get<3>(lhs) < std::get<3>(rhs);
	if (std::get<4>(lhs) != std::get<4>(rhs)) return std::get<4>(lhs) < std::get<4>(rhs);
	if (std::get<5>(lhs) != std::get<5>(rhs)) return std::get<5>(lhs) < std::get<5>(rhs);
	return std::get<5>(lhs) < std::get<5>(rhs);
}

SCRAP_4 create_scrap_4_element( tuple< int, int, int, int, int, int, int> route_pos){
	SCRAP_4 scrap_4_tmp;

	//int cos = get<0>(route_pos);
	int route = get<1>(route_pos);
	int my_pos = get<2>(route_pos);
	int pos_1 = get<3>(route_pos);
	int pos_2 = get<4>(route_pos);
	int pos_3 = get<5>(route_pos);

	switch(route){
		case 0:
			scrap_4_tmp.elements[pos_1] = make_pair( 0, 0);
			scrap_4_tmp.elements[pos_2]  = make_pair( 1, 0);
			scrap_4_tmp.elements[my_pos] = make_pair( 0, 1);
			scrap_4_tmp.elements[pos_3]  = make_pair( 1, 1);
			scrap_4_tmp.used[make_pair( 0, 0)] = pos_1;
			scrap_4_tmp.used[make_pair( 1, 0)] = pos_2;
			scrap_4_tmp.used[make_pair( 0, 1)] = my_pos;
			scrap_4_tmp.used[make_pair( 1, 1)] = pos_3;
			break;
		case 1:
			scrap_4_tmp.elements[my_pos] = make_pair(0, 0);
			scrap_4_tmp.elements[pos_1]  = make_pair(1, 0);
			scrap_4_tmp.elements[pos_3]  = make_pair(0, 1);
			scrap_4_tmp.elements[pos_2]  = make_pair(1, 1);
			scrap_4_tmp.used[make_pair( 0, 0)] = my_pos;
			scrap_4_tmp.used[make_pair( 1, 0)] = pos_1;
			scrap_4_tmp.used[make_pair( 0, 1)] = pos_3;
			scrap_4_tmp.used[make_pair( 1, 1)] = pos_2;
			break;
		case 2:
			scrap_4_tmp.elements[pos_3]  = make_pair(0, 0);
			scrap_4_tmp.elements[my_pos] = make_pair(1, 0);
			scrap_4_tmp.elements[pos_2]  = make_pair(0, 1);
			scrap_4_tmp.elements[pos_1]  = make_pair(1, 1);
			scrap_4_tmp.used[make_pair( 0, 0)] = pos_3;
			scrap_4_tmp.used[make_pair( 1, 0)] = my_pos;
			scrap_4_tmp.used[make_pair( 0, 1)] = pos_2;
			scrap_4_tmp.used[make_pair( 1, 1)] = pos_1;
			break;
		case 3:
			scrap_4_tmp.elements[pos_2]  = make_pair(0, 0);
			scrap_4_tmp.elements[pos_3]  = make_pair(1, 0);
			scrap_4_tmp.elements[pos_1]  = make_pair(0, 1);
			scrap_4_tmp.elements[my_pos] = make_pair(1, 1);
			scrap_4_tmp.used[make_pair( 0, 0)] = pos_2;
			scrap_4_tmp.used[make_pair( 1, 0)] = pos_3;
			scrap_4_tmp.used[make_pair( 0, 1)] = pos_1;
			scrap_4_tmp.used[make_pair( 1, 1)] = my_pos;
			break;
	}

	return scrap_4_tmp;
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

// スクラップサイズの更新(1:1の場合)
void convert_scrap_size(int *width, int *height, int direction){
	switch(direction){
		case DIRE_U:
		case DIRE_D:
			height += 1;
			break;
		case DIRE_R:
		case DIRE_L:
			width += 1;
			break;
	}
}

// 方向を逆にする
int inverse_direction(int dire){
	int inv_dire = -1;
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

string IntToString(int num){
	stringstream ss;
	ss << num;
	return ss.str();
}

// コンストラクタ　いろいろと初期化
PPMFILE::PPMFILE(cv::Mat origin_img_tmp, int piece_x, int piece_y){
	origin_img = origin_img_tmp.clone();
	part_size_x = piece_x;
	part_size_y = piece_y;

	part_px_x = origin_img.cols / part_size_x;
	part_px_y = origin_img.rows / part_size_y;

	//それぞれのピースに分割
	this->create_partition();
}

// 画像を表示するための関数　引数によって画像が変わる
// 予めcv::Mat形式のデータを入れておく必要がある
void PPMFILE::disp_img(int type){
	switch (type){
		case ORIGIN_IMG:
			cv::namedWindow("origin_image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
			cv::imshow("origin_image", origin_img);
			break;
		case LINE_IMG:
			cv::namedWindow("line_image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
			cv::imshow("line_image", line_img);
			break;
		case RESULT_IMG:
			cv::namedWindow("result_image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
			cv::imshow("result_image", result_img);
			break;
		case NUM_IMG:
			cv::namedWindow("num_image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
			cv::imshow("num_image", num_img);
			break;
		default:
							break;
	}
}

// 画像に境界線のラインを引く
void PPMFILE::write_line(void){
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

// 画像に番号を入れる
void PPMFILE::create_num_img(void){
	num_img = origin_img.clone(); // origin_img をコピー　
	for(int y=0; y < part_size_y; y++){
		for(int x=0; x < part_size_x; x++){
			cv::putText(num_img, IntToString((y*part_size_x)+x), cv::Point( (x * part_px_x) + (part_px_x/2) - 0, part_px_y + part_px_y*y - 5), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(0,0,200), 4, CV_AA);
		}
	}
}

// 画像をサイズによって分割する
void PPMFILE::create_partition(void){
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

// コスト計算法(ただの差分)
void PPMFILE::calc_cost(void){
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
			// 上下に対しては縦のピクセル数、左右に対しては横のピクセル数
			// を×る事によって、結合度の重みを長さに依存させない
			// ワンちゃんオーバーフローが恐い(たぶん大丈夫)
			cost_tmp[DIRE_U] *= part_img[0].rows;
			cost_tmp[DIRE_D] *= part_img[0].rows;
			cost_tmp[DIRE_R] *= part_img[0].cols;
			cost_tmp[DIRE_L] *= part_img[0].cols;
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

// まるさん方式のコスト計算法(横方向のみ)
void PPMFILE::calc_cost_maru(void){
	// costのサイズ変更
	cost_maru.resize( part_size_x*part_size_y);
	for(int i=0; i<part_size_x*part_size_y; i++){
		cost_maru[i].resize(4);
		for(int j=0; j<4; j++){
			cost_maru[i][j].resize(part_size_x*part_size_y);
		}
	}

	cout << "start_calc_maru" << endl;

	int cost_tmp[4];
	// 差分積算量は数が少ない方から多い方にじゃないと調べられない
	for(int abs_xy=0; abs_xy < part_size_x * part_size_y; ++abs_xy){
		for(int xy=abs_xy+1; xy < part_size_x * part_size_y; ++xy){
			//コスト初期化
			for(int i=0; i<4; i++){
				cost_tmp[i] = 0;
			}
			//カラーの数だけ繰り返す
			for(int c=1; c < part_img[abs_xy].channels(); ++c){
				//各ピクセル上下
				for(int px_x=0; px_x < part_img[0].cols; ++px_x){
					cost_tmp[DIRE_U] += abs(part_img[abs_xy].at<cv::Vec3b>( 0, px_x)[c] - abs(part_img[abs_xy].at<cv::Vec3b>( 1, px_x)[c] + part_img[xy].at<cv::Vec3b>( part_img[0].rows-1, px_x)[c])/2);
					cost_tmp[DIRE_D] += abs(part_img[abs_xy].at<cv::Vec3b>( part_img[0].rows-1, px_x)[c] - abs(part_img[xy].at<cv::Vec3b>( 0, px_x)[c] + part_img[abs_xy].at<cv::Vec3b>( part_img[0].rows-2, px_x)[c])/2);
				}
				//各ピクセル左右
				for(int px_y=0; px_y < part_img[0].rows; ++px_y){
					cost_tmp[DIRE_L] += abs(part_img[abs_xy].at<cv::Vec3b>( px_y, 0)[c] - abs(part_img[abs_xy].at<cv::Vec3b>( px_y, 1)[c] + part_img[xy].at<cv::Vec3b>( px_y, part_img[0].cols-1)[c])/2);
					cost_tmp[DIRE_R] += abs(part_img[abs_xy].at<cv::Vec3b>( px_y, part_img[0].cols-1)[c] - abs(part_img[xy].at<cv::Vec3b>( px_y, 0)[c] + part_img[abs_xy].at<cv::Vec3b>( px_y, part_img[0].cols-2)[c])/2);
				}
			}

			// 上下に対しては縦のピクセル数、左右に対しては横のピクセル数
			// を×る事によって、結合度の重みを長さに依存させない
			// ワンちゃんオーバーフローが恐い(たぶん大丈夫)

			cost_tmp[DIRE_U] *= part_img[0].rows;
			cost_tmp[DIRE_D] *= part_img[0].rows;
			cost_tmp[DIRE_R] *= part_img[0].cols;
			cost_tmp[DIRE_L] *= part_img[0].cols;

			// cost_t [ コスト, 自分の座標, 相手の座標, 方向]
			// cost [自分の座標][方向][相手の座標]
			for(int k=0; k < 4; ++k){
				cost_t.push_back(make_tuple( cost_tmp[k], abs_xy, xy, k));
				cost[abs_xy][k][xy] = make_pair( cost_tmp[k], xy);
			}
		}
	}

	cout << "start_sort_maru" << endl;
	// 独自のルールでCOST_TUPLEをソート
	sort( cost_t_maru.begin(), cost_t_maru.end(), my_compare);
	cout << "end calc" << endl;
}

// コストの一覧を表示していく
void PPMFILE::disp_cost_list(int cost_select){
	// 表示
	cout << "score : my_pos : pair_pos : direction " << endl;
	switch( cost_select){
		case COST_DEF:
			cout << "-----cost_def-----" << endl;
			for(int i=0; i < cost_t.size(); i++){
				cout << i << " |cost: " << get<0>(cost_t[i]) << ", (" << CONV_X( get<1>(cost_t[i])) << "," << CONV_Y( get<1>(cost_t[i])) << "), (" << CONV_X(get<2>(cost_t[i])) << "," << CONV_Y(get<2>(cost_t[i])) << "), " << get<3>(cost_t[i]) << ", " << endl;
			}
			break;

		case COST_MARU:
			cout << "-----cost_maru-----" << endl;
			for(int i=0; i < cost_t_maru.size(); i++){
				cout << i << " |cost: " << get<0>(cost_t_maru[i]) << ", (" << CONV_X( get<1>(cost_t_maru[i])) << "," << CONV_Y( get<1>(cost_t_maru[i])) << "), (" << CONV_X(get<2>(cost_t_maru[i])) << "," << CONV_Y(get<2>(cost_t_maru[i])) << "), " << get<3>(cost_t_maru[i]) << ", " << endl;
			}
			break;

		case COST_ALL:
			cout << "-----cost_all-----" << endl;
			for(int i=0; i < cost_t_all.size(); i++){
				cout << i << " |cost: " << get<0>(cost_t_all[i]) << ", (" << CONV_X( get<1>(cost_t_all[i])) << "," << CONV_Y( get<1>(cost_t_all[i])) << "), (" << CONV_X(get<2>(cost_t_all[i])) << "," << CONV_Y(get<2>(cost_t_all[i])) << "), " << get<3>(cost_t_all[i]) << ", " << endl;
			}
			break;
	}
}

// 横方向に要素を結合していく
void PPMFILE::add_side( SCRAP &scrap_tmp, int p1_x, int p1_y, int p2_x, int p2_y){
	int p1, p2;
	int p1_r, p2_r;
	pair<int,int> p1_p, p2_p;

	p1_p = make_pair( p1_x, p1_y);
	p2_p = make_pair( p2_x, p2_y);
	p1 = scrap_tmp.used_p[p1_p]; // 上にあるピース
	p2 = scrap_tmp.used_p[p2_p]; // 下にあるピース
	cout << "add side" << endl;
	cout << "p1 : " << p1 << " p2 : " << p2 << endl;
	// それぞれの右側の確率が一番高いピース
	p1_r = cost_all[p1][DIRE_R][0].second ;
	p2_r = cost_all[p2][DIRE_R][0].second ;
	// p1_rの下に来る確率が一番高いのがp2_rだったらそいつらは決定
	if(cost_all[p1_r][DIRE_D][0].second == p2_r){
	}

	scrap_tmp.used_p[p1_p] = 100;

	// 左側に ,,
	p1_r = cost_all[p1][3][0].second ;
	p2_r = cost_all[p2][3][0].second ;
}

// 上下方向に結合していく

// 画像を配置していく
void PPMFILE::placement(void){
	vector<SCRAP> scraps;
	// 使われたかどうかのフラグ
	vector<int> used_part(part_size_x*part_size_y, -1);
	// 目的の場所への差分
	int dif_x, dif_y;
	int part_s, part_l;
	int part_s_x, part_s_y;
	int part_l_x, part_l_y;
	int part_1, part_2;
	int part_1_x, part_1_y;
	int part_2_x, part_2_y;

	// 方向を表すペア
	pair<int,int> dire_pair;

	// 再配置の時に使う
	int pos_x_min = 0;
	int pos_y_min = 0;

	// すべてのコストについて実行
	for(int i=0; i < cost_t.size(); i++){
		SCRAP scrap_tmp;
		part_1 = used_part[get<1>(cost_t[i])];
		part_2 = used_part[get<2>(cost_t[i])];

		if(part_1 == -1){
			if(part_2 == -1){ // どちらも使われていない
				scrap_tmp.width = scrap_tmp.height = 0;
				// 追加したscrap番号を保持
				used_part[get<1>(cost_t[i])] = used_part[get<2>(cost_t[i])] = scraps.size();
				scrap_tmp.elements[get<1>(cost_t[i])] = make_pair( 0, 0);
				scrap_tmp.used_p[make_pair(0,0)] = get<1>(cost_t[i]);
				dire_pair = make_direction_pair(get<3>(cost_t[i]));
				scrap_tmp.elements[get<2>(cost_t[i])] = dire_pair;
				scrap_tmp.used_p[dire_pair] = get<2>(cost_t[i]);
				convert_scrap_size( &scrap_tmp.width, &scrap_tmp.height, get<3>(cost_t[i]));

				// 上下か左右によって条件分岐 (とりあえず、追加方式はナシで)
				// switch(get<3>(cost_t[i])){
				// 	case DIRE_U:
				// 		add_side( scrap_tmp, 0,0, 0,-1);
				// 		break;
				// 	case DIRE_D:
				// 		add_side( scrap_tmp, 0,0, 0,1);
				// 		break;
				// 	case DIRE_R:
				// 		// add_up( scrap_tmp, 0,0, 1,0);
				// 		break;
				// 	case DIRE_L:
				// 		// add_up( scrap_tmp, 0,0, -1,0);
				// 		break;
				// }
				// cout << "scrap_tmp used_p change ? " << scrap_tmp.used_p[make_pair(0,0)] << endl;

				scraps.push_back(scrap_tmp);
			}
			else{						// 2だけつかわれてる
				part_2_x = scraps[part_2].elements[get<2>(cost_t[i])].first;
				part_2_y = scraps[part_2].elements[get<2>(cost_t[i])].second;
				// 目的の場所(2が基準だから左右が逆になる)
				switch(get<3>(cost_t[i])){
					case DIRE_U:
						++part_2_y;
						break;
					case DIRE_D:
						--part_2_y;
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
					convert_scrap_size( &scrap_tmp.width, &scrap_tmp.height, get<3>(cost_t[i]));
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
						--part_1_y;
						break;
					case DIRE_D:
						++part_1_y;
						break;
					case DIRE_R:
						++part_1_x;
						break;
					case DIRE_L:
						--part_1_x;
						break;
				}
				// 目的の場所に要素が入っていなかった場合
				if(scraps[part_1].used_p.find(make_pair(part_1_x,part_1_y)) == scraps[part_1].used_p.end()){
					used_part[get<2>(cost_t[i])] = part_1;
					scraps[part_1].elements[get<2>(cost_t[i])] = make_pair( part_1_x, part_1_y);
					scraps[part_1].used_p[make_pair( part_1_x, part_1_y)] = get<2>(cost_t[i]);
					convert_scrap_size( &scrap_tmp.width, &scrap_tmp.height, get<3>(cost_t[i]));
				}else{
					// 既に要素が入っていた場合
				}
			}else{	// 両方ともつかわれている -> スクラップの添字が小さい方に大きい方をくっつける
				if( part_1 != part_2){  // 同じスクラップでない場合
					part_s = MIN_2( part_1, part_2);
					part_l = BIG_2( part_1, part_2);

#ifdef DEBUG
					cout << "now data " << endl;
					for(int i=0; i < scraps.size(); i++){
						cout << "pair : " << i << endl;
						for(map<int, pair<int,int> >::iterator j = scraps[i].elements.begin(); j != scraps[i].elements.end(); j++){
							int key = j->first;
							pair<int, int> pos = j->second;
							cout << "  (" << CONV_X(key) << "," << CONV_Y(key) << ") || (" << pos.first << "," << pos.second << ")" << endl;
						}
					}
					cout << "part_1 : " << part_1 << endl;
					cout << "marg : " << part_s << " to " << part_l << endl;
					cout << "PP1 : (" << CONV_X(get<1>(cost_t[i])) << "," << CONV_Y(get<1>(cost_t[i])) << ")" << endl;
					cout << "PP2 : (" << CONV_X(get<2>(cost_t[i])) << "," << CONV_Y(get<2>(cost_t[i])) << ")" << endl;
					cout << "discription : " << get<3>(cost_t[i]) << endl;
#endif
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
					for(int i=0; i < scraps.size(); i++){
						cout << "pair : " << i << endl;
						for(map<int, pair<int,int> >::iterator j = scraps[i].elements.begin(); j != scraps[i].elements.end(); j++){
							int key = j->first;
							pair<int, int> pos = j->second;
							cout << "  (" << CONV_X(key) << "," << CONV_Y(key) << ") || (" << pos.first << "," << pos.second << ")" << endl;
						}
					}
				}
			}
		}
	}
	// 座標の左上を0,0にする
	// この時点でscrapsは[0]しか要素を持たないはず
	for(map<int, pair<int,int> >::iterator j = scraps[0].elements.begin(); j != scraps[0].elements.end(); j++){
		pair<int, int> pos = j->second;
		if(pos.first < pos_x_min)
			pos_x_min = pos.first;
		if(pos.second < pos_y_min)
			pos_y_min = pos.second;
	}
	// 一番小さいものに座標を合わせて再配置
	for(map<int, pair<int,int> >::iterator j = scraps[0].elements.begin(); j != scraps[0].elements.end(); j++){
		int key = j->first;
		pair<int, int> pos = j->second;
		scraps[0].elements[key] = make_pair( (pos.first - pos_x_min), (pos.second - pos_y_min));
	}
	// グローバルに結果をコピー
	placement_pos = scraps[0].elements;
}

// 最初に、確実にあってると思われる4ピースを検出する
void PPMFILE::new_placement(void){
	vector<int> used_part(part_size_x * part_size_y, -1);

	int p1, p2;

	int core_flg = 0;

	// 最初にある２つのペアからつなげて4つにする
	for(int i=0; i< cost_t.size() && core_flg == 0; i++){
		p1 = get<1>(cost_t[i]); // p1,p2はそれぞれもとの場所を指す
		p2 = get<2>(cost_t[i]);

		int p1_right = cost_all[p1][DIRE_R][0].second;
		int p2_right = cost_all[p2][DIRE_R][0].second;
		int p1_right_up = cost_all[p1_right][DIRE_U][0].second;
		int p1_right_down = cost_all[p1_right][DIRE_D][0].second;

		int p1_up = cost_all[p1][DIRE_U][0].second;
		int p2_up = cost_all[p2][DIRE_U][0].second;
		int p1_up_right = cost_all[p1_up][DIRE_R][0].second;
		int p1_up_left = cost_all[p1_up][DIRE_L][0].second;

		// とりあえず、それらのピースに対して右側と上側しかみてないけどたぶんヒットする
		// というか、必ず同じ場所を回ってくるはず。
		switch(get<3>(cost_t[i])){
			case DIRE_U:
				if(p1_right_up == p2_right){
					cout << "sucsess U" << endl;
					used_part[p1_right] = used_part[p2_right] = 1;
					core_flg = 1;
				}
				break;
			case DIRE_D:
				if(p1_right_down == p2_right){
					cout << "sucsess D" << endl;
					used_part[p1_right] = used_part[p2_right] = 1;
					core_flg = 1;
				}
				break;
			case DIRE_R:
				if(p1_up_right == p2_up){
					cout << "sucsess R" << endl;
					cout << "p1 : " << p1 << " p2 : " << p2 << " p1_up : " << p1_up << " p2_up : " << p2_up << " p1_up_right : " << p1_up_right << endl;
					used_part[p1_up] = used_part[p2_up] = 1;
					core_flg = 1;
				}
				break;
			case DIRE_L:
				if(p1_up_left == p2_up){
					cout << "sucsess L" << endl;
					used_part[p1_up] = used_part[p2_up] = 1;
					core_flg = 1;
				}
				break;
		}
	}
	used_part[p1] = used_part[p2] = 1;
}


// 4ピースセット作戦
// 戦略としては、すべてのピースに対して自分の近くの4ピースを検索する。
// 自分付近の4ピースは計4つあるが、これについては最も良い物を作成する
// 1*n n*1 の場合には正しく動作しない
void PPMFILE::placement_4(void){
	// <コスト, ルート, 自分の座標, 遷移1, 遷移2, 遷移3, 最強点の数>
	// ルートは 0: URDL / 1: RDLU / 2: DLUR / 3:LURD
	vector<vector<tuple< int, int, int, int, int, int, int> > > less_route_pos;

	// ピースNo, 相対X, 相対Y
	//vector<vector<tuple< int, int, int> > > scrap_4(part_size_x*part_size_y);
	// scrap_4[ピースNo][組み合わせベスト4]
	vector<vector< SCRAP_4> > scrap_4;
	vector<vector< SCRAP_4> > scrap_4_backup;
	vector<int> used_part(part_size_x * part_size_y, 0);

	SCRAP_4 scrap_4_tmp;

	int p1_pos, p1_cost;
	int p2_pos, p2_cost;
	int p3_pos, p3_cost;
	int p4_cost; // これは、1周回ってきたときのコスト

	// tuple配列の初期化
	less_route_pos.resize( part_size_x * part_size_y);
	for(int i=0; i<part_size_x * part_size_y; i++){
		// 2^31-1 = 2147483647
		less_route_pos[i].resize(4);
		for(int j=0; j<4; j++){
			less_route_pos[i][j] = make_tuple( 2147483647, 0, 0, 0, 0, 0, 0);
		}
	}

	// scrap_4の初期化
	scrap_4.resize( part_size_x * part_size_y);
	scrap_4_backup.resize( part_size_x * part_size_y);
	for(int i=0; i<part_size_x * part_size_y; i++){
		scrap_4[i].resize(4);
		scrap_4_backup[i].resize(4);
	}

	// すべてのピースに対して
	// この中のj,k,lの値をいじると、深さが変えられる
	for(int i=0; i<part_size_x * part_size_y; i++){
		// 上右下左
		for(int j=0; j<DEEP_SERACH; j++){
			p1_cost = cost_all[i][DIRE_U][j].first;
			p1_pos = cost_all[i][DIRE_U][j].second;
			while(p1_pos == i){ // 同じものを選択した場合
				p1_cost = cost_all[i][DIRE_U][++j].first;
				p1_pos = cost_all[i][DIRE_U][++j].second;
			}
			for(int k=0; k<DEEP_SERACH; k++){
				p2_cost = cost_all[p1_pos][DIRE_R][k].first;
				p2_pos = cost_all[p1_pos][DIRE_R][k].second;
				while(p2_pos == p1_pos || p2_pos == i){ // 同じのを選択した場合
					p2_cost = cost_all[p1_pos][DIRE_R][++k].first;
					p2_pos = cost_all[p1_pos][DIRE_R][++k].second;
				}
				for(int l=0; l<DEEP_SERACH; l++){
					p3_cost = cost_all[p2_pos][DIRE_D][l].first;
					p3_pos = cost_all[p2_pos][DIRE_D][l].second;
					while(p3_pos == p2_pos || p3_pos == p1_pos || p3_pos == i){ // 同じのを選択した場合
						p3_cost = cost_all[p2_pos][DIRE_R][++l].first;
						p3_pos = cost_all[p2_pos][DIRE_R][++l].second;
					}
					p4_cost = cost_all_def[p3_pos][DIRE_L][i].first;
					if((p1_cost*BORDER_WEIGHT + p2_cost + p3_cost + p4_cost*BORDER_WEIGHT) < get<0>(less_route_pos[i][0])){
						less_route_pos[i][0] = make_tuple( p1_cost*BORDER_WEIGHT + p2_cost + p3_cost + p4_cost*BORDER_WEIGHT, 0, i, p1_pos, p2_pos, p3_pos, 0);
					}
				}
			}
		}
		// 右下左上
		for(int j=0; j<DEEP_SERACH; j++){
			p1_cost = cost_all[i][DIRE_R][j].first;
			p1_pos = cost_all[i][DIRE_R][j].second;
			while(p1_pos == i){ // 同じのを選択した場合
				p1_cost = cost_all[i][DIRE_U][++j].first;
				p1_pos = cost_all[i][DIRE_U][++j].second;
			}
			for(int k=0; k<DEEP_SERACH; k++){
				p2_cost = cost_all[p1_pos][DIRE_D][k].first;
				p2_pos = cost_all[p1_pos][DIRE_D][k].second;
				while(p2_pos == p1_pos || p2_pos == i){ // 同じのを選択した場合
					p2_cost = cost_all[p1_pos][DIRE_R][++k].first;
					p2_pos = cost_all[p1_pos][DIRE_R][++k].second;
				}
				for(int l=0; l<DEEP_SERACH; l++){
					p3_cost = cost_all[p2_pos][DIRE_L][l].first;
					p3_pos = cost_all[p2_pos][DIRE_L][l].second;
					while(p3_pos == p2_pos || p3_pos == p1_pos || p3_pos == i){ // 同じのを選択した場合
						p3_cost = cost_all[p2_pos][DIRE_R][++l].first;
						p3_pos = cost_all[p2_pos][DIRE_R][++l].second;
					}
					p4_cost = cost_all_def[p3_pos][DIRE_U][i].first;
					if((p1_cost*BORDER_WEIGHT + p2_cost + p3_cost + p4_cost*BORDER_WEIGHT) < get<0>(less_route_pos[i][1]))
						less_route_pos[i][1] = make_tuple( p1_cost*BORDER_WEIGHT + p2_cost + p3_cost + p4_cost*BORDER_WEIGHT, 1, i, p1_pos, p2_pos, p3_pos, 0);
				}
			}
		}
		// 下左上右
		for(int j=0; j<DEEP_SERACH; j++){
			p1_cost = cost_all[i][DIRE_D][j].first;
			p1_pos = cost_all[i][DIRE_D][j].second;
			while(p1_pos == i){ // 同じのを選択した場合
				p1_cost = cost_all[i][DIRE_U][++j].first;
				p1_pos = cost_all[i][DIRE_U][++j].second;
			}
			for(int k=0; k<DEEP_SERACH; k++){
				p2_cost = cost_all[p1_pos][DIRE_L][k].first;
				p2_pos = cost_all[p1_pos][DIRE_L][k].second;
				while(p2_pos == p1_pos || p2_pos == i){ // 同じのを選択した場合
					p2_cost = cost_all[p1_pos][DIRE_R][++k].first;
					p2_pos = cost_all[p1_pos][DIRE_R][++k].second;
				}
				for(int l=0; l<DEEP_SERACH; l++){
					p3_cost = cost_all[p2_pos][DIRE_U][l].first;
					p3_pos = cost_all[p2_pos][DIRE_U][l].second;
					while(p3_pos == p2_pos || p3_pos == p1_pos || p3_pos == i){ // 同じのを選択した場合
						p3_cost = cost_all[p2_pos][DIRE_R][++l].first;
						p3_pos = cost_all[p2_pos][DIRE_R][++l].second;
					}
					p4_cost = cost_all_def[p3_pos][DIRE_R][i].first;
					if((p1_cost*BORDER_WEIGHT + p2_cost + p3_cost + p4_cost*BORDER_WEIGHT) < get<0>(less_route_pos[i][2]))
						less_route_pos[i][2] = make_tuple( p1_cost*BORDER_WEIGHT + p2_cost + p3_cost + p4_cost*BORDER_WEIGHT, 2, i, p1_pos, p2_pos, p3_pos, 0);
				}
			}
		}
		// 左上右下
		for(int j=0; j<DEEP_SERACH; j++){
			p1_cost = cost_all[i][DIRE_L][j].first;
			p1_pos = cost_all[i][DIRE_L][j].second;
			while(p1_pos == i){ // 同じのを選択した場合
				p1_cost = cost_all[i][DIRE_U][++j].first;
				p1_pos = cost_all[i][DIRE_U][++j].second;
			}
			for(int k=0; k<DEEP_SERACH; k++){
				p2_cost = cost_all[p1_pos][DIRE_U][k].first;
				p2_pos = cost_all[p1_pos][DIRE_U][k].second;
				while(p2_pos == p1_pos || p2_pos == i){ // 同じのを選択した場合
					p2_cost = cost_all[p1_pos][DIRE_R][++k].first;
					p2_pos = cost_all[p1_pos][DIRE_R][++k].second;
				}
				for(int l=0; l<DEEP_SERACH; l++){
					p3_cost = cost_all[p2_pos][DIRE_R][l].first;
					p3_pos = cost_all[p2_pos][DIRE_R][l].second;
					while(p3_pos == p2_pos || p3_pos == p1_pos || p3_pos == i){ // 同じのを選択した場合
						p3_cost = cost_all[p2_pos][DIRE_R][++l].first;
						p3_pos = cost_all[p2_pos][DIRE_R][++l].second;
					}
					p4_cost = cost_all_def[p3_pos][DIRE_D][i].first;
					if((p1_cost*BORDER_WEIGHT + p2_cost + p3_cost + p4_cost*BORDER_WEIGHT) < get<0>(less_route_pos[i][3]))
						less_route_pos[i][3] = make_tuple( p1_cost*BORDER_WEIGHT + p2_cost + p3_cost + p4_cost*BORDER_WEIGHT, 3, i, p1_pos, p2_pos, p3_pos, 0);
				}
			}
		}
	}

	// とりあえず現状表示
	for(int i=0; i<less_route_pos.size(); i++){
		for(int j=0; j<less_route_pos[i].size(); j++){
			cout << "---- first " << i << " ----" << endl;
			cout <<
				"0:" << get<0>(less_route_pos[i][j]) << ":" << endl <<
				"1:" << get<1>(less_route_pos[i][j]) << ":" << endl <<
				"2:" << get<2>(less_route_pos[i][j]) << ":" << endl <<
				"3:" << get<3>(less_route_pos[i][j]) << ":" << endl <<
				"4:" << get<4>(less_route_pos[i][j]) << ":" << endl <<
				"5:" << get<5>(less_route_pos[i][j]) << ":" << endl <<
				"6:" << get<6>(less_route_pos[i][j]) << endl;
		}
	}

	// 最強の強さをさらに高める？(strongの利用)
	int strong_p;
	for(int i=0; i< part_size_x * part_size_y; i++){
		// ルート0
		strong_p = 0;
		if(get<3>(less_route_pos[i][0]) == get<5>(less_route_pos[i][3]))
			strong_p++;
		if(get<5>(less_route_pos[i][0]) == get<3>((less_route_pos[i][1])))
			strong_p++;
		less_route_pos[i][0] = make_tuple( get<0>(less_route_pos[i][0]), get<1>(less_route_pos[i][0]), get<2>(less_route_pos[i][0]), get<3>(less_route_pos[i][0]), get<4>(less_route_pos[i][0]), get<5>(less_route_pos[i][0]), strong_p);

		// ルート1
		strong_p = 0;
		if(get<3>(less_route_pos[i][1]) == get<5>(less_route_pos[i][0]))
			strong_p++;
		if(get<5>(less_route_pos[i][1]) == get<3>((less_route_pos[i][2])))
			strong_p++;
		less_route_pos[i][1] = make_tuple( get<0>(less_route_pos[i][1]), get<1>(less_route_pos[i][1]), get<2>(less_route_pos[i][1]), get<3>(less_route_pos[i][1]), get<4>(less_route_pos[i][1]), get<5>(less_route_pos[i][1]), strong_p);

		// ルート2
		strong_p = 0;
		if(get<3>(less_route_pos[i][2]) == get<5>(less_route_pos[i][1]))
			strong_p++;
		if(get<5>(less_route_pos[i][2]) == get<3>((less_route_pos[i][3])))
			strong_p++;
		less_route_pos[i][2] = make_tuple( get<0>(less_route_pos[i][2]), get<1>(less_route_pos[i][2]), get<2>(less_route_pos[i][2]), get<3>(less_route_pos[i][2]), get<4>(less_route_pos[i][2]), get<5>(less_route_pos[i][2]), strong_p);

		// ルート3
		strong_p = 0;
		if(get<3>(less_route_pos[i][3]) == get<5>(less_route_pos[i][2]))
			strong_p++;
		if(get<5>(less_route_pos[i][3]) == get<3>((less_route_pos[i][0])))
			strong_p++;
		less_route_pos[i][3] = make_tuple( get<0>(less_route_pos[i][3]), get<1>(less_route_pos[i][3]), get<2>(less_route_pos[i][3]), get<3>(less_route_pos[i][3]), get<4>(less_route_pos[i][3]), get<5>(less_route_pos[i][3]), strong_p);

		sort(less_route_pos[i].begin(), less_route_pos[i].end(), sort_scrap_4_strong);
	}

	// コストを相対座標に変換
	for(int i=0; i<part_size_x * part_size_y; i++){
		for(int j=0; j<4; j++){
			scrap_4[i][j] = create_scrap_4_element( less_route_pos[i][j]);
			scrap_4_backup[i][j] = create_scrap_4_element( less_route_pos[i][j]);
		}
	}
	// とりあえず現状表示
	for(int i=0; i<scrap_4.size(); i++){
		cout << "---- now " << i << " ----" << endl;
		for(map<int, pair<int,int> >::iterator j = scrap_4[i][0].elements.begin(); j != scrap_4[i][0].elements.end(); j++){
			int key = j->first;
			pair<int, int> pos = j->second;
			// cout << "(" << CONV_X(key) << "," << CONV_Y(key) << ") " << " (" << pos.first << "," << pos.second << ")" << endl;
			cout << "(" << key << ") " << " (" << pos.first << "," << pos.second << ")" << endl;
		}
	}

	// 2つ以上かぶっているものは結合
	// すべて被ってるのは片方削除
	int conf_cnt;
	int diff_x, diff_y;

	int diff_flg;	// 0ならすべて一致 それ以外だと一致してない場所がある
	vector<pair<int,int> > p1_abs; // それぞれの一致してるパーツの相対座標を
	vector<pair<int,int> > p2_abs; // 順番に保持していく

	// 適当に4回ぐらい繰り返せば全部繋がるっしょwww
	// 最初の統合 2ピースかぶりを繋げていく
	for(int ii=0; ii<4; ii++){
		for(int i=0; i<scrap_4.size()-1; i++){
			for(int j=i+1; j<scrap_4.size(); j++){
				conf_cnt = 0;
				for(map<int, pair<int,int> >::iterator k = scrap_4[i][0].elements.begin(); k != scrap_4[i][0].elements.end(); k++){
					int key = k->first;
					if(scrap_4[j][0].elements.find(key) == scrap_4[j][0].elements.end()){
						// かぶっていない
					}else{
						// 共有しているピースの数をカウントする
						conf_cnt++;
					}
				}
				// どちらかにすべて含まれていたら
				if(conf_cnt == MIN_2( scrap_4[j][0].elements.size(), scrap_4[i][0].elements.size()) && conf_cnt >= 4){
					if(scrap_4[i][0].elements.size() >= scrap_4[j][0].elements.size()){
						// scrap_4[j][0]のほうを消す
						scrap_4[j][0].used.clear();
						scrap_4[j][0].elements.clear();
					}else{
						scrap_4[i][0] = scrap_4[j][0];

						scrap_4[j][0].used.clear();
						scrap_4[j][0].elements.clear();
					}
				}else if(conf_cnt >= 2){ // 2つ以上かぶっていたら
					// diffを検索
					p1_abs.clear();
					p2_abs.clear();
					for(map<int, pair<int,int> >::iterator k = scrap_4[i][0].elements.begin(); k != scrap_4[i][0].elements.end(); k++){
						int key = k->first;
						pair<int, int> pos = k->second;
						if(scrap_4[j][0].elements.find(key) == scrap_4[j][0].elements.end()){
						}else{
							// 一致している要素
							p1_abs.push_back( make_pair(scrap_4[i][0].elements[key].first, scrap_4[i][0].elements[key].second));
							p2_abs.push_back( make_pair(scrap_4[j][0].elements[key].first, scrap_4[j][0].elements[key].second));

							diff_x = scrap_4[i][0].elements[key].first - scrap_4[j][0].elements[key].first;
							diff_y = scrap_4[i][0].elements[key].second - scrap_4[j][0].elements[key].second;
							cout << "scr : " << j << ":" << i << "  conf : " << key << " ";
							//break;
						}
					}
					// すべての相対座標のズレが一致しているか?
					diff_flg = 0;
#ifdef MARGE_CHK_POS
					for(int c=0; c<p1_abs.size(); c++){
						if(diff_x == p1_abs[c].first - p2_abs[c].first && diff_y == p1_abs[c].second - p2_abs[c].second){
						}else{
							// 一致していなかったら
							diff_flg = 1;
						}
					}
#endif
					// diffを用いて結合
					if(diff_flg == 0){
						for(map<int, pair<int,int> >::iterator k = scrap_4[j][0].elements.begin(); k != scrap_4[j][0].elements.end(); k++){
							int key = k->first;
							pair<int, int> pos = k->second;
							if(scrap_4[i][0].elements.find(key) == scrap_4[i][0].elements.end()){
								// 追加
								pair<int,int> tmp_pair = make_pair( pos.first + diff_x, pos.second + diff_y);
								int tmp_pos;
#ifdef USE_DONT_CONFRICT
								// 上書きしようとした時の対応をどうするか。。。
								if(scrap_4[i][0].used.find(tmp_pair) == scrap_4[i][0].used.end()){
								}else{
									// すでに何か要素が入っていたら
									tmp_pos = scrap_4[i][0].used[tmp_pair];
									if(tmp_pos != key){
										scrap_4[i][0].elements.erase(tmp_pos);
										cout << "1上書きした" << endl;
										//scrap_4[tmp_pos] = scrap_4_backup[tmp_pos];
									}else{
										// 上書きしようとしてるのが同じなら無視
									}
								}
#endif
								scrap_4[i][0].elements[key] = tmp_pair;
								scrap_4[i][0].used[tmp_pair] = key;
							}else{
								// すでにある -> 無視
								// 無視しないで、一応上書きする
								scrap_4[i][0].elements[key] = make_pair( pos.first + diff_x, pos.second + diff_y);
								scrap_4[i][0].used[make_pair( pos.first + diff_x, pos.second + diff_y)] = key;
							}
						}
						cout << "diff_x : " << diff_x << " diff_y : " << diff_y << endl;
						scrap_4[j][0].elements.clear();
						scrap_4[j][0].used.clear();
					}
				}
			}
		}
	}

	// 状態を表示
	for(int i=0; i<scrap_4.size(); i++){
		cout << "---- new "<< i << " ----" << endl;
		for(map<int, pair<int,int> >::iterator j = scrap_4[i][0].elements.begin(); j != scrap_4[i][0].elements.end(); j++){
			int key = j->first;
			pair<int, int> pos = j->second;
			cout << key << " (" << pos.first << "," << pos.second << ")" << endl;
			//cout << get<0>(scrap_4[i][0].elements[j][0]) << " : (" << get<1>(scrap_4[i][0].elements[j][0]) << "," << get<2>(scrap_4[i][0].elements[j][0]) << ")" << endl;
		}
	}

	// scra_4[0][0]以外にまだ要素が残っていたら
	// 2番目-4番目の近傍値を用いてそいつらを集めてくる
	{
		// これも適当に4回ぐらい。。。(あまりかわらない？
		for(int ii=0; ii<4; ii++){
			for(int n=0; n<4; n++){
				for(int i=1; i<scrap_4.size(); i++){
					if(scrap_4[i][0].elements.size() != 0){
						conf_cnt = 0;
						for(map<int, pair<int,int> >::iterator k = scrap_4[i][n].elements.begin(); k != scrap_4[i][n].elements.end(); k++){
							int key = k->first;
							if(scrap_4[0][0].elements.find(key) == scrap_4[0][0].elements.end()){
								// scrap_4[0][0]にその要素が含まれているか？
							}else{
								// 共有しているピースの数をカウントする
								conf_cnt++;
							}
						}
						// どちらかにすべて含まれていたなら　
						if(conf_cnt == MIN_2( scrap_4[0][0].elements.size(), scrap_4[i][0].elements.size()) && conf_cnt >= 4){
							if(scrap_4[i][0].elements.size() <= scrap_4[0][0].elements.size()){
								// scrap_4[0][0]のほうを消す
								scrap_4[i][0].used.clear();
								scrap_4[i][0].elements.clear();
							}else{
								// これが正しく動作するのかわからない
								scrap_4[0][0] = scrap_4[i][0];

								scrap_4[i][0].used.clear();
								scrap_4[i][0].elements.clear();
							}
						}else if(conf_cnt >= 2){ // 2つ以上かぶっていたら
							// diffを検索
							p1_abs.clear();
							p2_abs.clear();
							for(map<int, pair<int,int> >::iterator k = scrap_4[i][n].elements.begin(); k != scrap_4[i][n].elements.end(); k++){
								int key = k->first;
								pair<int, int> pos = k->second;
								if(scrap_4[0][0].elements.find(key) == scrap_4[0][0].elements.end()){
								}else{
									// 一致している要素
									p1_abs.push_back( make_pair(scrap_4[0][0].elements[key].first, scrap_4[0][0].elements[key].second));
									p2_abs.push_back( make_pair(scrap_4[i][n].elements[key].first, scrap_4[i][n].elements[key].second));

									diff_x = scrap_4[0][0].elements[key].first - scrap_4[i][n].elements[key].first;
									diff_y = scrap_4[0][0].elements[key].second - scrap_4[i][n].elements[key].second;
									//break;
								}
							}
							int diff_x_2 = scrap_4[i][n].elements[i].first - scrap_4[i][0].elements[i].first;
							int diff_y_2 = scrap_4[i][n].elements[i].second - scrap_4[i][0].elements[i].second;
							cout << "dx2 :" << diff_x_2 << "dy2 :" << diff_y_2 << endl;
							// すべての相対座標のズレが一致しているか?
							diff_flg = 0;
#ifdef MERGE_CHK_POS
							for(int c=0; c<p1_abs.size(); c++){
								if(diff_x == p1_abs[c].first - p2_abs[c].first && diff_y == p1_abs[c].second - p2_abs[c].second){
								}else{
									// 一致していなかったら
									diff_flg = 1;
								}
							}
#endif
							// diffを用いて結合
							if(diff_flg == 0){
								for(map<int, pair<int,int> >::iterator k = scrap_4[i][0].elements.begin(); k != scrap_4[i][0].elements.end(); k++){
									int key = k->first;
									pair<int, int> pos = k->second;
									if(scrap_4[0][0].elements.find(key) == scrap_4[0][0].elements.end()){
										// 追加
										// すでにその場所が使われていたら
										// もともと書かれているものをもとに戻す
										pair<int,int> tmp_pair = make_pair( pos.first + diff_x + diff_x_2, pos.second + diff_y + diff_y_2);
										int tmp_pos;
#ifdef USE_DONT_CONFRICT
										if(scrap_4[0][0].used.find(tmp_pair) == scrap_4[0][0].used.end()){
										}else{
											tmp_pos = scrap_4[0][0].used[tmp_pair];
											if(tmp_pos != key){
												scrap_4[0][0].elements.erase(tmp_pos);
												cout << "2上書きした" << endl;
												//scrap_4[tmp_pos] = scrap_4_backup[tmp_pos];
											}else{
												// 同じ座標を上書きなら無視
											}
										}
#endif
										scrap_4[0][0].elements[key] = tmp_pair;
										scrap_4[0][0].used[tmp_pair] = key;
									}else{
										// すでにある -> 無視
									}
								}
							}
							cout << "diff_x : " << diff_x << " diff_y : " << diff_y << endl;
							scrap_4[i][0].elements.clear();
							scrap_4[i][0].used.clear();
						}
					}
				}
			}
		}
	}

	// 座標の変換
	int small_x=0, small_y=0;
	for(int i=0; i<scrap_4.size(); i++){
		for(map<int, pair<int,int> >::iterator j = scrap_4[i][0].elements.begin(); j != scrap_4[i][0].elements.end(); j++){
			pair<int, int> pos = j->second;
			if(pos.first < small_x)
				small_x = pos.first;
			if(pos.second < small_y)
				small_y = pos.second;
		}
		//座標の再配置
		for(map<int, pair<int,int> >::iterator j = scrap_4[i][0].elements.begin(); j != scrap_4[i][0].elements.end(); j++){
			int key = j->first;
			pair<int, int> pos = j->second;
			scrap_4[i][0].elements[key] = make_pair( (pos.first - small_x), (pos.second - small_y));
		}
	}

	// 使われていないピースの計算
	for(map<int, pair<int,int> >::iterator k = scrap_4[0][0].elements.begin(); k != scrap_4[0][0].elements.end(); k++){
		int key = k->first;
		// pair<int,int> pos = k->second;
		used_part[key] = 1;
	}
	cout << "Not Used[0][0]" << endl;
	for(int i=1; i< part_size_x * part_size_y; i++){
		if(used_part[i] == 0){
			// 使われていないピースについて
			cout << "not use : " << i << endl;
#ifdef MERGE_ONE
			for(int n=0; n<4; n++){
				for(map<int, pair<int,int> >::iterator k = scrap_4_backup[i][n].elements.begin(); k != scrap_4_backup[i][n].elements.end(); k++){
					int key = k->first;
					if(scrap_4[0][0].elements.find(key) == scrap_4[0][0].elements.end()){
						// scrap_4[0][0]にその要素が含まれているか？
					}else{
						// 共有しているピースの数をカウントする
						conf_cnt++;
					}
				}
				if(conf_cnt >= 2){ // 2つ以上つながっていたら
					p1_abs.clear();
					p2_abs.clear();
					for(map<int, pair<int,int> >::iterator k = scrap_4_backup[i][n].elements.begin(); k != scrap_4_backup[i][n].elements.end(); k++){
						int key = k->first;
						pair<int, int> pos = k->second;
						if(scrap_4[0][0].elements.find(key) == scrap_4[0][0].elements.end()){
						}else{
							// 一致している要素
							p1_abs.push_back( make_pair(scrap_4[0][0].elements[key].first, scrap_4[0][0].elements[key].second));
							p2_abs.push_back( make_pair(scrap_4_backup[i][n].elements[key].first, scrap_4_backup[i][n].elements[key].second));
							diff_x = scrap_4[0][0].elements[key].first - scrap_4_backup[i][n].elements[key].first;
							diff_y = scrap_4[0][0].elements[key].second - scrap_4_backup[i][n].elements[key].second;
							//break;
						}
					}
					int diff_x_2 = scrap_4_backup[i][0].elements[i].first - scrap_4_backup[i][n].elements[i].first;
					int diff_y_2 = scrap_4_backup[i][0].elements[i].second - scrap_4_backup[i][n].elements[i].second;
					cout << "diff_2_x :" << diff_x_2 << "diff_2_y :" << diff_y_2 << endl;
					// すべての相対座標のズレが一致しているか?
					diff_flg = 0;
#ifdef MERGE_CHK_POS
					for(int c=0; c<p1_abs.size(); c++){
						if(diff_x == p1_abs[c].first - p2_abs[c].first && diff_y == p1_abs[c].second - p2_abs[c].second){
						}else{
							// 一致していなかったら
							diff_flg = 1;
						}
					}
#endif
					// diffを用いて結合
					if(diff_flg == 0){
						for(map<int, pair<int,int> >::iterator k = scrap_4_backup[i][n].elements.begin(); k != scrap_4_backup[i][n].elements.end(); k++){
							int key = k->first;
							pair<int, int> pos = k->second;
							if(scrap_4[0][0].elements.find(key) == scrap_4[0][0].elements.end()){
								// 追加
								pair<int,int> tmp_pair = make_pair( pos.first + diff_x + diff_x_2, pos.second + diff_y + diff_y_2);
								int tmp_pos;
#ifdef USE_DONT_CONFRICT
								if(scrap_4[0][0].used.find(tmp_pair) == scrap_4[0][0].used.end()){
								}else{
									tmp_pos = scrap_4[0][0].used[tmp_pair];
									if(tmp_pos != key){
										//scrap_4[0][0].elements.erase(tmp_pos);
										// この場合は違う座標を上書きはしない
										cout << "3上書きやめました " << endl;
										//scrap_4[tmp_pos] = scrap_4_backup[tmp_pos];
									}else{
										// 同じ座標を上書きなら無視
									}
								}
#endif
								scrap_4[0][0].elements[key] = tmp_pair;
								scrap_4[0][0].used[tmp_pair] = key;
							}else{
								// すでにある -> 無視
							}
						}
					}
				}
			}
#endif
		}
	}

	// 座標の変換
	for(int i=0; i<scrap_4.size(); i++){
		for(map<int, pair<int,int> >::iterator j = scrap_4[i][0].elements.begin(); j != scrap_4[i][0].elements.end(); j++){
			pair<int, int> pos = j->second;
			if(pos.first < small_x)
				small_x = pos.first;
			if(pos.second < small_y)
				small_y = pos.second;
		}
		//座標の再配置
		for(map<int, pair<int,int> >::iterator j = scrap_4[i][0].elements.begin(); j != scrap_4[i][0].elements.end(); j++){
			int key = j->first;
			pair<int, int> pos = j->second;
			scrap_4[i][0].elements[key] = make_pair( (pos.first - small_x), (pos.second - small_y));
		}
	}

	// 最後の状態を表示
	for(int i=0; i<scrap_4.size(); i++){
		cout << "---- new "<< i << " ----" << endl;
		for(map<int, pair<int,int> >::iterator j = scrap_4[i][0].elements.begin(); j != scrap_4[i][0].elements.end(); j++){
			int key = j->first;
			pair<int, int> pos = j->second;
			cout << key << " (" << pos.first << "," << pos.second << ")" << endl;
			//cout << get<0>(scrap_4[i][0].elements[j][0]) << " : (" << get<1>(scrap_4[i][0].elements[j][0]) << "," << get<2>(scrap_4[i][0].elements[j][0]) << ")" << endl;
		}
	}
	// グローバルにコピー
	placement_pos = scrap_4[0][0].elements;
}

// 　placementした結果を表示する
void PPMFILE::disp_placement(void){
	cout << "----------" << endl;
	cout << "placement " << endl;
	for(map<int, pair<int,int> >::iterator j = placement_pos.begin(); j != placement_pos.end(); j++){
		int key = j->first;
		pair<int, int> pos = j->second;
		cout << "  (" << CONV_X(key) << "," << CONV_Y(key) << ") || (" << pos.first << "," << pos.second << ") " << endl;
	}
}

// 完成した画像を表示するためのcv::Mat形式の画像を作成する
void PPMFILE::create_result_img(void){
	int part_width = origin_img.cols/part_size_x;
	int part_height = origin_img.rows/part_size_y;

	int max_part_x = 0, max_part_y = 0;

	// 最大の幅と高さを求める
	for(map<int, pair<int,int> >::iterator j = placement_pos.begin(); j != placement_pos.end(); j++){
		pair<int, int> pos = j->second;
		if(max_part_x < pos.first)
			max_part_x = pos.first;
		if(max_part_y < pos.second)
			max_part_y = pos.second;
	}
	cout << "mpx :" << max_part_x << " mpy :" << max_part_y << endl;

	// 大きさに応じたサイズのresult_imgを作成
	cv::Mat tmp_result_img( cv::Size((max_part_x+1) * part_width, (max_part_y+1) * part_height), CV_8UC3, cv::Scalar(0,0,0));
	// cv::Mat tmp_result_img( cv::Size( 2000, 2000), CV_8UC3, cv::Scalar(0,0,0));

	for(map<int, pair<int,int> >::iterator j = placement_pos.begin(); j != placement_pos.end(); j++){
		int key = j->first;
		pair<int, int> pos = j->second;
		part_img[key].copyTo(tmp_result_img( \
					cv::Rect( \
						pos.first*part_width, \
						pos.second*part_height, \
						part_width, \
						part_height) \
					));
		cout << "s_x :" << pos.first*part_width << " s_y :" <<	pos.second*part_height << " CopyTo:" << key << endl;
	}
	result_img = tmp_result_img.clone();
}

// ソートに渡すためのデータ変換ｎ
void PPMFILE::set_PosData(PosData *data){
	for(map<int, pair<int,int> >::iterator j = placement_pos.begin(); j != placement_pos.end(); j++){
		int key = j->first;
		pair<int, int> pos = j->second;

		data->setData( CONV_X(key), CONV_Y(key), pos.first, pos.second);
	}
}

// 左上のピースを取得してみる
void PPMFILE::get_left_top(){
	vector< pair<int,int> > bad_up( part_size_x * part_size_y);
	vector<bool> used_flg( part_size_x * part_size_y, false);

	cout << " get left top " << endl << endl ;
	sort( cost_t_all.begin(), cost_t_all.end(), my_compare_2);

	int i2=0;
	for(int i=0; i < cost_t_all.size(); i++){
		// 後半のイコールの数字をいじっていい感じのところを持ってくる
		if(used_flg[get<1>(cost_t_all[i])] == false && get<3>(cost_t_all[i]) == 2){
			used_flg[get<1>(cost_t_all[i])] = true;
			cout << " in  :(" << CONV_X(get<1>(cost_t_all[i]))  << "," << CONV_Y(get<1>(cost_t_all[i])) << ") cost" << get<0>(cost_t_all[i]) << endl;
			bad_up[i2] = (make_pair(get<0>(cost_t_all[i]), get<1>(cost_t_all[i])));
			i2++;
		}
	}
	sort(bad_up.begin(), bad_up.end());

	for(int i=0; i < bad_up.size(); i++){
		cout << "score : " << bad_up[i].first << " pos(" << CONV_X(bad_up[i].second) << "," << CONV_Y(bad_up[i].second) << ")" << endl;
	}
}

// コストをムダも含めてすべて計算する
void PPMFILE::calc_cost_all(void){
	// costのサイズ変更
	cost_all.resize( part_size_x*part_size_y);
	cost_all_def.resize( part_size_x*part_size_y);
	for(int i=0; i<part_size_x*part_size_y; i++){
		cost_all[i].resize(4);
		cost_all_def[i].resize(4);
		// for(int j=0; j<4; j++){
		// 	cost_all[i][j].resize(part_size_x*part_size_y);
		// 	cost_all_def[i][j].resize(part_size_x*part_size_y);
		// }
	}
	cout << "start_calc" << endl;
	int cost_tmp[4];
	// すべての方向についてコストの計算を行なう(全てについて反対方向の計算も含んでいる)
	for(int abs_xy=0; abs_xy < part_size_x * part_size_y; ++abs_xy){
		for(int xy=0; xy < part_size_x * part_size_y; ++xy){
			// 自分とのコストは計算しない
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
			// 上下に対しては縦のピクセル数、左右に対しては横のピクセル数
			// を×る事によって、結合度の重みを長さに依存させない
			// ワンちゃんオーバーフローが恐い(たぶん大丈夫)
			cost_tmp[DIRE_U] *= part_img[0].rows;
			cost_tmp[DIRE_D] *= part_img[0].rows;
			cost_tmp[DIRE_R] *= part_img[0].cols;
			cost_tmp[DIRE_L] *= part_img[0].cols;
			// cost_t [ コスト, 自分の座標, 相手の座標, 方向]
			// cost [自分の座標][方向][相手の座標]
			for(int k=0; k < 4; ++k){
				cost_t_all.push_back(make_tuple( cost_tmp[k], abs_xy, xy, k));
				cost_all[abs_xy][k].push_back(make_pair( cost_tmp[k], xy));
				cost_all_def[abs_xy][k].push_back(make_pair( cost_tmp[k], xy));
			}
		}
	}

	// すべてのコスト計算が終わったら、並び替え
	for(int i=0; i<part_size_x * part_size_y; i++){
		for(int j=0; j<4; j++){
			sort(cost_all[i][j].begin(), cost_all[i][j].end());
		}
	}
	// ちょっと表示してみる たぶんあってそう
	// for(int i=0; i<part_size_x * part_size_y; i++){
	// 	for(int j=0; j<4; j++){
	// 		for(int k=0; k<part_size_x * part_size_y; k++){
	// 			cout << "xy : " << i << " " << j << " cost :" << cost_all[i][j][k].first << endl;
	// 		}
	// 	}
	// }

	cout << "start_sort" << endl;
	// 独自のルールでCOST_TUPLEをソート
	sort( cost_t_all.begin(), cost_t_all.end(), my_compare);
	cout << "end calc" << endl;
}

// 回答が最低限の仕様を満たしているか？
bool PPMFILE::chk_result(void){
	vector<int> use_num(part_size_x * part_size_y, 0);
	int max_w=0, max_h=0;
	int un_use=0;
	for(map<int, pair<int,int> >::iterator j = placement_pos.begin(); j != placement_pos.end(); j++){
		int key = j->first;
		pair<int,int> pos = j->second;
		if(max_w < pos.first)
			max_w = pos.first;
		if(max_h < pos.second)
			max_h = pos.second;
		use_num[key] = 1;
	}
	for(int i=0; i<part_size_x * part_size_y; i++){
		if(use_num[i] == 0)
			un_use++;
	}
	// すべてが正しく設定されていたら
	if(un_use == 0 && max_w+1 == part_size_x && max_h+1 == part_size_y)
		return true;
	else
		return false;

	return false;
}
