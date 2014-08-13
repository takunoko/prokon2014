// stlを用いた画像の配置
// openCVの場所
#include <iostream>	//標準入出力
#include <fstream>	//ファイル入出力
#include <string>		//文字列を扱うため

#include <vector>


// openCV関連
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define FILENAME "1.ppm"
#define PIECE_X 16
#define PIECE_Y 16

using namespace std;

void disp_img();

class PPMFILE{
	private:
	public:
};

int main(void){
	disp_img();

	return 0;
}

void disp_img(void){
	vector<string> files;
	files.push_back("../pic_data/1.ppm");

	cv::namedWindow("image1", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::namedWindow("image2", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	cv::namedWindow("image3", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	
	for( vector<string>::iterator it = files.begin(); it!=files.end(); ++it) {
		std::cout << *it << std::endl;
		// 3チャンネル，カラー画像として読み込む．
		cv::Mat img1 = cv::imread(*it, 1);
		// グレースケール画像として読み込む．
		cv::Mat img2 = cv::imread(*it, 0);
		// 画像をそのまま読み込む．ただし，アルファチャンネルは無視される．
		cv::Mat img3 = cv::imread(*it, -1);

		cv::imshow("image1", img1);
		cv::imshow("image2", img2);
		cv::imshow("image3", img3);

		if(cv::waitKey(0)==27) break;
	}
}
