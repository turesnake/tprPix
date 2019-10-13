//-------- photoshop_javascript ---------//
//  before everything:
//  put all target-frames in single dir, named as val: dirName
//  set all frame files name as: "mid (1).png"
//  ---
//  then, run this script



//--------------------------------------------//
//              input vals
//--------------------------------------------//
var totalFrameNums = prompt( "How many Frames at all?(1~99)", 4, "How Many Frames" );
var frameNums_x = prompt( "How many Frames in X_Axle?(1~99)", 2, "How Many Frames" );
var frameNums_y = prompt( "How many Frames in Y_Axle?(1~99)", 2, "How Many Frames" );

if( (totalFrameNums<=0) && (totalFrameNums>99) ){
    alert( "InputVal: totalFrameNums: out of range;" );
}
if( (frameNums_x<=0) && (frameNums_x>99) ){
    alert( "InputVal: frameNums_x: out of range;" );
}
if( (frameNums_y<=0) && (frameNums_y>99) ){
    alert( "InputVal: frameNums_y: out of range;" );
}

if( frameNums_x * frameNums_y > totalFrameNums ){
    alert( "InputVal: frameNums_x * frameNums_y > totalFrameNums;" );
}


//--------------------------------------------//
//                 params
//--------------------------------------------//
var path_rootDir_win = "C:/Users/Administrator/Desktop/OUT/";
var dirName = "Mid_2"; // 要读取的数据的 dir
var fileName_prefix = "m";


//--------------------------------------------//
//                  prepare
//--------------------------------------------//
app.preferences.rulerUnits = Units.PIXELS; //- based on pixel
app.preferences.typeUnits  = TypeUnits.PIXELS;


//--------------------------------------------//
//                file names
//--------------------------------------------//
// 正式读取前，应当使用 windows 默认统一命名工具，将目录内所有文件，统一命名为 "xxx"
// 此时，单个文件名 就会自动变为 "xxx (1).png" (中间有空格)
var fileNames = new Array(); // "mid (1).png"...
var frameNames = new Array(); // "mid_1"...

for( var i=0; i<totalFrameNums; i++ ){
    var fileIdx = i + 1;
    var frameName = fileName_prefix + "_" + fileIdx.toString();
    var fileName = fileName_prefix + " (" + fileIdx.toString() + ")" + ".png";
    //---
    frameNames[i] = frameName;
    fileNames[i] = fileName;
}

//--------------------------------------------//
//                dir paths
//--------------------------------------------//
var path_targetDir = path_rootDir_win + dirName + "/";


//--------------------------------------------//
//              root doc
//--------------------------------------------//
var rootDoc = app.documents.add( 1020, 1020 ); //- 和 frameFile 同尺寸
//--- 隐藏 背景图层 ---
rootDoc.activeLayer.isBackgroundLayer = false;
rootDoc.activeLayer.kind = LayerKind.NORMAL;
rootDoc.activeLayer.visible = false;

//------
var rootLayerSet = rootDoc.layerSets.add();
rootLayerSet.name = "root";

var rootTopLayer = rootDoc.artLayers.add();
rootTopLayer.name = "__rootTop__";
rootTopLayer.move(rootLayerSet, ElementPlacement.PLACEATBEGINNING); //- at top



//--------------------------------------------//
//     for each frameFile
//--------------------------------------------//
for( var i=0; i<totalFrameNums; i++  ){//- each frameFile

    var filePath = path_targetDir + fileNames[i];
    var frameDoc = open(File(filePath));
    app.activeDocument = frameDoc;
    handle_frameDoc( frameDoc, frameNames[i], rootLayerSet );
}



//--------------------------------------------//
//        frames size / pos
//--------------------------------------------//
var rootBounds0 = rootLayerSet.bounds[0].value;
var rootBounds1 = rootLayerSet.bounds[1].value;
var rootBounds2 = rootLayerSet.bounds[2].value;
var rootBounds3 = rootLayerSet.bounds[3].value;

var frameSZ_x = rootBounds2 - rootBounds0;
var frameSZ_y = rootBounds3 - rootBounds1;


//--------------------------------------------//
//     
//--------------------------------------------//
create_kuangLayer( frameSZ_x, frameSZ_y );
resize_canvas();



//--------------------------------------------//
//     
//--------------------------------------------//
var posX; //- 每一帧，target pos
var posY;
var offX; //- offset
var offY;
var frameIdx;
for( var h=0; h< frameNums_y; h++ ){
    posY = h * frameSZ_y;

    for( var w=0; w< frameNums_x; w++ ){
        posX = w * frameSZ_x;

        frameIdx = h * frameNums_x + w;
        if( frameIdx >= totalFrameNums ){
            break;
        }

        var targetLayer = rootLayerSet.artLayers.getByName( frameNames[frameIdx] ); //- 目标layer
        offX = posX - rootBounds0;
        offY = posY - rootBounds1;
        targetLayer.translate( offX, offY );
    }
}


// 将 frameFile 中的数据，复制到 rootDoc，命名好，再放进规定的文件夹
// 然后通过预先制作好的 action，删除底色区域
function handle_frameDoc( frameDoc_, frameName_, layerSet_ ){
    //---
    app.activeDocument = frameDoc_;
    frameDoc_.activeLayer.copy();
    //---
    app.activeDocument = rootDoc;
    var pasteLayer = rootDoc.paste();
    pasteLayer.name = frameName_;
    pasteLayer.move(layerSet_, ElementPlacement.PLACEATBEGINNING);
    //---
    app.doAction("delete_backColor","tapir.ATN") // make sure exist ！
    //---
    frameDoc_.close();
}


//-- kuang 表示 1-frame 的实际尺寸，在未来，辅助 Jpng 的制作
//   一出生就被放置在 左上角 
function create_kuangLayer( sizeX_, sizeY_ ){

    var kuangLayer = rootDoc.artLayers.add();
    kuangLayer.name = "kuang";
    kuangLayer.move(rootLayerSet, ElementPlacement.PLACEBEFORE);
    rootDoc.activeLayer = kuangLayer;

    var shapeRef = [
        [0,      0],
        [0,      sizeY_],
        [sizeX_, sizeY_],
        [sizeX_, 0]
    ]

    var sColor = new SolidColor();
    sColor.rgb.red = 30;
    sColor.rgb.green = 30;
    sColor.rgb.blue = 30;

    rootDoc.selection.select(shapeRef);
    rootDoc.selection.fill( sColor ); //- 填充实心颜色
    rootDoc.activeLayer.opacity = 50; //- 图层半透明

    rootDoc.selection.deselect();
}



function resize_canvas(){

    rootDoc.resizeCanvas( 
        frameSZ_x * frameNums_x,
        frameSZ_y * frameNums_y,
        AnchorPosition.TOPLEFT );
}






