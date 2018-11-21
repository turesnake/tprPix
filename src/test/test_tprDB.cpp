/*
 * ========================= test_tprDB.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.14
 *                                        修改 -- 2018.10.14
 * ----------------------------------------------------------
 * 
 *  tprDB 专用 测试
 * 
 * ----------------------------
 */

//-------------------- C --------------------//
#include <unistd.h> //- size_t, sysconf

#include <sys/stat.h> //- stat
#include <string.h> //-- memcpy
#include <errno.h> //- errno

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>
#include <vector> 
#include <sstream> //- stringstream
#include <iomanip> //- setw, setfill
#include <deque> 
#include <list> 

//-------------------- Libs --------------------//
#include "tprFileModeT.h"
#include "wrapFuncs.h"
#include "tprFileSys.h" 
#include "tprCrypto.h" 
#include "tprDataType.h" 
#include "tprBinary.h" 
#include "tprCrypto.h" 

#include "tprDB.h"

//-------------------- SELF --------------------//
#include "global.h" 
#include "RegData.h" 


using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;
using std::deque;
using std::list;

using tpr::DB::len_t;
using tpr::DB::base_t;
using tpr::DB::eid_t;
using tpr::DB::DATA_T;






//------------------- 提供给外部的 函数 ----------------
void testMain_tprDB();


namespace{




}



/* ===========================================================
 *                  testMain_tprDB
 * -----------------------------------------------------------
 */
void testMain_tprDB(){

    string err_info = "testMain_tprDB(): ";


    

    RegData sd;

    //sd.insert( RegData_User{ "ssss", 333 } );

    RegData_User rdu;
    //sd.read( 4, &rdu );

    //sd.erase( 2 );


    //sd.write( 3, RegData_User{ "lll", 567 }, RegData_Mask{ true, true } );

    //sd.write( 4, RegData_User{ "", 0 }, RegData_Mask{ false, false } );

    //sd.write( 3, RegData_User{ "aaaaaa", 0 }, RegData_Mask{ true, false } );
    sd.write( 3, RegData_User{ "", 55555 }, RegData_Mask{ false, true } );


    sd.info( true, true );
    //-----------------//

}

namespace{ //------------- namespace -------------//
























}//------ namespace ----


