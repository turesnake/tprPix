/*
 * ========================= tprDebug.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.05
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    MOST  POOR DEBUG SYSTEM
 * ----------------------------
 */
#ifndef TPR_DEBUG_H
#define TPR_DEBUG_H

#include <utility>
#include <string>
#include "fmt/format.h"



namespace tprDebug {//---------- namespace: tprDebug --------------//


// 简单地打印一段 字符串到 console
// 推荐使用 fmt 拼接

void tmp_console_inn( const std::string &str_ );


// 简单的套娃
template < typename S, typename... Args >
void console(const S& format_str, Args&&... args) {
    
    auto str = fmt::format( format_str, std::forward<Args>(args)... );
    tmp_console_inn( str );
}



}//-------------------- namespace: tprDebug end --------------//
#endif 

