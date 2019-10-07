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

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

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


void collect_fileNames( const std::string &dirName_,
                        const std::string &headFileName_,
                        std::vector<std::string> &container_ );


}//------------- namespace json: end ----------------
#endif 

