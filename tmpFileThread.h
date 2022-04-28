/*******************************************************************
 *
 *  DESCRIPTION: Atomic Model TmpFileWriteThread & TmpFileReadThread
 *
 *  AUTHOR: Weiwei Li
 *
 *  EMAIL: mailto://wli086@uottawa.ca
 *
 *  DATE: 08/10/2012
 *
 *******************************************************************/

#ifndef __TMPFILETHREAD_H
#define __TMPFILETHREAD_H

#include "atomic.h"    // class Atomic
#include "filesystem.h"

class TmpFileWriteThread : public Atomic
    {
    public:
        // Default constructor
        TmpFileWriteThread( const string &name = "TmpFileWriteThread" );
        virtual string className() const ;

    protected:
        Model &initFunction();
        Model &externalFunction( const ExternalMessage & );
        Model &internalFunction( const InternalMessage & );
        Model &outputFunction( const InternalMessage & );

    private:
        // File matching function
        bool find( queue<int> tmpQueue, int value );

    private:
        const Port &storeData;
        Port &dataOut;
        int iCurFile;
        bool iWriting;
    }; // class TmpFileWriteThread

// ** inline ** //
inline string TmpFileWriteThread::className() const
    {
    return "TmpFileWriteThread";
    }

class TmpFileReadThread : public Atomic
    {
        public:
            // Default constructor
            TmpFileReadThread( const string &name = "TmpFileReadThread" );
            virtual string className() const ;

        protected:
            Model &initFunction();
            Model &externalFunction( const ExternalMessage & );
            Model &internalFunction( const InternalMessage & );
            Model &outputFunction( const InternalMessage & );

        private:
            const Port &deleteData;
            const Port &requestData;
            Port &dataOut;
            int iCurFile;
            bool iReading;
            bool iDeleting;
    }; // class TmpFileReadThread

// ** inline ** //
inline string TmpFileReadThread::className() const
    {
    return "TmpFileReadThread" ;
    }
#endif //__TMPFILETHREAD_H
