/*******************************************************************
 *
 *  DESCRIPTION: Atomic Model Receiver
 *
 *  AUTHOR: Tao Zheng
 *
 *  EMAIL: mailto://zhengtao@sce.carleton.ca
 *
 *  DATE: 15/10/2002
 *
 *  REUSED and MODIFIED by AUTHOR: Weiwei Li
 *
 *  EMAIL: mailto://wli086@uottawa.ca
 *
 *  DATE: 13/10/2012
 *******************************************************************/

#ifndef __RECEIVER_H
#define __RECEIVER_H

#include "atomic.h"     // class Atomic

class Receiver : public Atomic
    {
    public:
        //Default constructor
        Receiver( const string &name = "Receiver" );
        virtual string className() const ;

    protected:
        Model &initFunction();
        Model &externalFunction( const ExternalMessage & );
        Model &internalFunction( const InternalMessage & );
        Model &outputFunction( const InternalMessage & );

    private:
        const Port &in;
        Port &out;
        Port &outData;
        Time iPreparationTime;
        int iFileReceived;
    }; // class Receiver

// ** inline ** // 
inline string Receiver::className() const
    {
    return "Receiver" ;
    }

#endif   //__RECEIVER_H
