//-------- photoshop_javascript ---------//



//--------------------------------------------//
//              input vals
//--------------------------------------------//
var cFrameNum = prompt( "How many Frames in one animAction?(1~99)", 6, "How Many Frames" );
if( (cFrameNum<=0) && (cFrameNum>99) ){
    alert( "InputVal: out of range;" );
}



//--------------------------------------------//
//                  prepare
//--------------------------------------------//
app.preferences.rulerUnits = Units.PIXELS; //- 设为像素制
app.preferences.typeUnits  = TypeUnits.PIXELS;

//var doc = app.activeDocument;
//var layer= doc.activeLayer;


//--------------------------------------------//
//                file names
//--------------------------------------------//
var fileNames = new Array(); // "0001.png"...
var frameNames = new Array(); // "0001"...
for( var i=0; i<cFrameNum; i++ ){
    var fileIdx = i + 1;
    if( fileIdx < 10 ){
        frameNames[i] = "000" + fileIdx.toString();
        fileNames[i] = frameNames[i] + ".png";
    }else if( fileIdx < 100 ){
        frameNames[i] = "00" + fileIdx.toString();
        fileNames[i] = frameNames[i] + ".png";
    }else{
        alert("Too Mush Frames per animAction;");// not support yet
    }
}

//--------------------------------------------//
//                dir paths
//--------------------------------------------//
var cpath_out = "C:/Users/Administrator/Desktop/OUT/"; // png文件总目录
var cpath_subDirs = [ "1_Left", "2_LeftTop", "3_Top", "4_RightTop", "5_Right", "6_RightBottom", "7_Bottom", "8_LeftBottom" ];

//--- 8 dir path ---//
var path_dirs = new Array();
for( var i=0; i<cpath_subDirs.length; i++ ){//- 8 directorys
    path_dirs[i] = cpath_out + cpath_subDirs[i] + "/";
}


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

for( var i=0; i<cpath_subDirs.length; i++ ){//- 8 directorys
    var layerSet = rootDoc.layerSets.add();
    layerSet.name = cpath_subDirs[i];
    layerSet.move(rootTopLayer, ElementPlacement.PLACEAFTER);
    //...
}

//--------------------------------------------//
//     for each frameFile in 8 directorys
//--------------------------------------------//
for( var j=0; j<path_dirs.length; j++ ){//- 8 directorys
//for( var j=0; j<1; j++ ){//- test .....

    var layerSet = rootLayerSet.layerSets.getByName( cpath_subDirs[j] );

    for( var i=0; i<cFrameNum; i++  ){//- each frameFile in 1 dir 

        var filePath = path_dirs[j] + fileNames[i];
        var frameDoc = open(File(filePath));
        app.activeDocument = frameDoc;
        handle_frameDoc( frameDoc, frameNames[i], layerSet );
    }
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
for( var h=0; h<path_dirs.length; h++ ){//- 1-directory，1-line
    
    posY = h * frameSZ_y;
    var targetLayerSet = rootLayerSet.layerSets.getByName( cpath_subDirs[h] );

    for( var w=0; w<cFrameNum; w++ ){//- each frame in animAction
        posX = w * frameSZ_x;
        var targetLayer = targetLayerSet.artLayers.getByName( frameNames[w] ); //- 目标layer
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
        frameSZ_x * cFrameNum,
        frameSZ_y * path_dirs.length,
        AnchorPosition.TOPLEFT );
}






