/*******************************************************************
 *
 *  DESCRIPTION: Atomic Model FileServer Sender
 *
 *   AUTHOR: Weiwei Li
 *
 *  EMAIL: mailto://wli086@uottawa.ca
 *
 *  DATE: 13/10/2012
 *
 *******************************************************************/

#ifndef __FILESERVERSENDER_H
#define __FILESERVERSENDER_H


#include "atomic.h"     // class Atomic

class FileServerSender : public Atomic
    {
    public:
        //Default constructor
        FileServerSender( const string &name = "FileServerSender" );
        virtual string className() const;

    protected:
        Model &initFunction();
        Model &externalFunction( const ExternalMessage & );
        Model &internalFunction( const InternalMessage & );
        Model &outputFunction( const InternalMessage & );

    private:
        const Port &requestIn;
        const Port &ackIn;
        const Port &dataIn;
        Port &dataOut;
        Port &fileSent;
        Port &ackReceived;
        Port &requestData;
        Time iPreparationTime;
        Time iTimeOut;
        bool iAck;
        int  iCurFile;
        int iAbt;
        bool iSending;
        bool iRequestData;
        bool iDone;
    };	// class FileServerSender

// ** inline ** // 
inline string FileServerSender::className() const
    {
    return "FileServerSender";
    }

#endif   //__FILESERVERSENDER_H
