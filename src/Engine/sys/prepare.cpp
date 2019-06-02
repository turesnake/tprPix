/*
 * ========================= prepare.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//---------------- from cmake ------------------//
#include "SysConfig.h" // MUST BEFORE _TPR_OS_WIN32_ !!!

//-------------------- C ----------------------//
#ifdef _TPR_OS_WIN32_
	#include <windows.h>
#else
	#include <unistd.h>  //- fchdir
	#include <fcntl.h>    //-- open，openat, AT_FDCWD
#endif

#include <cassert>

//-------------------- CPP --------------------//
#include <string> 
#include <fstream>   //-- ifstream
#include <sstream>   //-- stringstream

//------------------- Libs --------------------//
#include "tprGeneral.h"


#ifdef _TPR_OS_WIN32_
    #include "tprFileSys_win.h" 
#else
    #include "wrapUnix.h"
    #include "tprFileModeT.h"
    #include "tprFileSys_unix.h" 
#endif

//-------------------- Engine --------------------//
#include "global.h"



#include <iostream>
using std::cout;
using std::endl;



namespace {//------------ namespace ------------//
    void build_path_cwd();
    void data_type_confirm();
    void check_OS();
    //void check_fst_run();
    void check_and_creat_important_dir();
}//------------ namespace: end ------------//



/*===========================================================
 *                      prepare  
 *-----------------------------------------------------------
 */
void prepare(){

    //------------------------//
    //      变量类型检测
    //------------------------//
    data_type_confirm();

    //------------------------//
    //   查看当前所在的 操作系统
    //------------------------//
    check_OS();

    //------------------------//
    //    初始化 path_cwd
    //------------------------//
    build_path_cwd();

    //------------------------//
    // 检测 关键 目录 的存在，若没有，创建之
    //------------------------//
    check_and_creat_important_dir();


    //----------------------------//
    //  检测 是否为 本程序的 首次运行
    //----------------------------//
    //check_fst_run();

    return;
}

