/*******************************************************************
 *
 *  DESCRIPTION: Simulator::registerNewAtomics()
 *
 *  ORIGNAL AUTHOR: Rujun Hu*
 *
 *  STUDENT#: 258902
 *
 *  EMAIL: rujun_hu@yahoo.ca
 *
 *  DATE: 16/10/2001
 *
 *  Reused by author: weiwei Li
 *
 *  Email: wli086@uottawa.ca
 *
 *  Data : 08/10/2012
 *
 *******************************************************************/

#include "modeladm.h" 
#include "mainsimu.h"
#include "receiver.h"         // class Receiver
#include "sender.h"           // class Sender
#include "subnet.h"           // class Subnet
#include "tmpFileThread.h"    // class TmpFileWriteThread  class TmpFileReadThread
#include "fileServerSender.h" // class FileServerSender
void MainSimulator::registerNewAtomics()
    {
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Subnet>(), "Subnet" ) ;
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Sender>(), "Sender" ) ;
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Receiver>(), "Receiver" ) ;
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<TmpFileWriteThread>(), "TmpFileWriteThread" ) ;
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<TmpFileReadThread>(), "TmpFileReadThread" ) ;
    SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<FileServerSender>(), "FileServerSender" ) ;
    }

