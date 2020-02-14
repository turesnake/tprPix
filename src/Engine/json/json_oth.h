/*
 * ======================= json_oth.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.07.06
 *                                        修改 -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_JSON_OTH_H
#define TPR_JSON_OTH_H

//--------------- CPP ------------------//
#include <utility>
#include <vector>
#include <string>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

//--------------- Engine ------------------//
#include "tprAssert.h"


namespace json{//------------- namespace json ----------------

float get_float( const rapidjson::Value &val_ );
double get_double( const rapidjson::Value &val_ );
std::pair<bool, int> get_nullable_int( const rapidjson::Value &val_ );


enum class JsonValType : int{
    Object,
    Array,
    Bool,
    String,
    Number,
    Int,
    Int64,
    Uint,
    Uint64,
    Float,
    Double
};

const rapidjson::Value &check_and_get_value( const rapidjson::Value &val_,
                                            const std::string &name_,
                                            JsonValType jsonValType_ );


void collect_fileNames( const std::string &headPath_,
                        const std::string &dirName_,
                        const std::string &headFileName_,
                        std::vector<std::string> &container_ );



inline std::string get_jsonFile_dirPath( const std::string &jsonFile_ )noexcept{

    // 暂时只支持 Unix 风格的 path 格式
    // 在本游戏中使用，暂无问题
    tprAssert( jsonFile_.find('/') != std::string::npos );
    size_t last_point_idx = jsonFile_.find_last_of( '/' ); //- 指向最后一个 '/'

    auto lastIt = jsonFile_.begin();
    std::advance( lastIt, last_point_idx ); //- advance 并不防止 溢出

    std::string ret (jsonFile_.begin(), lastIt);
    return ret;
}





}//------------- namespace json: end ----------------
#endif 

