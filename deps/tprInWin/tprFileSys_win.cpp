/*
 * ========================= tprFileSys_win.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- WIN --------------------//
#include <windows.h>

//-------------------- C --------------------//
#include <cassert>

//-------------------- CPP --------------------//
#include <iostream>

//-------------------- self --------------------//
#include "tprFileSys_win.h"

#include "tprGeneral.h"


namespace tprWin {//--------------- namespace: tprWin -------------------//


/* ===========================================================
 *                       path_combine [1]
 * -----------------------------------------------------------
 */
/*
const std::string path_combine( const std::string &_pa, const std::string &_pb ){

    std::string err_info = "path_combine(): ";
    //---------------------
    std::string path; 
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
    if( (is_pa_has_slash ^ is_pb_has_slash) == true ){ 
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
				//- 暂不使用...
    return path;
}
*/



/* ===========================================================
 *                     mk_dir
 * -----------------------------------------------------------
 * -- 简陋版 目录生成
 */
const std::string mk_dir(const std::string &_path_dir,
						 const std::string &_name,
						 const std::string &_err_info ){

	std::string err_info = _err_info + "mk_dir(): ";
	std::string path = tprGeneral::path_combine( _path_dir, _name );
	
	if( CreateDirectory(path.c_str(), NULL) ||
		GetLastError() == ERROR_ALREADY_EXISTS ){
		//-- done
	}else{
		std::cout << err_info << "ERROR. "
				<< "path = " << path 
				<< std::endl;
		assert(0);
	}

	return path;
}


/* ===========================================================
 *                      file_load
 * -----------------------------------------------------------
 * 返回 实际读取的 bytes
 */
i32_t file_load( const std::string &_path,
				 std::string &_buf){

	//--- create ---//
	HANDLE hFile = (HANDLE)CreateFile(_path.c_str(),
									GENERIC_READ,
									FILE_SHARE_READ,
									NULL,
									OPEN_EXISTING,
									NULL,
									NULL );
	if( hFile == INVALID_HANDLE_VALUE ){
		hFile = NULL;
		MessageBox( NULL, "CreateFile()", "Error", MB_OK );
		assert(0);
	}

	//--- get bytes ---//
	LARGE_INTEGER fileBytes;
	BOOL ret = GetFileSizeEx( hFile, &fileBytes );
	if( ret == FALSE ){
		hFile = NULL;
		MessageBox( NULL, "GetFileSizeEx()", "Error", MB_OK );
		assert(0);
	}


	//--- read ---//
	_buf.resize( fileBytes.QuadPart );
	DWORD readedBytes;
	ret = ReadFile(hFile,
				   (LPVOID)&(_buf.at(0)),
				   static_cast<DWORD>(fileBytes.QuadPart),
				   &readedBytes,
				   NULL );
		//cout << _buf << endl;

	//--- close ---//
	ret = CloseHandle( hFile );
	if( ret == FALSE ){
		MessageBox( NULL, "CloseHandle()", "Error", MB_OK );
		assert(0);
	}
	return readedBytes;
}





}//--------------- namespace: tprWin end -------------------//



