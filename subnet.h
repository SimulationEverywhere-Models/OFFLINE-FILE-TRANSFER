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

#ifndef __SUBNET_H
#define __SUBNET_H

#include "atomic.h"     // class Atomic

/** forward declarations **/
class Distribution;

class Subnet : public Atomic
    {
    public:
        //Default constructor
        Subnet( const string &name = "Subnet" );
        ~Subnet();
        virtual string className() const;

    protected:
        Model &initFunction();
        Model &externalFunction( const ExternalMessage & );
        Model &internalFunction( const InternalMessage & );
        Model &outputFunction( const InternalMessage & );

    private:
        Distribution &distribution()
            {
            return *iDist;
            }

    private:
        const Port &in;
        Port &out;
        int iPacket;
        int iIndex;
        Distribution * iDist;
    }; // class Subnet

// ** inline ** // 
inline string Subnet::className() const
    {
    return "Subnet";
    }

#endif //__SUBNET_H
