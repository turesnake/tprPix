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

#include "esrc_state.h"

#include "ubo_all.h"


namespace esrc {//------------------ namespace: esrc -------------------------//
namespace ubo_inn {//-------- namespace: ubo_inn --------------//
 
    std::unordered_map<ubo::UBOType, std::unique_ptr<ubo::UniformBlockObj>> uboUPtrs {};

}//------------- namespace: ubo_inn end --------------//


void init_uniformBlockObjs()noexcept{

    {//---------- Seeds ------------//
        auto uboType = ubo::UBOType::Seeds;
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( sizeof(ubo::UBO_Seeds) );
        std::string uboName {"Seeds"};
        auto outPair = ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
        tprAssert( outPair.second );
    }

    {//---------- Camera ------------//
        auto uboType = ubo::UBOType::Camera;
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( ubo::UBO_Camera_size );
        std::string uboName {"Camera"};
        auto outPair = ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
        tprAssert( outPair.second );                            
    }

    {//---------- Window ------------//
        auto uboType = ubo::UBOType::Window;
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( sizeof(ubo::UBO_Window) );
        std::string uboName {"Window"};
        auto outPair = ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
        tprAssert( outPair.second );                            
    }

    {//---------- Time ------------//
        auto uboType = ubo::UBOType::Time;
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( sizeof(ubo::UBO_Time) );
        std::string uboName {"Time"};
        auto outPair = ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
        tprAssert( outPair.second );                            
    }

    {//---------- WorldCoord ------------//
        auto uboType = ubo::UBOType::WorldCoord;
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( sizeof(ubo::UBO_WorldCoord) );
        std::string uboName {"WorldCoord"};
        auto outPair = ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
        tprAssert( outPair.second );                            
    }

    {//---------- OriginColorTable ------------//
        auto uboType = ubo::UBOType::OriginColorTable;
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>(ColorTable::get_dataSize());
        std::string uboName {"OriginColorTable"};
        auto outPair = ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
        tprAssert( outPair.second );                            
    }

    {//---------- UnifiedColorTable ------------//
        auto uboType = ubo::UBOType::UnifiedColorTable;
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>(ColorTable::get_dataSize());
        std::string uboName {"UnifiedColorTable"};
        auto outPair = ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
        tprAssert( outPair.second );                            
    }

    {//---------- GroundColorTable ------------//
        auto uboType = ubo::UBOType::GroundColorTable;
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( sizeof(FloatVec4) * 20 );// shader 中手写了 [20]数组，丑陋的方案...
        std::string uboName {"GroundColorTable"};
        auto outPair = ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
        tprAssert( outPair.second );                            
    }

    {//---------- ColorTableId ------------//
        auto uboType = ubo::UBOType::ColorTableId;
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = sizeof(colorTableId_t);
        std::string uboName {"ColorTableId"};
        auto outPair = ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
        tprAssert( outPair.second );                            
    }

    //...
    esrc::insertState("ubo");
}


ubo::UniformBlockObj &get_uniformBlockObjRef( ubo::UBOType type_ )noexcept{
    tprAssert( ubo_inn::uboUPtrs.find(type_) != ubo_inn::uboUPtrs.end() );
    return *(ubo_inn::uboUPtrs.at(type_));
}


}//---------------------- namespace: esrc -------------------------//

