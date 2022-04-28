/*******************************************************************
 *
 *  DESCRIPTION: Atomic Model FileServerSender
 *
 *  AUTHOR: Weiwei Li
 *
 *  EMAIL: mailto://wli06@uottawa.ca
 *
 *  DATE: 11/10/2012
 *
 *******************************************************************/

/** include files **/
#include "fileServerSender.h"  // class FileServerSender
#include "message.h"           // class ExternalMessage, InternalMessage
#include "mainsimu.h"          // MainSimulator::Instance().getParameter( ... )

const int fileSuccessSent = 1;
const int requestMoreFile = 1;
const int parityCheck = 2;     // Use to generate parity check
const int offset = 10;         // Use to encapsulate data

/** public functions **/

/*******************************************************************
 * Function Name: FileServerSender
 * Description:
 ********************************************************************/
FileServerSender::FileServerSender( const string &name )
: Atomic( name )
, requestIn( addInputPort( "requestIn" ) )
, ackIn( addInputPort( "ackIn" ) )
, dataIn( addInputPort( "dataIn" ) )
, dataOut( addOutputPort( "dataOut" ) )
, fileSent( addOutputPort( "fileSent" ) )
, ackReceived( addOutputPort( "ackReceived" ) )
, requestData( addOutputPort( "requestData" ) )
, iPreparationTime( 0, 0, 10, 0 )
, iTimeOut( 0, 0, 20, 0 )
, iAbt( 0 )
, iRequestData( false )
, iDone( true )
    {
    string preparationTime( MainSimulator::Instance().getParameter( description(), "preparation" ) );

    if( preparationTime != "" )
        {
        iPreparationTime = preparationTime;
        }

    string timeOut( MainSimulator::Instance().getParameter( description(), "timeout" ) );

    if( timeOut != "" )
        {
        iTimeOut = timeOut;
        }
    }

/*******************************************************************
 * Function Name: initFunction
 * Description: Resetea la lista
 * Precondition: El tiempo del proximo evento interno es Infinito
 ********************************************************************/
Model &FileServerSender::initFunction()
    {
    this-> passivate();
    return *this;
    }

/*******************************************************************
 * Function Name: externalFunction
 * Description:
 ********************************************************************/
Model &FileServerSender::externalFunction( const ExternalMessage &msg )
    {
    if( msg.port() == dataIn )
        {
        int tmpFile= static_cast <int> ( msg.value() );
        if ( tmpFile > 0 )
            {
            // File is valid and could be sent
            iCurFile = tmpFile;
            iAck = false;
            iSending = true;
            iRequestData = false;
            iAbt = ( iAbt + 1 ) % parityCheck;	//set alternative bit
            holdIn( active, iPreparationTime );
            }
        else if ( tmpFile == 0 )
            {
            // Indicates more files are ready to be sent
            iRequestData = true;
            holdIn( active, Time::Zero );
            }
        else
            {
            // No file available or try to delete a file that does not exist
            iRequestData = false;
            iSending = false;
            iAck = false;
            // Transfer is Done
            // In some cases, the speed of access network of UB might be faster than that of UA, it is
            // possible that UB has downloaded all the files in the temporary file server while UA is still uploading
            // files. We will still stop in this case as it is up to UB to decide whether to download
            // the files but not file server.
            iDone = true;
            holdIn( active, Time::Zero );
            }
        }
    if( msg.port() == ackIn  && this->state() == active )
        {
        if ( iAbt == static_cast < int > ( msg.value() ) )
            {
            iAck = true;
            iSending = false;
            holdIn( active, Time::Zero );
            }
        }
    if( iDone && msg.port() == requestIn && this->state() == passive )
        {
        // Ignore request for data if data is still transferring
        iRequestData = true;
        iDone = false;
        holdIn( active, Time::Zero );
        }

    return *this;
    }

/*******************************************************************
 * Function Name: internalFunction
 * Description:
 ********************************************************************/
Model &FileServerSender::internalFunction( const InternalMessage & )
    {
    if( iDone )
        {
        // Go to idle state as all files have been successfully delivered.
        passivate();
        }
    else
        {
        if ( iAck )
            {
            // Send ack to file server and wait the file server to send ready  notification.
            // If file server fails to response, transmission will be canceled.
            // This is very common and for instance, the service is temporary down. The only solution for this is
            // that UB send another request.
            // However, in this simulation, we did not simulate this behavior. File server will always be expected to
            // response, logic simulating this behavior might be implemented in the next version.
            iAck = false;
            passivate();
            }
        else
            {
            if ( iSending )
                {
                iSending = false;
                holdIn( active, iTimeOut );
                }
            else
                {
                if( iRequestData )
                    {
                    // Set timeout for data request, file server should response within the timeout
                    // Otherwise, another request will be sent.
                    holdIn( active, iTimeOut );
                    }
                else
                    {
                    iSending = true;
                    holdIn( active, iPreparationTime );
                    }
                }
            }
        }
    return *this;
    }

/*******************************************************************
 * Function Name: outputFunction
 * Description:
 ********************************************************************/
Model &FileServerSender::outputFunction( const InternalMessage &msg )
    {
    if ( iSending )
        {
        // Send file to UB
        sendOutput( msg.time(), dataOut, iCurFile * offset + static_cast<int>( iAbt ) ) ;
        sendOutput(	msg.time(), fileSent, iCurFile ) ;
        }
    else
        {
        if ( iAck )
            {
            // Send ack to file server to notify file has already been successfully delivered.
            // File Server could thus delete this temporary file.
            sendOutput( msg.time(), ackReceived, fileSuccessSent );
            }
        if( iRequestData )
            {
            // Request data from file server
            sendOutput( msg.time(), requestData, requestMoreFile );
            }
        }
    return *this;
    }
