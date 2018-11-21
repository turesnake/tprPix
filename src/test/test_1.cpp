/*
 * ========================= test_1.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.10.14
 *                                        修改 -- 2018.10.14
 * ----------------------------------------------------------
 * 
 *   
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
#include "tprDB.h" 
#include "tprFileSys.h" 
#include "tprCrypto.h" 
#include "tprDataType.h" 
#include "tprBinary.h" 
#include "tprCrypto.h" 

//-------------------- SELF --------------------//
#include "global.h" 

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


struct DBSSS{
    tpr::DB::T_STRID name;  //- DATA_T::STRID
    tpr::DB::T_I32   type;  //- DATA_T::I32
    tpr::DB::T_I32   id;    //- DATA_T::I32
}; 



//------------------- 提供给外部的 函数 ----------------
void testMain_1();






/* ===========================================================
 *                  testMain_1    
 * -----------------------------------------------------------
 */
void testMain_1(){


    //---------------- checksum ---------



    


    //-----------------//

}




