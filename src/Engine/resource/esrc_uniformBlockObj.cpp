/*
 * =================== esrc_uniformBlockObj.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.23
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#include "esrc_uniformBlockObj.h"
#include <memory>

//-------------------- Engine --------------------//
#include "uniformBlockObjs.h"
#include "tprAssert.h"
#include "ColorTable.h"

#include "UBO_Seeds.h"
#include "UBO_Camera.h"


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace ubo_inn {//-------- namespace: ubo_inn --------------//
 
    std::unordered_map<UBOType, std::unique_ptr<UniformBlockObj>> uboUPtrs {};

}//------------- namespace: ubo_inn end --------------//


//-- must before esrc::init_shaders() !!! ---
void init_uniformBlockObjs()noexcept{

    {//---------- Seeds ------------//
        UBOType uboType = UBOType::Seeds;
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = uboBindPoints.at(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( sizeof(UBO_Seeds) );
        std::string uboName {"Seeds"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    {//---------- Camera ------------//
        UBOType uboType = UBOType::Camera;
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = uboBindPoints.at(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( UBO_Camera_size );
        std::string uboName {"Camera"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    {//---------- OriginColorTable ------------//
        UBOType uboType = UBOType::OriginColorTable;
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = uboBindPoints.at(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>(ColorTable::get_dataSize());
        std::string uboName {"OriginColorTable"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    {//---------- UnifiedColorTable ------------//
        UBOType uboType = UBOType::UnifiedColorTable;
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = uboBindPoints.at(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>(ColorTable::get_dataSize());
        std::string uboName {"UnifiedColorTable"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    //...
}


UniformBlockObj &get_uniformBlockObjRef( UBOType type_ )noexcept{
    tprAssert( ubo_inn::uboUPtrs.find(type_) != ubo_inn::uboUPtrs.end() );
    return *(ubo_inn::uboUPtrs.at(type_).get());
}




}//---------------------- namespace: esrc -------------------------//

