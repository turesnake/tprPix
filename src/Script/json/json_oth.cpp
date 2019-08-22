/*
 * ======================= json_oth.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/json/json_oth.h"

//--------------- CPP ------------------//
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"

#include "tprDebug.h" 


using namespace rapidjson;


namespace json_inn {//-------- namespace: json_inn --------------//


/* ===========================================================
 *                    get_float
 * -----------------------------------------------------------
 */
float get_float( const Value &val_ ){
    if( val_.IsFloat() ){
        return val_.GetFloat();
    }else if( val_.IsInt() ){
        return static_cast<float>( val_.GetInt() );
    }else{
        tprAssert(0);
        return 0.0f; //- never reach
    }
}


/* ===========================================================
 *                  get_double
 * -----------------------------------------------------------
 */
double get_double( const Value &val_ ){
    if( val_.IsDouble() ){
        return val_.GetDouble();
    }else if( val_.IsInt() ){
        return static_cast<double>( val_.GetInt() );
    }else{
        tprAssert(0);
        return 0.0; //- never reach
    }
}

/* ===========================================================
 *                 get_nullable_int
 * -----------------------------------------------------------
 */
std::pair<bool, int> get_nullable_int( const Value &val_ ){
    std::string nil {"nil"};
    if( val_.IsInt() ){
        return std::pair<bool,int>{ true, val_.GetInt() };
    }else if( val_.IsString() && (val_.GetString()==nil) ){
        return std::pair<bool,int>{ false, 0 };
    }else{
        tprAssert(0);
        return std::pair<bool,int>{ false, 0 }; //- never reach
    }
}

/* ===========================================================
 *                check_and_get_value
 * -----------------------------------------------------------
 */
const rapidjson::Value &check_and_get_value( const rapidjson::Value &val_,
                                            const std::string &name_,
                                            JsonValType jsonValType_ ){
    tprAssert( val_.HasMember(name_.c_str()) );
    const Value &a = val_[name_.c_str()];
    switch (jsonValType_){
        case JsonValType::Object: tprAssert( a.IsObject() ); break;
        case JsonValType::Array:  tprAssert( a.IsArray() ); break;
        case JsonValType::Bool:   tprAssert( a.IsBool() ); break;
        case JsonValType::String: tprAssert( a.IsString() ); break;
        case JsonValType::Number: tprAssert( a.IsNumber() ); break;
        case JsonValType::Int:    tprAssert( a.IsInt() ); break;
        case JsonValType::Int64:  tprAssert( a.IsInt64() ); break;
        case JsonValType::Uint:   tprAssert( a.IsUint() ); break;
        case JsonValType::Uint64: tprAssert( a.IsUint64() ); break;
        case JsonValType::Float:  tprAssert( a.IsFloat() ); break;
        case JsonValType::Double: tprAssert( a.IsDouble() ); break;
        default:
            tprAssert(0);
    }
    return a;
}





}//------------- namespace: json_inn end --------------//
