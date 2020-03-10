/*
 * ======================= assemble_go.h =====================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_ASSEMBLE_GO_H
#define TPR_ASSEMBLE_GO_H


//-------------------- Engine --------------------//
//...

// need
class GameObj;
class DyParam;
class GoDataForCreate;


const GoDataForCreate *assemble_regularGo( GameObj &goRef_,const DyParam &dyParams_ );
const GoDataForCreate *assemble_regularGo( GameObj &goRef_, const GoDataForCreate *goDataPtr_ );


#endif

