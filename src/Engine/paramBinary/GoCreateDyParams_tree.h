/*
 * =================== GoCreateDyParams_tree.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_CREATE_DY_PARAMS_TREE_H
#define TPR_GO_CREATE_DY_PARAMS_TREE_H


//-------------------- Engine --------------------//
#include "MapAltitude.h"
#include "Density.h"
#include "ParamBinary.h"

//-- 简陋的临时版 ，用于 地景go 的创建
struct GoCreateDyParams_tree{

    double       fieldWeight         {};
	MapAltitude  fieldNodeMapEntAlti {};
	Density      fieldDensity        {};
};



#endif 

