#include <iostream>
#include <fstream>
#include <string>

#include <stdlib.h> /* abs 絶対値 */
#include <stdlib.h> /* rand() */
#include <time.h>   /* time */

#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#define FILENAME "1.ppm"
#define PIECE_X 16
#define PIECE_Y 16

using namespace std;


void display(void);
void init(void);
void resize( int, int);
void Point( int, int, int, int, int, int); 

class PPMFILE{
	private:
		int width, height, bright;

		//image[col][x][y] (メモリの動的確保)
		unsigned char ***image = new unsigned char **[3];
	public:
		void init_data(string filename){
			// バイナリモードで開く
			ifstream fin;
			fin.open(filename, ios::in | ios::binary);

			if(!fin){
				cout << "file : \"" << filename << "\"reading failure." << endl;
				exit(1);
			}else{
				cout << "file : \"" << filename << "\" reading succes." << endl;
			}

			// 1行目の取得 P6:バイナリモード or P3:テキストモード
			string line;
			getline( fin, line);
			cout << "format : " << line << endl;
			// 3行目(width height の取得)
			while (width == 0 || height == 0){
				string line;
				getline( fin,line);
				if(line.at(0) !='#')
					sscanf(line.c_str(),"%d %d",&width, &height);
			}
			cout << "width: " << width << endl << "height: " << height << endl;

			//問題文形式だと、輝度で1行使っている
			getline( fin, line);
			sscanf(line.c_str(),"%d",&bright);
			cout << "bright: " << bright << endl;

			//動的画像保存領域の確保
			for(int i=0; i<3; ++i){
				image[i] = new unsigned char*[width];
				for(int x=0; x<width; ++x){
					image[i][x] = new unsigned char[height];
				}
			}

			//画像の読み込み
			for(int y=0; y<height; ++y){
				for(int x=0; x<width; ++x){
					for(int col=0; col<3; ++col){
						fin.read(reinterpret_cast<char *>(&image[col][x][y]),1);
					}
				}
			}

			fin.close();
		}

		void delete_data(){
			for(int i=0; i<3; ++i){
				for(int x=0; x<width; ++x){
					delete[] image[i][x];
				}
				delete[] image[i];
			}
			delete[] image;
			cout << "create delete memory" << endl;
		}

		void disp_data(){
			for(int y=0; y<height; ++y){
				for(int x=0; x<width; ++x){
					Point( x, y, 1, image[0][x][y],image[1][x][y],image[2][x][y]);
					//赤だけ表示とか
					//Point( x, y, 1, image[0][x][y],0,0);
				}
			}
		}

		//画面サイズをwidth,heightで初期化する
		void imageGlutInitWindowSize(){
			glutInitWindowSize( width, height);
		}

		int get_width(){
			return width;
		}
		int get_height(){
			return height;
		}

		void set_data(int x, int y, int col, unsigned char data){
			image[col][x][y] = data;
		}
		int get_data(int x, int y, int col){
			return image[col][x][y];
		}
};

class PIECE_DATA{
	private:
		int piece_width, piece_height;
		unsigned char ***piece = new unsigned char **[3];

	public:
		int rx, ry;
		int lx, ly;
		int ux, uy;
		int dx, dy;
		double rp, lp, up, dp;

		int re_pos_x, re_pos_y;

		void init_data(int width, int height);
		void set_data(int x, int y, int col, unsigned char data);
		int get_data(int x, int y, int col);
		void delete_data();
		void disp_data_pos(int , int);
};

void PIECE_DATA::init_data(int width, int height){
	piece_width = width;
	piece_height = height;

	//動的画像保存領域の確保
	for(int i=0; i<3; ++i){
		piece[i] = new unsigned char*[piece_width];
		for(int x=0; x<width; ++x){
			piece[i][x] = new unsigned char[piece_height];
		}
	}
}
void PIECE_DATA::set_data(int x, int y, int col, unsigned char data){
	piece[col][x][y] = data;	
}
int PIECE_DATA::get_data(int x, int y, int col){
	return piece[col][x][y];
}
void PIECE_DATA::delete_data(){
	for(int i=0; i<3; ++i){
		for(int x=0; x<piece_width; ++x){
			delete[] piece[i][x];
		}
		delete[] piece[i];
	}
	delete[] piece;
}
void PIECE_DATA::disp_data_pos(int start_x, int start_y){
	for (int y = 0; y < piece_height; y++) {
		for (int x = 0; x < piece_width; x++) {
			Point( start_x+x, start_y+y, 1, piece[0][x][y], piece[1][x][y], piece[2][x][y]);
		}
	}
}

