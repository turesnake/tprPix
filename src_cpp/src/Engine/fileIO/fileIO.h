/*
 * ======================= fileIO.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.07.08
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_FILE_IO_H
#define TPR_FILE_IO_H

//--------------- CPP ------------------//
#include <string>
#include <memory>

std::unique_ptr<std::string> read_a_file( const std::string &filePath_ );

#endif 

