/*
 * ========================= main.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.08.23
 *                                        修改 -- 2018.08.23
 * ----------------------------------------------------------
 * 
 *    main();
 *    
 * ----------------------------
 */

//-------------------- C ----------------------//
#include <unistd.h> //-- pid_t，fork，getpid

#include <netinet/in.h> //- sockaddr_in
#include <sys/socket.h> //- socklen_t

#include  <arpa/inet.h>
#include  <netinet/in.h> //- htons

#include <string.h> //- memset, memcpy
#include <cassert> //-- assert

//-------------------- CPP --------------------//
#include <iostream> //-- cout
#include <string>
#include <vector>
#include <sstream>

//------------------- Libs --------------------//
#include "wrapFuncs.h"
#include "MsgIO.h"


//------------------- SELF --------------------//
#include "globfunc.h"
#include "global.h"

 
using std::cout;
using std::endl;
using std::string;


//------------------- 从外部获得的 函数 ----------------
extern void testMain_1();
extern void testMain_tprDB();


/* ===========================================================
 *                        main
 * -----------------------------------------------------------
 */
int main(){
    //==========================================//
    //                 prepare
    //------------------------------------------//
    prepare();
    
    //==========================================//
    //                  TEST
    //------------------------------------------//



    //testMain_1();
    testMain_tprDB();



    //------------//
    cout << "\n\n__ DEBUG _ RETURN __\n" << endl;
    return(0);
}







