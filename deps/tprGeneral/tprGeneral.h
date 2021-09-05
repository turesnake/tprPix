/*
 * ========================= tprGeneral.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    general funcs, support unix/win
 * ----------------------------
 */
#ifndef TPR_GENERAL_H_
#define TPR_GENERAL_H_

#include <string>

namespace tprGeneral {//--------- namespace: tprGeneral -------------//



std::string path_combine( const std::string &_pa, const std::string &_pb );
std::string path_combine( const char *_pa, const char *_pb );


std::string nameString_combine(   const std::string &_prefix,
                                        size_t _idx,
                                        const std::string &_suffix );
                                        

// 获得路径名中，目标文件所在的 上一级目录 的 path
std::string get_dirPath( const std::string &path_ );



}//------------------- namespace: tprGeneral ------------------------//
#endif 

