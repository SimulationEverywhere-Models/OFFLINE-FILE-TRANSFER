/*******************************************************************
 *
 *  DESCRIPTION: Atomic Model Receiver
 *
 *  ORIGINAL AUTHOR: Tao Zheng
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
 *
 *******************************************************************/

/** include files **/
#include "receiver.h"   // class Receiver
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

const int offset = 10;  // offset to encapsulate data

/** public functions **/

/*******************************************************************
 * Function Name: Receiver
 * Description:
 ********************************************************************/
Receiver::Receiver( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )
, out( addOutputPort( "out" ) )
, outData( addOutputPort( "outData" ) )
, iPreparationTime( 0, 0, 10, 0 )
, iFileReceived( 0 )
    {
    string preparationTime( MainSimulator::Instance().getParameter( description(), "preparation" ) );

    if( preparationTime != "" )
        {
        iPreparationTime = preparationTime;
        }
    }

/*******************************************************************
 * Function Name: initFunction
 * Description: Resetea la lista
 * Precondition: El tiempo del proximo evento interno es Infinito
 ********************************************************************/
Model &Receiver::initFunction()
    {
    this-> passivate();
    return *this;
    }

/*******************************************************************
 * Function Name: externalFunction
 * Description:
 ********************************************************************/
Model &Receiver::externalFunction( const ExternalMessage &msg )
    {
    if( msg.port() == in )
        {
        iFileReceived = static_cast <int> ( msg.value() );
        holdIn( active, iPreparationTime );
        }

    return *this;
    }

/*******************************************************************
 * Function Name: internalFunction
 * Description:
 ********************************************************************/
Model &Receiver::internalFunction( const InternalMessage & )
    {
    passivate();
    return *this;
    }

/*******************************************************************
 * Function Name: outputFunction
 * Description:
 ********************************************************************/
Model &Receiver::outputFunction( const InternalMessage &msg )
    {
    sendOutput( msg.time(), out, iFileReceived % offset );
    sendOutput( msg.time(), outData, iFileReceived / offset );
    return *this;
    }
