/*
 * ========================= tprGeneral.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    general funcs, support unix/win
 * ----------------------------
 */
#ifndef _TPR_GENERAL_H_
#define _TPR_GENERAL_H_

#include <string>

namespace tprGeneral {//--------- namespace: tprGeneral -------------//



const std::string path_combine( const std::string &_pa, const std::string &_pb );
const std::string path_combine( const char *_pa, const char *_pb );

const std::string nameString_combine(   const std::string &_prefix,
                                        size_t _idx,
                                        const std::string &_suffix );





}//------------------- namespace: tprGeneral ------------------------//
#endif 