namespace {//------------ namespace ------------//


/* ===========================================================
 *                  build_path_cwd   
 * -----------------------------------------------------------
 */
void build_path_cwd(){


#ifdef _TPR_OS_WIN32_

	char buf[MAX_PATH];
	GetModuleFileName( NULL, buf, MAX_PATH );
	// 当前 buf数据 为 ".../xx.exe"
	// 需要将 最后一段 截掉
	std::string::size_type pos = std::string(buf).find_last_of( "\\/" );
	path_cwd = std::string(buf).substr( 0, pos );

#else
    //----------------------------//
    //       MODIFY 当前工作目录
    //----------------------------//
    //    这个 fd_cwd 好想 从未被 用过...
    //-- 始终将 项目根目录 设置为 当前工作目录。
    if( (fd_cwd = openat( AT_FDCWD, "..", (O_RDONLY | O_DIRECTORY) )) == -1 ){
        //cout << "prepare: openat: ERROR. fd_cwd.\n" 
        //    << endl;
        assert(0);
    }
    if( fchdir( fd_cwd ) == -1 ){ //-- 将 system目录的 父目录 设为 当前工作目录
        //cout << "prepare: fchdir: ERROR. \n"
        //    << endl;
        assert(0);
    }

    //----------------------------//
    //    获得 当前工作目录 路径名
    //----------------------------//
    char cpath_cwd[1024];
    if( getcwd( cpath_cwd, 1024 ) == NULL ){
        //cout << "prepare: getcwd: ERROR.\n"
        //    << endl;
        assert(0);
    }

    //---  通过临时 string 对象，将 字符串 转存到 string对象：path_cwd 中。
    std::string s( cpath_cwd );
    path_cwd = s;
    
#endif

}


/*===========================================================
 *           check_and_creat_important_dir    
 *-----------------------------------------------------------
 */
void check_and_creat_important_dir(){

    std::string err_info = "check_and_creat_important_dir(): ";

    //----------------------------//
    //  已经确认的 目录：
    //      path_cwd


#ifdef _TPR_OS_WIN32_

    //---------------------------------//
    //           path_data
    //---------------------------------//
    path_dataBase = tprWin::mk_dir( path_cwd,
                                "dataBase/",
                                err_info );
    //---------------------------------//
    //           path_shaders
    //---------------------------------//
    path_shaders = tprWin::mk_dir( path_cwd,
                                "shaders/",
                                err_info );
    //---------------------------------//
    //           path_textures
    //---------------------------------//
    path_textures = tprWin::mk_dir( path_cwd,
                                "textures/",
                                err_info );
    //---------------------------------//
    //          path_animFrameSets
    //---------------------------------//
    path_animFrameSets = tprWin::mk_dir( path_textures,
                                    "animFrameSets/",
                                    err_info );
    //---------------------------------//
    //          path_colliEntSet
    //---------------------------------//
    path_colliEntSet = tprWin::mk_dir( path_textures,
                                    "colliEntSet/",
                                    err_info );
    //---------------------------------//
    //          path_fieldBorderSet
    //---------------------------------//
    path_fieldBorderSet = tprWin::mk_dir( path_textures,
                                        "fieldBorderSet/",
                                        err_info );

#else
    //---------------------------------//
    //           path_home
    //---------------------------------//
    //   从未被使用...
    path_home = tprUnix::Getenv( "HOME", err_info );

    //---------------------------------//
    //           path_data
    //---------------------------------//
    path_dataBase = tprUnix::mk_dir( path_cwd,
                        "dataBase/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //           path_shaders
    //---------------------------------//
    path_shaders = tprUnix::mk_dir( path_cwd,
                        "shaders/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //           path_textures
    //---------------------------------//
    path_textures = tprUnix::mk_dir( path_cwd,
                        "textures/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //          path_animFrameSets
    //---------------------------------//
    path_animFrameSets = tprUnix::mk_dir( path_textures,
                        "animFrameSets/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //          path_colliEntSet
    //---------------------------------//
    path_colliEntSet = tprUnix::mk_dir( path_textures,
                        "colliEntSet/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //          path_fieldBorderSet
    //---------------------------------//
    path_fieldBorderSet = tprUnix::mk_dir( path_textures,
                        "fieldBorderSet/",
                        RWXR_XR_X,
                        err_info );
#endif
}


/* ===========================================================
 *                  data_type_confirm    
 * -----------------------------------------------------------
 * -- 一种简陋的 类型检测。
 */
void data_type_confirm(){

    int len; //-- 临时变量。
    //------- short ------//
    len = sizeof(short);
    if( len != 2 ){
        //cout << "data_type_confirm: ERROR."
        //    << "short len = " << len
        //    << endl;
        assert(0);
    }

    //------- unsigned short ------//
    len = sizeof(unsigned short);
    if( len != 2 ){
        //cout << "data_type_confirm: ERROR."
        //    << "unsigned short len = " << len
        //    << endl;
        assert(0);
    }

    //------- int ------//
    len = sizeof(int);
    if( len != 4 ){
        //cout << "data_type_confirm: ERROR."
        //    << "int len = " << len
        //    << endl;
        assert(0);
    }

    //------- unsigned int ------//
    len = sizeof(unsigned int);
    if( len != 4 ){
        //cout << "data_type_confirm: ERROR."
        //    << "unsigned int len = " << len
        //    << endl;
        assert(0);
    }

    //------- long long ------//
    len = sizeof(long long);
    if( len != 8 ){
        //cout << "data_type_confirm: ERROR."
        //    << "long long len = " << len
        //    << endl;
        assert(0);
    }

    //------- unsigned long long ------//
    len = sizeof(unsigned long long);
    if( len != 8 ){
        //cout << "data_type_confirm: ERROR."
        //    << "unsigned long long len = " << len
        //    << endl;
        assert(0);
    }

    //------- float ------//
    len = sizeof(float);
    if( len != 4 ){
        //cout << "data_type_confirm: ERROR."
        //    << "float len = " << len
        //   << endl;
        assert(0);
    }

    //------- double ------//
    len = sizeof(double);
    if( len != 8 ){
        //cout << "data_type_confirm: ERROR."
        //    << "double len = " << len
        //   << endl;
        assert(0);
    }

    //------- off_t ------//
    /*
    len = sizeof(off_t);
    if( len != 8 ){
        //cout << "data_type_confirm: ERROR."
        //    << "off_t len = " << len
        //    << endl;
        assert(0);
    }
    */
         //-- unix 中为 8， win 中为 4
         //   暂不处理...
}


/* ===========================================================
 *                     check_OS
 * -----------------------------------------------------------
 * -- 检测 当前 程序 身处什么 操作系统 
 */
void check_OS(){

    cout << SYSTEM_NAME << endl;

    current_OS = OS_NULL; //-- 先设置为 未找到状态

    #ifdef _TPR_OS_APPLE_
        current_OS = OS_APPLE;
        cout << "_TPR_OS_APPLE_" << endl;
    #endif 

    #ifdef _TPR_OS_UNIX_
        current_OS = OS_UNIX;
        cout << "_TPR_OS_UNIX_" << endl;
    #endif 

    #ifdef _TPR_OS_WIN32_
        current_OS = OS_WIN32;
        cout << "_TPR_OS_WIN32_" << endl;
    #endif 
    

}


/* ===========================================================
 *                       check_fst_run  
 * -----------------------------------------------------------
 * -- 检查本程序 是否为 编译后 首次运行，如果是，需要执行一些 初始化工作
 */
/*
void check_fst_run(){

    //-- 合成 already_run 文件 绝对路径名 
    std::string path = tpr::path_combine( path_cwd, "/build/already_run.flag" );

    //-- 查看 already_run 文件 是否存在，若存在，open return -1， 若不存在，创建它。
    if( open(path.c_str(),(O_RDONLY | O_CREAT | O_EXCL),RWXR_XR_X) >= 0 ){
        //-- 说明 目标文件 不存在： 本程序第一次运行 --
        //cout << "+++ fst run. +++" << endl;
        is_fst_run = true;

    }else{
        //-- 说明 目标文件 存在： 本程序 不是 第一次运行 --
        //cout << "+++ not fst run. +++" << endl;
        is_fst_run = false;
    }
}
*/


}//------------- namespace ----------------//

