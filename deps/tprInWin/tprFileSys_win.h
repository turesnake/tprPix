/*
 * ========================= tprFileSys_win.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   简陋的 win平台 常用代码库
 * ----------------------------
 */
#ifndef TPR_FILE_SYS_WIN_H_
#define TPR_FILE_SYS_WIN_H_

#include <string>

namespace tprWin {//--------------- namespace: tprWin -------------------//


const std::string mk_dir(const std::string &path_dir_,
						 const std::string &name_,
						 const std::string &err_info_ );


int32_t file_load( const std::string &path_,
				 std::string &buf_);



}//--------------- namespace: tprWin end -------------------//

#endif
