import java.util.*;
import java.io.*;

// evaluation: h*(x)  各セルの縦横の距離の差の合計
// floor: 初期状態からの距離？
// DataSetを継承すればいける
class field{
 // 実際のマス
 int cell[  ]=new int[ 9 ];
 // evaluation: 評価, 判定
 int evaluation;
 int route[  ];
 int floor;

 // hashが指定されていない時のコンストラクタ
 // 無名配列で下と一緒にできそう
 field(int cell[  ],int hash){
  route = new int[ 1 ];
  floor=1;
  // routeにhashをコピー
  route[ 0 ] = hash;

  // セルコピー
  for(int i=0;i<9;i++)this.cell[ i ]=cell[ i ];
  int eva=0;
  for(int i=0;i<9;i++){
   // 横方向と縦方向の本来の位置からの距離
   if(cell[ i ]!=0){
    //横方向の距離を加算
    //i:着目しているセル
    //cell[ i ]:着目している数字
    eva+=Math.abs( (cell[ i ]-1)%3-i%3);
    //縦方向の距離を加算
    eva+=Math.abs( (cell[ i ]-1)/3-i/3);
   }
  }
  // 
  evaluation = eva;
 }

 // hashの指定があるときにコンストラクタ
 field(int cell[  ],int route[  ],int hash){
  // floorは渡されたrouteの個数になる
  this.floor=route.length;
  // 本来のrouteより1つ多く(hashを代入するため)
  this.route = new int[ floor+1 ];
  // routeをコピー
  System.arraycopy(route,0,this.route,0,floor);
  // routeの最後にhashをコピー
  this.route[ floor ]=hash;
  for(int i=0;i<9;i++)this.cell[ i ]=cell[ i ];
  int eva=0;
  for(int i=0;i<9;i++){
   // セル番号が0の時に限り無視
   if(cell[ i ]!=0){
    //横方向の距離を加算
    //i:着目しているセル
    //cell[ i ]:着目している数字
    eva+=Math.abs( (cell[ i ]-1)%3-i%3);
    //縦方向の距離を加算
    eva+=Math.abs( (cell[ i ]-1)/3-i/3);
   }
  }
  evaluation = eva;
 }
 // cellの中身表示
 public String toString(){
  return new String("[ "+cell[ 0 ]+" ]"+"[ "+cell[ 1 ]+" ]"+"[ "+cell[ 2 ]+" ]\n"+"[ "+cell[ 3 ]+" ]"+"[ "+cell[ 4 ]+" ]"+"[ "+cell[ 5 ]+" ]\n"+"[ "+cell[ 6 ]+" ]"+"[ "+cell[ 7 ]+" ]"+"[ "+cell[ 8 ]+" ]");
 }
}


// 初期状態からの距離+h*(x)の値でソート済みのソートリスト(優先度付きキュー)
// h*(x)各数字パネルの元の位置からの距離の輪
// 距離: 縦横にそれぞれどれだけ離れているか
class ev_list{
 // fieldの要素
 field elem;
 // 次のev_list
 ev_list next;
 // null要素として
 ev_list(){
  next = null;
  this.elem = null;
 }
 // elemを含む要素
 ev_list(field elem){
  next = null;
  this.elem = elem;
 }
 // 挿入
 // 使えるのは多分先頭だけ
 public void insert(ev_list node){
  //ヒューリスティック関数の値が次のノードの値より大きい、
  //または次のノードが先端のとき挿入
  if(this.next==null || this.next.elem.evaluation+this.next.elem.floor<node.elem.evaluation+node.elem.floor){
   node.next = this.next;
   this.next = node;
  }
  // なければ次のリストに対して同じ処理
  else this.next.insert(node);
 }
 // 最後の要素を抜き出す
 public field pop(){
  if(this.next==null)return null;
  if(this.next.next==null){
   field temp=this.next.elem;
   this.next=null;
   return temp;
  }
  else return this.next.pop();
 }
}

class egt_pzl{

 // enumで簡略化出来そう
 static final int LEFT=0;
 static final int RIGHT=1;
 static final int UP=2;
 static final int DOWN=3;
 static final int factor[  ]={1,1,2,6,24,120,720,5040,40320};

 //ハッシュキー生成関数
 //cellをハッシュに変換する
 static public int hashkey(int cell[  ]){
  int val[  ] = new int[ 9 ];
  System.arraycopy(cell,0,val,0,9);
  for(int i=0;i<9;i++){
   for(int j=i+1;j<9;j++){
    if(val[ j ]>val[ i ])val[ j ]--;
   }
  }
  int hash=0;
  for(int i=0;i<9;i++){
   hash+=val[ i ]*factor[ 8-i ];
  }
  return hash;
 }

 // ハッシュからセルに復元する？
 static public field dehash(int key){
  int cell[  ] = new int[ 9 ];
  for(int i=0;i<9;i++){
   cell[ i ]=key/factor[ 8-i ];
   key%=factor[ 8-i ];
  }
  for(int i=8;i>=0;i--){
   for(int j=i+1;j<9;j++){
    if(cell[ i ]<=cell[ j ])cell[ j ]++;
   }
  }
  return new field(cell,key);
 }

 //3*3のフィールドの、移動できる範囲
 //move[ i ][ LEFT ]:左に動けるか(0,1)
 //move[ i ][ RIGHT ]:右に動けるか(0,1)
 //move[ i ][ UP ]:上に動けるか(0,1)
 //move[ i ][ DOWN ]:下に動けるか(0,1)
 static boolean move[  ][  ] = new boolean[ 9 ][ 4 ];

