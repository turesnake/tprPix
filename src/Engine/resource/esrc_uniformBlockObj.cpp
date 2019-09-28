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
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( sizeof(ubo::UBO_Seeds) );
        std::string uboName {"Seeds"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    {//---------- Camera ------------//
        auto uboType = ubo::UBOType::Camera;
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( ubo::UBO_Camera_size );
        std::string uboName {"Camera"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    {//---------- Window ------------//
        auto uboType = ubo::UBOType::Window;
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( sizeof(ubo::UBO_Window) );
        std::string uboName {"Window"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    {//---------- Time ------------//
        auto uboType = ubo::UBOType::Time;
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( sizeof(ubo::UBO_Time) );
        std::string uboName {"Time"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    {//---------- OriginColorTable ------------//
        auto uboType = ubo::UBOType::OriginColorTable;
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>(ColorTable::get_dataSize());
        std::string uboName {"OriginColorTable"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    {//---------- UnifiedColorTable ------------//
        auto uboType = ubo::UBOType::UnifiedColorTable;
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>(ColorTable::get_dataSize());
        std::string uboName {"UnifiedColorTable"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    {//---------- GroundColorTable ------------//
        auto uboType = ubo::UBOType::GroundColorTable;
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = static_cast<GLsizeiptr>( sizeof(FloatVec4) * 20 );// shader 中手写了 [20]数组，丑陋的方案...
        std::string uboName {"GroundColorTable"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    {//---------- ColorTableId ------------//
        auto uboType = ubo::UBOType::ColorTableId;
        tprAssert( ubo_inn::uboUPtrs.find(uboType) == ubo_inn::uboUPtrs.end() );
        GLuint bindPoint = ubo::get_bindPoint(uboType);
        GLsizeiptr dataSize = sizeof(colorTableId_t);
        std::string uboName {"ColorTableId"};
        ubo_inn::uboUPtrs.insert({  uboType, 
                                    std::make_unique<ubo::UniformBlockObj>(bindPoint, dataSize, uboName) });
    }

    //...
    esrc::insertState("ubo");
}


ubo::UniformBlockObj &get_uniformBlockObjRef( ubo::UBOType type_ )noexcept{
    tprAssert( ubo_inn::uboUPtrs.find(type_) != ubo_inn::uboUPtrs.end() );
    return *(ubo_inn::uboUPtrs.at(type_).get());
}


}//---------------------- namespace: esrc -------------------------//

