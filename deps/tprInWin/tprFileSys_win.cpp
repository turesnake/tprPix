/*
 * ========================= tprFileSys_win.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//-------------------- WIN --------------------//
#include <windows.h>

//-------------------- CPP --------------------//
#include <iostream>
#include <sstream>

//-------------------- self --------------------//
#include "tprFileSys_win.h"

#include "tprGeneral.h"


namespace tprWin {//--------------- namespace: tprWin -------------------//



/* ===========================================================
 *                     mk_dir
 * -----------------------------------------------------------
 * -- 简陋版 目录生成
 */
const std::string mk_dir(const std::string &path_dir_,
						 const std::string &name_,
						 const std::string &err_info_ ){

	std::string err_info = err_info_ + "mk_dir(): ";
	std::string path = tprGeneral::path_combine( path_dir_, name_ );
	
	if( CreateDirectory(path.c_str(), nullptr) ||
		GetLastError() == ERROR_ALREADY_EXISTS ){
		//-- done
	}else{
        std::stringstream ss;
        ss << err_info
            << "path = " << path;
        MessageBox( nullptr, ss.str().c_str(), "ERROR", MB_OK );
        exit(-99);
	}

	return path;
}


/* ===========================================================
 *                      file_load
 * -----------------------------------------------------------
 * 返回 实际读取的 bytes
 */
int32_t file_load( const std::string &path_,
				 std::string &buf_){

	//--- create ---//
	HANDLE hFile = (HANDLE)CreateFile(path_.c_str(),
									GENERIC_READ,
									FILE_SHARE_READ,
									NULL,
									OPEN_EXISTING,
									NULL,
									NULL );
	if( hFile == INVALID_HANDLE_VALUE ){
		hFile = NULL;
        std::stringstream ss;
        ss << "CreateFile(); INVALID_HANDLE_VALUE\n"
            << "path = " << path_;
		MessageBox( nullptr, ss.str().c_str(), "Error", MB_OK );
		exit(-99);
	}

	//--- get bytes ---//
	LARGE_INTEGER fileBytes;
	BOOL ret = GetFileSizeEx( hFile, &fileBytes );
	if( ret == FALSE ){
		hFile = NULL;
        std::stringstream ss;
		ss << "CreateFile(); GetFileSizeEx()==FALSE\n"
            << "path = " << path_;
		MessageBox( nullptr, ss.str().c_str(), "Error", MB_OK );
		exit(-99);
	}


	//--- read ---//
	buf_.resize( static_cast<std::string::size_type>(fileBytes.QuadPart) );
	DWORD readedBytes;
	ret = ReadFile(hFile,
				   static_cast<LPVOID>(&(buf_.at(0))),
				   static_cast<DWORD>(fileBytes.QuadPart),
				   &readedBytes,
				   NULL );

	//--- close ---//
	ret = CloseHandle( hFile );
	if( ret == FALSE ){
        std::stringstream ss;
		ss << "CreateFile(); CloseHandle()==FALSE\n"
            << "path = " << path_;
		MessageBox( nullptr, ss.str().c_str(), "Error", MB_OK );
		exit(-99);
	}
	return static_cast<int32_t>(readedBytes);
}





}//--------------- namespace: tprWin end -------------------//



