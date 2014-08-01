/* colとrowの位置修正版 
   for文のiは列(y座標)を表す。よって配列の左側の添え字は列(y座標)を表す。
   colは行の番号を表す。
   rowは列の番号を表す。
            col
          0 1 2 3
    row 0 
    row 1
    row 2
    row 3
  関数の定義でcolが先に定義されるのは、x座標は左側に書くから。
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXCOL 16 // 0x10
#define MAXROW 16
#define COL 3
#define ROW 3
#define PRI_CORNER 10
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3
#define MOVNUM 3

class cell {
public:
  int **data;
  int col;
  int row;

  void init(int col, int row);
  void disp(int col = -1, int row = -1);
  int find_data(int num);
};

class dist {
public:
  int **dcol;
  int **drow;
  int **dpri;
  int col;
  int row;

  void init(cell myc);
  void disp_dist();
  void disp_pri();
  void find_dist(cell myc);
  void find_pri(cell myc);
  void setpri0();
  int find_big();
};

class data {
public:
  int col;
  int row;
  int num;
};

class selcha {
public:
  int sel;
  int cha;

  void init();
};

class route {
public:
  int col;
  int row;
  int pri;
  route *next;
};

cell make_data(cell myc);
cell make_dist(cell myc);
cell sort_data1(cell myc, int *change_num);
cell sort_data2(cell myc);
cell sort_force(cell myc, int col, int row);
void change_data(int *num1, int *num2);
int sign(int num);
int surround_pri(dist myd, data pos, int dir);
int sort_loop(cell *myc, dist *myd, int x, int y, int old, int *kaisuu);
int find_mysur(dist myd, int x, int y, int *old);
int convert_num(int col, int row);
int convert_col(int num);
int convert_row(int num);
int find_max(int array[], int num);

int main(void) {
  cell myc;
  int change_num;

  srand(unsigned(time(NULL)));

  myc.init(COL, ROW);
  myc = make_data(myc);
  
  
  myc.data[0][0] = 0x02; myc.data[0][1] = 0x11; myc.data[0][2] = 0x10;
  myc.data[1][0] = 0x20; myc.data[1][1] = 0x00; myc.data[1][2] = 0x01;
  myc.data[2][0] = 0x12; myc.data[2][1] = 0x22; myc.data[2][2] = 0x21;
  
  /*
  myc.data[0][0] = 0x31; myc.data[0][1] = 0x11; myc.data[0][2] = 0x30; myc.data[0][3] = 0x21;
  myc.data[1][0] = 0x10; myc.data[1][1] = 0x01; myc.data[1][2] = 0x00; myc.data[1][3] = 0x20;
  */
  myc.disp();
  //sort_data1(myc, &change_num);
  //sort_data2(myc);
  /*myc = sort_data1(myc, &change_num);
  myc.disp();
  printf("交換回数 = %d\n", change_num);
  */
  return 0;
}

// 乱数のデータ作成
cell make_data(cell myc) {
  int i, j, r, c;

  for(i = 0; i < myc.row; i++) {
    for(j = 0; j < myc.col; j++) {
      myc.data[i][j] = j * 0x10 + i;
    }
  }

  myc.disp();

  for(i = 0; i < myc.row; i++) {
    for(j = 0; j < myc.col; j++) {
      r = rand() % myc.row;
      c = rand() % myc.col;
      change_data(&myc.data[i][j], &myc.data[r][c]);
    }
  }

  return myc;
}

// ソートのテスト
cell sort_data1(cell myc, int *change_num) {
  int i, j;
  int k, orig, orig_col, orig_row;
  int dest, dest_col, dest_row;
  int selled;
  int select = 0;

  *change_num = 0;

  for(i = 0; i < myc.row; i++) {
    for(j = 0; j < myc.col; j++) {
      dest = convert_num(j, i);
      orig = myc.find_data(dest);
      orig_col = convert_col(orig); // 元のx座標
      orig_row = convert_row(orig); // 元のy座標
      printf("[%x, %x]\n", orig_col, orig_row);
      myc.disp(orig_col, orig_row);
      selled = 0;
      for(k = orig_col;;) {
        if(k == j) {
          break;
        } else if(k > j) {
          selled = 1;
          change_data(&myc.data[orig_row][k], &myc.data[orig_row][k-1]);
          k--;
        } else if(k < j) {
          selled = 1;
          change_data(&myc.data[orig_row][k], &myc.data[orig_row][k+1]);
          k++;
        }
        (*change_num)++; // 交換回数
      }
      for(k = orig_row;;) {
        if(k == i) {
          break;
        }else if(k > i) {
          selled = 1;
          change_data(&myc.data[k][j], &myc.data[k-1][j]);
          k--;
        }else if(k < i) {
          selled = 1;
          change_data(&myc.data[k][j], &myc.data[k+1][j]);
          k++;
        }
        (*change_num)++; // 交換回数
      }
              if(selled == 1) select++;
    }
  }

  printf("select = %d\n", select);

  return myc;
}

