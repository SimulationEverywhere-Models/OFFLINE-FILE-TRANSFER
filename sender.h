/*******************************************************************
 *
 *  DESCRIPTION: Atomic Model Sender
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

#ifndef __SENDER_H
#define __SENDER_H


#include "atomic.h"     // class Atomic

class Sender : public Atomic
    {
    public:
        //Default constructor
        Sender( const string &name = "Sender" );
        virtual string className() const;

    protected:
        Model &initFunction();
        Model &externalFunction( const ExternalMessage & );
        Model &internalFunction( const InternalMessage & );
        Model &outputFunction( const InternalMessage & );

    private:
        const Port &controlIn;
        const Port &ackIn;
        Port &dataOut;
        Port &fileSent;
        Port &ackReceived;
        Time iPreparationTime;
        Time iTimeout;
        bool iAck;
        int iFileNum;
        int iTotalFileNum;
        int iAbt;
        bool iSending;
    }; // class Sender

// ** inline ** // 
inline string Sender::className() const
    {
    return "Sender";
    }

#endif //__SENDER_H
