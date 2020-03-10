/*
 * ========================= tprDebug.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    MOST  POOR DEBUG SYSTEM
 * ----------------------------
 */
#include "pch.h"
#include "tprDebug.h"

// 临时版 ...
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::flush;
using std::string;


// spdlog
/*
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
*/


//-------------------- Engine --------------------//



namespace tprDebug {//---------- namespace: tprDebug --------------//

//namespace _inn {//-------- namespace: _inn ----------//
//}//-------------- namespace: _inn end --------//



void tmp_console_inn( const std::string &str_ ){
    cout << str_ << endl;

            // 未来改用 spdlog
            // ...
}




}//-------------------- namespace: tpr_debug end --------------//