PPMFILE pic_data;
PIECE_DATA **piece;

int piece_x;
int piece_y;

int piece_width, piece_height;

//OpenGL
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	pic_data.disp_data();

	for(int pos_y=0; pos_y<piece_y; pos_y++){
		for(int pos_x=0; pos_x<piece_x; pos_x++){
			piece[pos_x][pos_y].disp_data_pos( piece[pos_x][pos_y].re_pos_x*piece_width, piece[pos_x][pos_y].re_pos_y*piece_height);
			cout << pos_x << ":" << pos_y << "|" << piece[pos_x][pos_y].re_pos_x << ":" << piece[pos_x][pos_y].re_pos_y << endl;
		}
	}
	glFlush();
}

//どれだけ隣り合うドット同士の色が近いのか
//数字分けはとりあえず適当
int compare_dot(int a, int b){
	if (a==b)
		return 10;
	else if(abs(a-b) <= 5)
		return 9;
	else if(abs(a-b) <=10)
		return 4;
	else if(abs(a-b) <=30)
		return 2;
	else
		return 0;
}
	

int main(int argc, char *argv[])
{
	//乱数初期化
	srand((unsigned)time(NULL));

	pic_data.init_data(FILENAME);

	//分割数が入る予定
	piece_x = PIECE_X;
	piece_y = PIECE_Y;

	//画像の大きさ
	int image_width = pic_data.get_width();
	int image_height = pic_data.get_height();

	//分割画像の大きさ
	piece_width = image_width/piece_x;
	piece_height = image_height/piece_y;

	//各ピースの配列宣言
	piece = new PIECE_DATA *[piece_x];
	for(int i=0; i<piece_x; i++){
		piece[i] = new PIECE_DATA[piece_y];
	}

	//各ピースのメモリの確保
	for(int i=0; i<piece_x; i++){
		for(int j=0; j<piece_y; j++){
			piece[i][j].init_data( piece_width, piece_height);
		}
	}

	//ピースごとに画像を取得
	for(int def_y=0; def_y<piece_y; def_y++){
		for(int def_x=0; def_x<piece_x; def_x++){

			for(int y=0; y<piece_height; y++){
				for(int x=0; x<piece_width; x++){
					for(int col=0; col<3; col++){
						piece[def_x][def_y].set_data( x, y, col, pic_data.get_data( x+(piece_width*def_x), y+(piece_height*def_y), col));
					}
				}
			}

		}
	}

	//比較結果保存 メモリ確保
	int **compare_r = new int *[piece_x];
	int **compare_l = new int *[piece_x];
	int **compare_u = new int *[piece_x];
	int **compare_d = new int *[piece_x];
	for(int i=0; i<piece_x; i++){
		compare_r[i] = new int[piece_y];
		compare_l[i] = new int[piece_y];
		compare_u[i] = new int[piece_y];
		compare_d[i] = new int[piece_y];
	}

	//自分の見ているピース。調べたい中心
	
	int max_rx, max_ry, max_r;
	int max_lx, max_ly, max_l;
	int max_ux, max_uy, max_u;
	int max_dx, max_dy, max_d;

	//自分の見るピース
	for(int pos_y=0; pos_y<piece_y; pos_y++){
		for(int pos_x=0; pos_x<piece_x; pos_x++){

			//初期化
			for(int i=0; i<piece_x; i++){
				for(int j=0; j<piece_y; j++){
					compare_r[i][j] = 0;
					compare_l[i][j] = 0;
					compare_u[i][j] = 0;
					compare_d[i][j] = 0;
				}
			}

			//それぞれのピースに対して
			for(int def_y=0; def_y<piece_y; def_y++){
				for(int def_x=0; def_x<piece_x; def_x++){

					for(int y=0; y<piece_height; y++){
						//画像の右側
						compare_r[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( piece_width-1, y, 0), piece[def_x][def_y].get_data( 0, y, 0));
						compare_r[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( piece_width-1, y, 1), piece[def_x][def_y].get_data( 0, y, 1));
						compare_r[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( piece_width-1, y, 2), piece[def_x][def_y].get_data( 0, y, 2));
                                                          
						//画像の左側                                  
						compare_l[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( 0, y, 0), piece[def_x][def_y].get_data( piece_width-1, y, 0));
						compare_l[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( 0, y, 1), piece[def_x][def_y].get_data( piece_width-1, y, 1));
						compare_l[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( 0, y, 2), piece[def_x][def_y].get_data( piece_width-1, y, 2));
					}
					for(int x=0; x<piece_width; x++){
						//画像の上側
						compare_u[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( x, 0, 0), piece[def_x][def_y].get_data( x, piece_height-1, 0));
						compare_u[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( x, 0, 1), piece[def_x][def_y].get_data( x, piece_height-1, 1));
						compare_u[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( x, 0, 2), piece[def_x][def_y].get_data( x, piece_height-1, 2));
						//画像の下側                                  
						//cout << "chk_7" << endl;
						compare_d[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( x, piece_height-1, 0), piece[def_x][def_y].get_data( x, 0, 0));
						compare_d[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( x, piece_height-1, 1), piece[def_x][def_y].get_data( x, 0, 1));
						compare_d[def_x][def_y] += compare_dot( piece[pos_x][pos_y].get_data( x, piece_height-1, 2), piece[def_x][def_y].get_data( x, 0, 2));
					}

				}
			}

			max_r  = compare_r[0][0];
			max_rx = max_ry = 0;
			max_l  = compare_l[0][0];
			max_lx = max_ly = 0;
			max_u  = compare_u[0][0];
			max_ux = max_uy = 0;
			max_d  = compare_d[0][0];
			max_dx = max_dy = 0;

			//それぞれのピースに対して
			for(int def_y=0; def_y<piece_y; def_y++){
				for (int def_x = 0; def_x < piece_x; def_x++) {
					if(compare_r[def_x][def_y] > max_r){
						max_r = compare_r[def_x][def_y];
						max_rx = def_x;
						max_ry = def_y;
					}
					if(compare_l[def_x][def_y] > max_l){
						max_l = compare_l[def_x][def_y];
						max_lx = def_x;
						max_ly = def_y;
					}
					if(compare_u[def_x][def_y] > max_u){
						max_u = compare_u[def_x][def_y];
						max_ux = def_x;
						max_uy = def_y;
					}
					if(compare_d[def_x][def_y] > max_d){
						max_d = compare_d[def_x][def_y];
						max_dx = def_x;
						max_dy = def_y;
					}
				}
			}

			piece[pos_x][pos_y].rx = max_rx;
			piece[pos_x][pos_y].ry = max_ry;
			piece[pos_x][pos_y].rp = (max_r)/(double)(piece_height*3*10)*100;
			//cout << max_r << ":aaa" << endl;

			piece[pos_x][pos_y].lx = max_lx;
			piece[pos_x][pos_y].ly = max_ly;
			piece[pos_x][pos_y].lp = (max_l)/(double)(piece_height*3*10)*100;

			piece[pos_x][pos_y].ux = max_ux;
			piece[pos_x][pos_y].uy = max_uy;
			piece[pos_x][pos_y].up = (max_u)/(double)(piece_width*3*10)*100;
			
			piece[pos_x][pos_y].dx = max_dx;
			piece[pos_x][pos_y].dy = max_dy;
			piece[pos_x][pos_y].dp = (max_d)/(double)(piece_width*3*10)*100;

		}
	}

	//結果表示
	for(int pos_y=0; pos_y<piece_y; pos_y++){
		for(int pos_x=0; pos_x<piece_x; pos_x++){
			cout << "[" << pos_x << ":" << pos_y << "]" << endl;
			cout << "R: (" << piece[pos_x][pos_y].rx << ":" << piece[pos_x][pos_y].ry << ")" << "[" << piece[pos_x][pos_y].rp << "]" << endl;
			cout << "L: (" << piece[pos_x][pos_y].lx << ":" << piece[pos_x][pos_y].ly << ")" << "[" << piece[pos_x][pos_y].lp << "]" << endl;
			cout << "U: (" << piece[pos_x][pos_y].ux << ":" << piece[pos_x][pos_y].uy << ")" << "[" << piece[pos_x][pos_y].up << "]" << endl;
			cout << "D: (" << piece[pos_x][pos_y].dx << ":" << piece[pos_x][pos_y].dy << ")" << "[" << piece[pos_x][pos_y].dp << "]" << endl;
		}
	}


	//ここから並び替え
	int datum_x, datum_y;
	
	int r1p, r2p;
	int r1x,r1y, r2x,r2y;

	do{
		//適当に基準となるピースを選択
		datum_x = rand()%piece_x;
		datum_y = rand()%piece_y;

		cout << "dx: " << datum_x << " dy: " << datum_y << endl;

		//自分の一つ右のピースの
		r1p = piece[datum_x][datum_y].rp;
		r1x = piece[piece[datum_x][datum_y].rx][piece[datum_x][datum_y].ry].dx;
		r1y = piece[piece[datum_x][datum_y].rx][piece[datum_x][datum_y].ry].dy;

		r2p = piece[datum_x][datum_y].dp;
		r2x = piece[piece[datum_x][datum_y].dx][piece[datum_x][datum_y].dy].rx;
		r2y = piece[piece[datum_x][datum_y].dx][piece[datum_x][datum_y].dy].ry;
	}while(r1x != r2x || r1y != r2y || r1p <= 50 || r2p <= 50);

	//まずは、左に行けるところまで
	do{
		cout << "dx: " << datum_x << " dy: " << datum_y << endl;

		r1p = piece[datum_x][datum_y].lp;
		r1x = piece[piece[datum_x][datum_y].lx][piece[datum_x][datum_y].ly].dx;
		r1y = piece[piece[datum_x][datum_y].lx][piece[datum_x][datum_y].ly].dy;

		r2p = piece[piece[datum_x][datum_y].dx][piece[datum_x][datum_y].dy].lp;
		r2x = piece[piece[datum_x][datum_y].dx][piece[datum_x][datum_y].dy].lx;
		r2y = piece[piece[datum_x][datum_y].dx][piece[datum_x][datum_y].dy].ly;

		if((r1x == r2x) && (r1y == r2y) && (r1p >= 50 || r2p >= 50 )){
			cout << "left" << endl;
			int tmp;
			tmp = datum_x;
			datum_x = piece[datum_x][datum_y].lx;
			datum_y = piece[tmp][datum_y].ly;
		}else{
			break;
		}
	}while(1);

	//次に、上に行ける所まで
	do{
		cout << "dx: " << datum_x << " dy: " << datum_y << endl;

		r1p = piece[datum_x][datum_y].up;
		r1x = piece[piece[datum_x][datum_y].ux][piece[datum_x][datum_y].uy].rx;
		r1y = piece[piece[datum_x][datum_y].ux][piece[datum_x][datum_y].uy].ry;

		r2p = piece[piece[datum_x][datum_y].rx][piece[datum_x][datum_y].ry].up;
		r2x = piece[piece[datum_x][datum_y].rx][piece[datum_x][datum_y].ry].ux;
		r2y = piece[piece[datum_x][datum_y].rx][piece[datum_x][datum_y].ry].uy;

		if((r1x == r2x) && (r1y == r2y) && (r1p >= 50 || r2p >= 50 )){
			cout << "up" << endl;
			int tmp;
			tmp = datum_x;
			datum_x = piece[datum_x][datum_y].ux;
			datum_y = piece[tmp][datum_y].uy;
		}else{
			break;
		}
	}while(1);	

	//左上に来たため、順番に右に進んでいく
	int xx, yy;
	xx = datum_x;
	yy = datum_y;

	for(int pos_y=0; pos_y<piece_y; pos_y++){
		for(int pos_x=0; pos_x<piece_x; pos_x++){
			cout << xx << "," << yy << "=" << pos_x << "," << pos_y << endl;
			piece[xx][yy].re_pos_x = pos_x;
			piece[xx][yy].re_pos_y = pos_y;
			
			int tmp;
			tmp = xx;
			xx = piece[xx][yy].rx;
			yy = piece[tmp][yy].ry;
		}
		xx = piece[datum_x][datum_y].dx;
		yy = piece[datum_x][datum_y].dy;
		datum_x = xx;
		datum_y = yy;
	}
	//glutInitWindowPosition(100, 100);
	
	 pic_data.imageGlutInitWindowSize();
	 glutInit(&argc, argv);
	 glutInitDisplayMode(GLUT_RGBA);
	 glutCreateWindow("create_image");
	 glutReshapeFunc(resize);
	 init();
	
	 glutDisplayFunc(display);

	 glutMainLoop();

	//メモリの解放呼び出されて無いかも。。。
	pic_data.delete_data();
	for(int i=0; i<piece_y; i++){
		for(int j=0; j<piece_x; j++){
			piece[j][i].delete_data();
		}
	}
	for(int i=0; i<piece_x; i++){
		delete[] piece[i];
	}
	delete[] piece;
	cout << "create delete PIECE memory" << endl;

	for(int i=0; i<piece_x; i++){
		delete[] compare_r[i];
		delete[] compare_l[i];
		delete[] compare_u[i];
		delete[] compare_d[i];
	}
	delete[] compare_r;
	delete[] compare_l;
	delete[] compare_u;
	delete[] compare_d;
	cout << "create delete compare memory" << endl;
}
