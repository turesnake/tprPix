

#ifndef TPR_DY_PARAM_H
#define TPR_DY_PARAM_H

//-- 尝试通过 继承来实现 ....

#include "tprDebug.h"


class DyParam{
public:
    DyParam()
        {
            cout << "DyParam: constructor;" << endl;
        };


    virtual ~DyParam(){
        cout << "DyParam: destructor;" << endl;
    };

    virtual void foo();



};




class DyParam_A : public DyParam{
public:
    DyParam_A()
        {
            cout << "DyParam_A: constructor;" << endl;
        }

    ~DyParam_A(){
        cout << "DyParam_A: destructor;" << endl;
    }


};








#endif 

