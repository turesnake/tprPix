/*
 * ========================= esrc_state.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.29
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_ESRC_STATE_H
#define TPR_ESRC_STATE_H

//-------------------- CPP --------------------//
#include <string>

namespace esrc {//------------------ namespace: esrc -------------------------//

void insertState( const std::string &str_ );
bool is_setState( const std::string &str_ );

}//---------------------- namespace: esrc -------------------------//
#endif

