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

/** include files **/
#include "sender.h"     // class Sender
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

const int offset = 10;  // offset to encapsulate data
const int parityCheck = 2;

/** public functions **/

/*******************************************************************
 * Function Name: Sender
 * Description:
 ********************************************************************/
Sender::Sender( const string &name )
: Atomic( name )
, controlIn( addInputPort( "controlIn" ) )
, ackIn( addInputPort( "ackIn" ) )
, dataOut( addOutputPort( "dataOut" ) )
, fileSent( addOutputPort( "fileSent" ) )
, ackReceived( addOutputPort( "ackReceived" ) )
, iPreparationTime( 0, 0, 10, 0 )
, iTimeout( 0, 0, 20, 0 )
, iAbt( 0 )
    {
    string preparationTime( MainSimulator::Instance().getParameter( description(), "preparation" ) );

    if( preparationTime != "" )
        {
        iPreparationTime = preparationTime;
        }

    string timeout( MainSimulator::Instance().getParameter( description(), "timeout" ) );

    if( timeout != "" )
        {
        iTimeout = timeout;
        }
    }

/*******************************************************************
 * Function Name: initFunction
 * Description: Resetea la lista
 * Precondition: El tiempo del proximo evento interno es Infinito
 ********************************************************************/
Model &Sender::initFunction()
    {
    this-> passivate();
    return *this ;
    }

/*******************************************************************
 * Function Name: externalFunction
 * Description:
 ********************************************************************/
Model &Sender::externalFunction( const ExternalMessage &msg )
    {
    if( msg.port() == controlIn  && this->state() == passive )
        {
        iTotalFileNum = static_cast <int>( msg.value() );
        if ( iTotalFileNum > 0 )
            {
            iFileNum = 1;
            iAck = false;
            iSending = true;
            // Set alternative bit
            iAbt = ( iAbt + 1 ) % parityCheck;
            holdIn( active, iPreparationTime );
            }
        }
    if( msg.port() == ackIn  && this->state() == active )
        {
        if ( iAbt == static_cast <int>( msg.value() ) )
            {
            iAck = true;
            iSending = false;
            holdIn( active, Time::Zero ) ;
            }
        }
    return *this;
    }

/*******************************************************************
 * Function Name: internalFunction
 * Description:
 ********************************************************************/
Model &Sender::internalFunction( const InternalMessage & )
    {
    if( iAck )
        {
        if( iFileNum < iTotalFileNum )
            {
            iFileNum ++;
            iAck = false;
            iAbt = ( iAbt + 1 ) % parityCheck;
            iSending = true;
            holdIn( active, iPreparationTime );
            }
        else
            {
            iAck = false;
            iSending = false;
            passivate();
            }
        }
    else
        {
        if( iSending )
            {
            iSending = false;
            holdIn( active, iTimeout );
            }
        else
            {
            iSending = true;
            holdIn( active, iPreparationTime );
            }
        }
    return *this;
    }

/*******************************************************************
 * Function Name: outputFunction
 * Description:
 ********************************************************************/
Model &Sender::outputFunction( const InternalMessage &msg )
    {
    if( iSending )
        {
        sendOutput( msg.time(), dataOut, iFileNum * offset + iAbt ) ;
        sendOutput(	msg.time(), fileSent, iFileNum ) ;
        }
    else
        {
        if ( iAck )
            {
            sendOutput( msg.time(), ackReceived, iAbt ) ;
            }
        }
    return *this;
    }
