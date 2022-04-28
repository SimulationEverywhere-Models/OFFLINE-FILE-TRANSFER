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

/** include files **/
#include "tmpFileThread.h"
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include "filesystem.h" // simple file system

const int fileSystemEmpty = -2;
const int fileNotFound = -1;
const int readyForRequest = 0;

/************************* Class TmpFileWriteThread Implementation ***********************************/


/** public functions **/

/*******************************************************************
* Function Name: TmpFileWriteThread
* Description: 
********************************************************************/
TmpFileWriteThread::TmpFileWriteThread( const string &name )
: Atomic( name )
, storeData( addInputPort( "storeData" ) )
, dataOut(addOutputPort( "dataOut" ) )
, iCurFile( -1 )
, iWriting( false )
{
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &TmpFileWriteThread::initFunction()
{
    this-> passivate();
    return *this;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &TmpFileWriteThread::externalFunction( const ExternalMessage &msg )
{
    if( msg.port() == storeData &&  this->state() == passive  )
    {
        iCurFile = static_cast < int > ( msg.value() ) ;
        // File number must be larger than 0 to be considered as a valid file
        if( iCurFile > 0 && !find( tmpFile,iCurFile ) )
        {
        // No need to overwrite file that already exists
        tmpFile.push( iCurFile );
        iWriting = true;
        }
        holdIn( active, Time::Zero );
    }
    return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &TmpFileWriteThread::internalFunction( const InternalMessage & )
{

    iWriting = false;
    passivate();
    return *this;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &TmpFileWriteThread::outputFunction( const InternalMessage &msg )
{
    if( iWriting && iCurFile > 0 )
    {
    sendOutput( msg.time(), dataOut, iCurFile );
    }
    return *this;
}

/*******************************************************************
* Function Name: find
* Description: check whether file is already existed in file
*              system
********************************************************************/
bool TmpFileWriteThread::find( queue<int> tmpQueue, int value )
{
    // Performance is not so efficient comparing to use a reference of tmpFile.
    // Using a reference of tmpFile to search a particular file, you could pop
    // the front element and push it to the back of the queue. After a round
    // search, the queue is not changed.
    // However, should never try to use a reference of tmpFile here as find is
    // not an atomic operation and there are two working threads trying to visit
    // the same tmpFile. You might get a wrong order of the files  you are receiving
    // or even miss some files.
    int size = tmpQueue.size();
    bool find = false;
    for( int i = 0; i < size; i++ )
    {
        int tmpValue = tmpQueue.front();
        if( value == tmpValue )
        {
            find = true;
            break;
        }
        tmpQueue.pop();
    }
    return find;
}

/************************* Class TmpFileReadThread Implementation ***********************************/

/** public functions **/

/*******************************************************************
* Function Name: TmpFileReadThread
* Description:
********************************************************************/
TmpFileReadThread::TmpFileReadThread( const string &name )
: Atomic( name )
, deleteData( addInputPort( "deleteData" ) )
, requestData(addInputPort( "requestData") )
, dataOut(addOutputPort( "dataOut") )
,iCurFile( -1 )
, iReading( false )
, iDeleting( false )
{
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &TmpFileReadThread::initFunction()
{
    this-> passivate();
    return *this;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &TmpFileReadThread::externalFunction( const ExternalMessage &msg )
{
    if ( msg.port() == requestData && this->state() == passive )
        {
        if( !tmpFile.empty() )
            {
            iCurFile = tmpFile.front();
            }
            iReading = true;
            holdIn( active,Time::Zero );
        }
    else if( msg.port() == deleteData && this->state() == passive )
        {
        if( !tmpFile.empty() )
            {
            tmpFile.pop();
            iCurFile = tmpFile.front();
            }
        iDeleting =true;
        holdIn( active,Time::Zero );
    }
    return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &TmpFileReadThread::internalFunction( const InternalMessage & )
{
    iReading = false;
    iDeleting = false;
    passivate();
    return *this;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &TmpFileReadThread::outputFunction( const InternalMessage &msg )
{
    if( iReading )
        {
        if( !tmpFile.empty() )
            {
            // Send Normal file
            sendOutput( msg.time(), dataOut, iCurFile );
            }
        else
            {
            // No file found in the file system, usually means all files have been sent
            sendOutput( msg.time(), dataOut, fileNotFound );
            }
        }
    if( iDeleting )
        {
        // Ack, file is successfully deleted and there are more files in the file system
        if( !tmpFile.empty() )
            {
            sendOutput( msg.time(), dataOut, readyForRequest );
            }
        else
           {
            // File is deleted and no more file is left or delete operation fails as there
            // is no file in the file system
            sendOutput( msg.time(), dataOut, fileSystemEmpty );
           }
    }
    return *this;
}
