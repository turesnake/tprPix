/*
 * ========================= tprGeneral.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "tprGeneral.h"

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>
#include <sstream>


namespace tprGeneral {//--------- namespace: tprGeneral -------------//


/* ===========================================================
 *                  nameString_combine
 * -----------------------------------------------------------
 * -- 拼接一种特殊的 string，类似 "dog_2_jump" 这种
 */
std::string nameString_combine(   const std::string &_prefix,
                                        size_t _idx,
                                        const std::string &_suffix ){
    std::stringstream ss;
    ss.str("");
    ss << _idx;
    return (_prefix + ss.str() + _suffix);
}




/* ===========================================================
 *                       path_combine [1]
 * -----------------------------------------------------------
 * -- 将 _pa，_pb 合成一个 合法的 路径名。（主要是 处理中间的 '/' 问题）
 * -- 目前，不管调用者在 _pa，_pb 间写没写 '/', 都能正确合成。 
 *    '/' 在 win 中也可使用，所以，这个函数算是 跨平台的
 */
std::string path_combine( const std::string &_pa, const std::string &_pb ){

    std::string err_info = "path_combine(): ";
    //---------------------

    std::string path; //-- 最终合成的 路径名
    std::string pa = _pa;
    std::string pb = _pb;

    bool is_pa_has_slash = false;
    bool is_pb_has_slash = false;
    //---------------------
    if( pa.back() == '/' ){
        is_pa_has_slash = true;
    }
    if( pb.front() == '/' ){
        is_pb_has_slash = true;
    }
    //---------------------
    if( (is_pa_has_slash ^ is_pb_has_slash) == true ){ //-- 异或
        //---- 若 只有一个 拥有 '/' ----//
    }else if( is_pa_has_slash == true ){
        //---- 若 两个都有 '/' ----//
        pa.pop_back();
    }else{
        //---- 若 两个都无 '/' ----//
        pa += "/";
    }
    //---------------------
    path = pa + pb;
    //------ 检测 path 长度 -------
    //Is_path_not_too_long( path, err_info );
            //-- 暂不检测...

    return path;
}



/* ===========================================================
 *                       path_combine [2]
 * -----------------------------------------------------------
 * -- 重载版本，参数有变，不推荐使用此版 
 */
std::string path_combine( const char *_pa, const char *_pb ){

    std::string a = _pa;
    std::string b = _pb;
    return path_combine( a, b );
}



std::string get_dirPath( const std::string &path_ ){

    // 默认 参数 path_ 是用 "/" 分隔的 路径名
    // 不做检测 ...
    size_t slashIdx = path_.find_last_of("/");
    auto itEnd = path_.begin(); // 临时指向头部
    std::advance( itEnd, slashIdx ); //- 将迭代器 itS 前移. advance 并不防止 溢出

    std::string dirPath {};
    dirPath.assign( path_.begin(), itEnd );
    //---
    return dirPath;
}







}//------------------- namespace: tprGeneral ------------------------//