cell sort_data2(cell myc) {
  dist myd; // 優先度系
  data pos; // 場所を示す
  selcha scnum; // 選択、交換回数
  int i, j;

  // 優先度系クラスを初期化
  myd.init(myc);
  myd.find_dist(myc);
  myd.find_pri(myc);
  myd.disp_dist();
  myd.disp_pri();

  // 選択する場所
  pos.num = myd.find_big();
  pos.col = convert_col(pos.num);
  pos.row = convert_row(pos.num);

  // メインループ
  while(pos.num >= 0) {
    scnum.sel++;

  }
}

route *select_route(dist myd, route root, data pos) {

}

int check_pri(dist myd, data pos, int dir) {
  int mydir; // 今調べる方向
  int pri[4];

  // 4方向の中で一番優先度が高いのを選ぶ
  for(mydir = 0; mydir < 4; mydir++) {
    // 戻る方向なら選択されない
    if(abs(mydir - dir) == 2) {
      pri[mydir] = -1;
    }else {
      surround_pri(myd, pos, mydir);
    }
  }

  return find_max(pri, 4);

}

int surround_pri(dist myd, data pos, int dir) {
  // iはy, jはx
  int i, j;
  int ip, jp; // iとjに足す数
  int pri = 0;

  // 方向によってiとjに足す数を決める
  switch(dir) {
    case UP:
      ip = -1;
      jp = 0;
    break;
    case RIGHT:
      ip = 0;
      jp = 1;
    break;
    case DOWN:
      ip = 1;
      jp = 0;
    break;
    case LEFT:
      ip = 0;
      jp = -1;
    break;
  }

  // 配列の範囲をはみ出していたら
  if(pos.row + ip < 0 || pos.row + ip >= myd.row || pos.col + jp < 0 || pos.col + jp >= myd.col) {
    return -1; // 選択させない
  }

  switch(dir) {
    case LEFT || RIGHT:
    if(sign(myd.dcol[pos.row + ip][pos.col + jp]) == sign(myd.dcol[pos.row][pos.col])) {

    }
    break;
  }

  return myd.dpri[pos.row + ip][pos.col + jp];
}

// numの符号を返す
int sign(int num) {
  return num/abs(num);
}

// 配列の中で最大の値を持つ要素の要素番号を返す
int find_max(int array[], int num) {
  int i, max;

  max = 0;
  for(i = 1; i < num; i++) {
    if(array[max] < array[i]) max = i;
  }

  return max;
}

cell sort_force(cell myc, int col, int row) {
  int i, j;
  int k, orig, orig_col, orig_row;
  int dest, dest_col, dest_row;
  int selled;
  int select = 0;

  dest = myc.data[row][col];
  dest_col = convert_col(dest);
  dest_row = convert_row(dest);
  orig = convert_num(col, row);
  orig_col = col; // 元のx座標
  orig_row = row; // 元のy座標
  printf("[%x, %x]\n", orig_col, orig_row);
  myc.disp(orig_col, orig_row);
  selled = 0;
  for(k = orig_col;;) {
    if(k == dest_col) {
      break;
    } else if(k > dest_col) {
      selled = 1;
      change_data(&myc.data[orig_row][k], &myc.data[orig_row][k-1]);
      k--;
    } else if(k < dest_col) {
      selled = 1;
      change_data(&myc.data[orig_row][k], &myc.data[orig_row][k+1]);
      k++;
    }
  }
  for(k = orig_row;;) {
    if(k == dest_row) {
      break;
    }else if(k > dest_row) {
      selled = 1;
      change_data(&myc.data[k][j], &myc.data[k-1][j]);
      k--;
    }else if(k < dest_row) {
      selled = 1;
      change_data(&myc.data[k][j], &myc.data[k+1][j]);
      k++;
    }
  }

  return myc;

}

// データの入れ替え
void change_data(int *num1, int *num2) {
  int num = *num1;
  *num1 = *num2;
  *num2 = num;
}

int convert_num(int col, int row) {
  return row + col * 0x10;
}

// 数字を列の数字に変換
int convert_col(int num) {
  return (num / 0x10);
}

// 数字を行の数字に変換
int convert_row(int num) {
  return (num % 0x10);
}

// ------------------------------------- class cell ---------------------------------------

