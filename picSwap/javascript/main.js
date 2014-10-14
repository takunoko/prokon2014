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
  swap(width,height);
 });
});

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

function swap(width,height){
 var target=$("#template")[0];
 var targetContext=target.getContext('2d');

 var origCanvas=$("#display")[0];
 var origCtx=origCanvas.getContext('2d');

 target.width=origCanvas.width;
 target.height=origCanvas.height;

 var len=width*height;
 var arr=new Array(len);
 var partWidth=Math.floor(origCanvas.width/width);
 var partHeight=Math.floor(origCanvas.height/height);

 for(var i=0;i<len;i++){
  arr[i]=i;
 }

 for(var i=0;i<len;i++){
  var j=Math.floor(Math.random()*((width*height-1)-i))+i;
  console.log(j);
  var tmp=arr[j];
  arr[j]=arr[i];
  arr[i]=tmp;
 }

 for(i=0;i<len;i++){
  var x=(i%width)*partWidth;
  var y=Math.floor(i/width)*partHeight;
  // get And Put and Output;
  var part=origCtx.getImageData(x,y,partWidth,partHeight);
  targetContext.putImageData(part,(arr[i]%width)*partWidth,Math.floor(arr[i]/width)*partHeight);
 }

 $("#converted").attr("src",target.toDataURL());
}