 public static void main(String args[  ]){

  ////////移動可能方向の設定部分////////

  //移動できる方向を各セルに対して設定する。
  // n*mに対応できるように
  // 本来の位置の状態での判定だと思われる

  //右に動けるかどうかを指定。
  move[ 0 ][ RIGHT ]=move[ 1 ][ RIGHT ]=move[ 3 ][ RIGHT ]=move[ 4 ][ RIGHT ]=move[ 6 ][ RIGHT ]=move[ 7 ][ RIGHT ]=true;
  move[ 2 ][ RIGHT ]=move[ 5 ][ RIGHT ]=move[ 8 ][ RIGHT ]=false;

  //左に動けるかどうかを指定。
  move[ 1 ][ LEFT ]=move[ 2 ][ LEFT ]=move[ 4 ][ LEFT ]=move[ 5 ][ LEFT ]=move[ 7 ][ LEFT ]=move[ 8 ][ LEFT ]=true;
  move[ 0 ][ LEFT ]=move[ 3 ][ LEFT ]=move[ 6 ][ LEFT ]=false;

  //上に動けるかどうかを指定。
  move[ 3 ][ UP ]=move[ 4 ][ UP ]=move[ 5 ][ UP ]=move[ 6 ][ UP ]=move[ 7 ][ UP ]=move[ 8 ][ UP ]=true;
  move[ 0 ][ UP ]=move[ 1 ][ UP ]=move[ 2 ][ UP ]=false;

  //下に動けるかどうかを指定。
  move[ 0 ][ DOWN ]=move[ 1 ][ DOWN ]=move[ 2 ][ DOWN ]=move[ 3 ][ DOWN ]=move[ 4 ][ DOWN ]=move[ 5 ][ DOWN ]=true;
  move[ 6 ][ DOWN ]=move[ 7 ][ DOWN ]=move[ 8 ][ DOWN ]=false;

  ////////初期配置の設定部分////////

  //初期位置
  field st;
  int cell[  ]= new int[ 9 ];

  //各セルの初期値を設定。
  //めんどいのでハードコーディング。
  //「0」は空セルの意味。

  cell[ 0 ]=1;
  cell[ 1 ]=8;
  cell[ 2 ]=0;
  cell[ 3 ]=4;
  cell[ 4 ]=3;
  cell[ 5 ]=2;
  cell[ 6 ]=5;
  cell[ 7 ]=7;
  cell[ 8 ]=6;

  st = new field(cell,hashkey(cell) );
  ////ハッシュテーブル////
  boolean visited[  ] = new boolean[ 362880 ];
  for(int i=0;i<362880;i++)visited[ i ]=false;

  ////////探索部分////////

  //ソートリストを設定
  ev_list list = new ev_list();
  list.insert(new ev_list(st) );
  visited[ hashkey(cell) ]=true;

  field cur;
  int hash;
  int count=0;
  while( (cur=list.pop() )!=null){
   count++;

   for(int i=0;i<9;i++){
    // セルが0でなければ
    if(cur.cell[ i ]==0){
     // 左が動かせるなら
     if(move[ i ][ LEFT ]){
      cur.cell[ i ]=cur.cell[ i-1 ];
      cur.cell[ i-1 ]=0;
      hash=hashkey(cur.cell);
      if(!visited[ hash ]){
       visited[ hash ]=true;
       list.insert(new ev_list(new field(cur.cell,cur.route,hash) ));
      }
      cur.cell[ i-1 ]=cur.cell[ i ];
      cur.cell[ i ]=0;
     }
     if(move[ i ][ RIGHT ]){
      cur.cell[ i ]=cur.cell[ i+1 ];
      cur.cell[ i+1 ]=0;
      hash=hashkey(cur.cell);
      if(!visited[ hash ]){
       visited[ hash ]=true;
       list.insert(new ev_list(new field(cur.cell,cur.route,hash) ));
      }
      cur.cell[ i+1 ]=cur.cell[ i ];
      cur.cell[ i ]=0;
     }
     if(move[ i ][ UP ]){
      cur.cell[ i ]=cur.cell[ i-3 ];
      cur.cell[ i-3 ]=0;
      hash=hashkey(cur.cell);
      if(!visited[ hash ]){
       visited[ hash ]=true;
       list.insert(new ev_list(new field(cur.cell,cur.route,hash) ));
      }
      cur.cell[ i-3 ]=cur.cell[ i ];
      cur.cell[ i ]=0;
     }
     if(move[ i ][ DOWN ]){
      cur.cell[ i ]=cur.cell[ i+3 ];
      cur.cell[ i+3 ]=0;
      hash=hashkey(cur.cell);
      if(!visited[ hash ]){
       visited[ hash ]=true;
       list.insert(new ev_list(new field(cur.cell,cur.route,hash) ));
      }
      cur.cell[ i+3 ]=cur.cell[ i ];
      cur.cell[ i ]=0;
     }
    }
   }
   if(cur.evaluation==0){
    System.out.println(count+"");
    System.out.println("evaluation[ "+0+" ]="+dehash(cur.route[ 0 ]).evaluation+"\n"+dehash(cur.route[ 0 ]) );
    for(int i=1;i<cur.route.length;i++){
     System.out.println("↓");
     System.out.println("evaluation[ "+i+" ]="+dehash(cur.route[ i ]).evaluation);
     System.out.println("evaluation+floor[ "+i+" ]="+(dehash(cur.route[ i ]).evaluation+i) );
     System.out.println( dehash(cur.route[ i ]).toString() );
    }
    break;
   }
  }


 }
}