void cell::init(int col, int row) {
  int i, j;
  
  this->col = col;
  this->row = row;
  this->data = new int*[this->row]; // 配列作成

  for(i = 0; i < this->row && i < MAXROW; i++) {
    this->data[i] = new int[this->col]; // 配列作成
    for(j = 0; j < this->col && j < MAXCOL; j++) {
      this->data[i][j] = 0;
    }
  }
}

void cell::disp(int col, int row) {
  int i, j;

  for(i = 0; i < this->row; i++) {
    for(j = 0; j < this->col; j++) {
      if(col == j && row == i) {
        if(this->data[i][j] < 0x10) printf("[0%-X]", this->data[i][j]);
        else printf("[%-2X]", this->data[i][j]);
      }else {
        if(this->data[i][j] < 0x10) printf(" 0%-2X", this->data[i][j]);
        else printf(" %-3X", this->data[i][j]);
      }
    }
    puts("");
  }
  puts("");
}

int cell::find_data(int num) {
  int i, j;

  for(i = 0; i < row; i++) {
    for(j = 0; j < col; j++) {
      if(data[i][j] == num) return convert_num(j, i);
    }
  }
}

// ------------------------------------- class dist ---------------------------------------

void dist::init(cell myc) {
  int i, j;

  this->col = myc.col;
  this->row = myc.row;

  dcol = new int*[this->row]; // 配列作成
  drow = new int*[this->row]; // 配列作成
  dpri = new int*[this->row]; // 配列作成

  for(i = 0; i < this->row && i < MAXROW; i++) {
    dcol[i] = new int[this->col]; // 配列作成
    drow[i] = new int[this->col]; // 配列作成
    dpri[i] = new int[this->col]; // 配列作成
    for(j = 0; j < this->col && j < MAXCOL; j++) {
      dpri[i][j] = 0;
    }
  }
}

void dist::find_dist(cell myc) {
  int i, j;

  for(i = 0; i < this->row && i < MAXROW; i++) {
    for(j = 0; j < this->col && j < MAXCOL; j++) {
      dcol[i][j] = convert_col(myc.data[i][j]) - j;
      drow[i][j] = convert_row(myc.data[i][j]) - i;
    }
  }
}

void dist::disp_dist() {
  int i, j;

  for(i = 0; i < row; i++) {
    for(j = 0; j < col; j++) {
      printf("[%2d,%2d]", dcol[i][j], drow[i][j]);
    }
    puts("");
  }
  puts("");
}

void dist::disp_pri() {
  int i, j;

  for(i = 0; i < row; i++) {
    for(j = 0; j < col; j++) {
      printf("%3d", dpri[i][j]);
    }
    puts("");
  }
  puts("");
}

int dist::find_big() {
  int i, j, big_p, big;

  big = dpri[0][0];
  big_p = 0;

  for(i = 0; i < row; i++) {
    for(j = 0; j < col; j++) {
      if(big < dpri[i][j]) {
        big = dpri[i][j];
        big_p = convert_num(j, i);
      }
    }
  }

  if(big == 0) big_p = -1;

  return big_p;
}

void dist::find_pri(cell myc) {
  int i, j, du;
  int dum;

  setpri0();

  // 角に行くブロックは優先度PRI_CORNER+
  du = myc.find_data(0x00);
  if(du != 0x00) {
    dpri[convert_row(du)][convert_col(du)] += (myc.row * myc.col);
  }
  du = myc.find_data(convert_num(0, row-1));
  if(du != convert_num(0, row-1)) {
    dpri[convert_row(du)][convert_col(du)] += (myc.row * myc.col);
  }
  du = myc.find_data(convert_num(col-1, 0));
  if(du != convert_num(col-1, 0)) {
    dpri[convert_row(du)][convert_col(du)] += (myc.row * myc.col);
  }
  du = myc.find_data(convert_num(col-1, row-1));
  if(du != convert_num(col-1, row-1)) {
    dpri[convert_row(du)][convert_col(du)] += (myc.row * myc.col);
  }

  for(i = 0; i < this->row; i++) {
    for(j = 0; j < this->col; j++) {
      // 遠いほど優先度+
      dpri[i][j] += abs(drow[i][j]) + abs(dcol[i][j]);
      dum = convert_col(myc.data[i][j]);
      //if(dum == 0 || dum == myc.col) dpri[i][j] += 100;
    }
  }
}

void dist::setpri0() {
  int i, j;

  for(i = 0; i < row; i++) {
    for(j = 0; j < col; j++) {
      dpri[i][j] = 0;
    }
  }
}

void selcha::init() {
  sel = 0;
  cha = 0;
}
