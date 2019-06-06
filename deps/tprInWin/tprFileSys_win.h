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

#include "tprDataType.h"


namespace tprWin {//--------------- namespace: tprWin -------------------//


const std::string mk_dir(const std::string &_path_dir,
						 const std::string &_name,
						 const std::string &_err_info );


i32_t file_load( const std::string &_path,
				 std::string &_buf);



}//--------------- namespace: tprWin end -------------------//

#endif
