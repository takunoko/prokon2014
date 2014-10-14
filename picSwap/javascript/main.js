function cancelEvent (e){
 e.stopPropagation();
 e.preventDefault();
 return false;
}

function handleDroppedFile(e){
 //var file=e.originEvent.dataTransfer.files;
 cancelEvent(e);
 readPicture(e.originalEvent.dataTransfer.files[0]);
 return false;
}

$(function(){
 var doc=$(document);
 if(!window.File){
  alert("Your browser doesn't support File API");
 }

 // dropファイル
 /*
 doc.bind("dragenter",cancelEvent);
 */
 doc.bind("dragover",cancelEvent);

 doc.bind("drop",handleDroppedFile);
 $("#execute").on("click",function(e){
  var width=$("#widthSprit").val();
  var height=$("#heightSprit").val();
  swap(parseInt(width),parseInt(height));
 });
});

// 画像読み込み
function readPicture(file){
 var reader;

 $("#filename").text(file.name);

 reader=new FileReader();
 reader.onerror=function(e){
  alert("reader something wrong");
 }
 reader.onload=function(e){
  var loadedPicture=new Image();
  loadedPicture.src=e.target.result;
  loadedPicture.onload=function(){
   var canvas=$("#display");
   var picWidth=loadedPicture.width;
   var picHeight=loadedPicture.height;
   $("#comments").text("width:"+picWidth+",height:"+picHeight);
   canvas.attr("width",picWidth);
   canvas.attr("height",picHeight);
   var canvasCtx=canvas[0].getContext('2d');
   canvasCtx.drawImage(loadedPicture,0,0,picWidth,picHeight,0,0,picWidth,picHeight);

   /* †交換† */
   //swap(16,16);
  }
 }
 reader.readAsDataURL(file);
}

// width,height: 分割数
function swap(width,height){
 // 対象画像
 var target=$("#template")[0];
 var targetContext=target.getContext('2d');

 // 元画像
 var origCanvas=$("#display")[0];
 var origCtx=origCanvas.getContext('2d');

 // 解答用配列
 var answer=new Array(height);
 console.log(answer);
 for(var i=0;i<answer.length;i++){
  answer[i]=new Array(width);
 }
 console.log(answer);

 target.width=origCanvas.width;
 target.height=origCanvas.height;

 var len=width*height;
 // 乱数用配列
 var arr=new Array(len);
 // パーツの大きさ
 var partWidth=Math.floor(origCanvas.width/width);
 var partHeight=Math.floor(origCanvas.height/height);

 for(var i=0;i<len;i++){
  arr[i]=i;
 }
 // ランダムに並び替え
 for(var i=0;i<len;i++){
  // iから最後までの中から1つ選択する
  var j=Math.floor( Math.random()*((len-1)-i) )+i;
  // 交換する
  var tmp=arr[j];
  arr[j]=arr[i];
  arr[i]=tmp;
 }

 console.log(answer);

 // 画像
 for(i=0;i<len;i++){
  console.log("x:" + (i%width) + "\ny:" + Math.floor(i/width));
  console.log("x:" + (arr[i]%width) + "\ny:" + Math.floor(arr[i]/width));
  // get And Put and Output;
  // arr[x][y]を抜き出して
  var part=origCtx.getImageData((arr[i]%width)*partWidth,Math.floor(arr[i]/width)*partHeight,partWidth,partHeight);
  // [i%width][i/width]に
  targetContext.putImageData(part,(i%width)*partWidth,Math.floor(i/width)*partHeight);
  // 正解指定 (answer:y,x);
  answer[Math.floor(i/width)][i%width]=[arr[i]%width,Math.floor(arr[i]/width)];
 }

 $("#converted").attr("src",target.toDataURL());
 $("#answer").text(JSON.stringify(answer,null," "));
}
