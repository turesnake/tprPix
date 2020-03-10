/*
 * =================== load_and_divide_png.cpp =================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    加载 解析 “图元帧式的png文件” 
 * ----------------------------
 */
#include "pch.h"
#include "stb_image_no_warnings.h"

//------------------- CPP --------------------//
#include <iterator>

//------------------- Engine --------------------//
#include "load_and_divide_png.h"


/* ===========================================================
 *                load_and_divide_png
 * -----------------------------------------------------------
 * -- param: _path   -- png文件的 绝对path
 * -- param: _frameNums -- xy轴 图元帧 个数
 * -- param: _totalFrameNums -- 总帧数 
 * -- param: frame_data_ary_ -- 将每一帧的图形数据，存入这组 帧容器中
 * -- return:
 *          pixNum_per_frame
 */
IntVec2 load_and_divide_png( const std::string &path_,
                            IntVec2     frameNum_,
                            size_t      totalFrameNum_,
        std::vector< std::vector<RGBA>> &frame_data_ary_ ){

    //------------------------------//
    //   加载 png图片，获得其 原始数据
    //------------------------------//
    int width  {};
    int height {};
    int nrChannels {};
    unsigned char *data {nullptr}; //-- 临时，图片数据的指针。
                        //- 真实数据存储在 stbi 模块自己创建的 内存中。
                        //- 我们只获得一个 调用指针。

    stbi_set_flip_vertically_on_load( 1 ); //-- 防止 图片 y轴颠倒。
    data = stbi_load( path_.c_str(), &width, &height, &nrChannels, 0 );
        if(  data == nullptr ){
            tprDebug::console( "path:{}", path_ );
            tprAssert( data != nullptr );
        }

    //------------------------------------//
    //   获得 每一帧的数据, 存入各自 帧容器中
    //------------------------------------// 
    //-- 事先准备好 每一帧的容器 --
    frame_data_ary_.clear();
    for( size_t i=0; i<totalFrameNum_; i++ ){
        //frame_data_ary_.push_back( std::vector<RGBA>{} );
        frame_data_ary_.emplace_back();
    }

    auto fit = frame_data_ary_.begin(); //- 指向某个 帧容器

    int wf {}; //-- 以帧为单位，目标像素在横排中 的序号
    int hf {}; //-- 以帧为单位，目标像素的 纵向 序号 (左下坐标系)
    size_t nrf {}; //-- 像素 属于的 帧序号

    RGBA *pixHeadPtr = (RGBA*)data;
    RGBA *pixPtr {nullptr}; //- tmp

    tprAssert( ((width%frameNum_.x)==0) && 
            ((height%frameNum_.y)==0) );
    IntVec2 pixNum_per_frame { width/frameNum_.x, height/frameNum_.y };

    //--- 遍历 png 中的 每一像素 ---
    for( int h=0; h<height; h++  ){
        for( int w=0; w<width; w++  ){

            //-- 计算 本像素 所属帧的容器 的迭代器 fit --
            wf = w/pixNum_per_frame.x;
            hf = h/pixNum_per_frame.y;
            hf = frameNum_.y - 1 - hf; 
                        //- 关键步骤！修正帧排序，(注意必须先减1，可画图验证)
                        //- 现在，帧排序从 左下 修正为 左上角坐标系

            int tmpInt = hf*frameNum_.x + wf;
            tprAssert( tmpInt >= 0 );
            nrf = cast_2_size_t( tmpInt );

            //-- 只处理 非空置的 frame ---
            if( nrf < totalFrameNum_ ){
                //-- 获得 本像素 的数据 --
                pixPtr = pixHeadPtr + (h*width + w);

                //-- 将数据 压入 对应的 帧容器 中 --
                fit = frame_data_ary_.begin();
                std::advance( fit, nrf ); //-- 事实上，std::advance 并不检测是否越界...
                fit->push_back( *pixPtr ); //-copy
            }
        }
    }
    //-- png图片 原始数据已经没用了，释放掉 ---
    stbi_image_free( data );

    return pixNum_per_frame;
}

