/*******************************************************************
 *
 *  DESCRIPTION: Atomic Model Subnet
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
#include "subnet.h"      // class Subnet
#include "message.h"     // class ExternalMessage, InternalMessage
#include "distri.h"      // class Distribution
#include "mainsimu.h"    // class MainSimulator
#include "strutil.h"     // str2float( ... )
#include <stdlib.h>

/** public functions **/

/*******************************************************************
 * Function Name: Subnet
 * Description:
 ********************************************************************/
Subnet::Subnet( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )
, out( addOutputPort( "out" ) )
, iIndex( 1 )
    {
    iDist = Distribution::create( MainSimulator::Instance().getParameter( description(), "distribution" ) );
    if( iDist != NULL )
        {
        MASSERT( iDist );

        for ( register int i = 0 ; i < iDist->varCount() ; i++ )
            {
            string parameter( MainSimulator::Instance().getParameter( description(), iDist->getVar(i) ) ) ;
            iDist->setVar( i, str2float( parameter ) ) ;
            }
        }
    }

/*******************************************************************
 * Function Name: initFunction
 * Description: Resetea la lista
 * Precondition: El tiempo del proximo evento interno es Infinito
 ********************************************************************/
Model &Subnet::initFunction()
    {
    this-> passivate();
    return *this;
    }

/*******************************************************************
 * Function Name: externalFunction
 * Description:
 ********************************************************************/
Model &Subnet::externalFunction( const ExternalMessage &msg )
    {
    if( msg.port() == in )
        {
        iIndex ++;
        iPacket = static_cast <int>( msg.value() );
        holdIn( active, Time( static_cast<float>( fabs( distribution().get() ) ) ) );
        }
    return *this;
    }

/*******************************************************************
 * Function Name: internalFunction
 * Description:
 ********************************************************************/
Model &Subnet::internalFunction( const InternalMessage & )
    {
    passivate();
    return *this;
    }

/*******************************************************************
 * Function Name: outputFunction
 * Description:
 ********************************************************************/
Model &Subnet::outputFunction( const InternalMessage &msg )
    {
    if ( static_cast<double>( rand() ) / static_cast<double>( RAND_MAX )  < 0.95 )
        {
        sendOutput( msg.time(), out, iPacket );
        }
    return *this;
    }

Subnet::~Subnet()
    {
    delete iDist;
    }
