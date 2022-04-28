Version: 1.0

Author: Weiwei Li, Yuhan Zeng

All bat files could only be executed after the whole project is compiled.

Each bat file is used to test different atomic model or couple model of OfflineFileTransfer based on integration test stragegy.

Receiver.bat :
  This script is used to test atomic model of receiver. The input of receiver is expected to be a positive integer. And it will extract the file from the input. The last digit is 0 or 1 which is alternative bit used to judge whether the correct file has been delivered while the other digits are considered as the file being transferred.

tmpFileWriteThread.bat:
  This script is to test atomic model of tmpFileWriteThread. The input of tmpFileWriteThread is expected to be a positive integer. It will write this file into file system. However, if the file that tmpFileWriteThread receives has already exists in the file system. It will discard the file to gain a better performance.

Subnet.bat
  This script is to test atomic model of subnet. The input of subnet is expected to be a positive integer. Subnet will random delay the time of file being transferred. In addition, subnet only has 95% probability to successfully deliver the file which means the file might be lost in the subnet.

tmpFileReadThread.bat
  This script is to test atomic model of tmpFileReadThread. tmpFileReadThread has two inputs. The inputs are expected to be an integer. The input of requestData will trigger tmpFileReadThread to read the file from the file system while deleteData will trigger the deletion. tmpFileReadThread will return error code if there is no file in the file system. For reading operation, this error code will be -1 and for deleting operation, this error code will be -2.

fileServerSender.bat
  This script is to test atomic model of fileServerSender. fileServerSender is the most complicated atomic model in this simulator. It has two handshaking processes with tmpFileReadThread. fileServerSender has three inputs and four outputs. The input of dataIn is expected to be an integer greater than -3. The input of ackIn is expected to be an integer of 0 or 1. The input of requestIn is expected to be an integer. The output of dataOut is expected to a positive integer. The output of ackreceived is expected to an integer. The output of requestData is expected to a positive integer of 1. The output of fileSent is expected to a positive integer. fileServerSender will be activated once it received an requestIn request. It will then send a requestData to tmpFileReadThread. tmpFileReadThread will then send an answer to fileServerSender. If the received number is -1 which means no file exists in the file system, fileServerSender will finish the transfer and go to passive state. 
If this number is a positive integer, fileServerSender will send this number to UB via AccessNetworkB and wait for acknowledge from UB. When fileServerSender receives acknowledge from UB, it will send ackreceived to tmpFileReadThread to delete the file just transferred and wait for tmpFileReadThread¡¯s response. tmpFileReadThread will response fileServerSender¡¯s request and send its output to dataIn of fileServerSender with as following principles. If the file is deleted successfully and there are more files in the file system, it will send code 0 to fileServerSender. If file is deleted successfully and no more file exists or the file system has already been empty, then tmpFileReadThread will send -2 to fileServerSender. Once fileServerSender receive this response, it will have two choices: first, finish the whole transfer if the response is -2; second, send requestData to fileServerReadThread if the response is 0. This process will recursively executed until there is no file existing in the file system. 

Sender
¡¡This script is to test atomic model of Sender. Sender has two inputs and three outputs. The input of controlIn is expected to a positive integer. The input of ackIn is expected to a positive integer of 0 or 1. The Output of dataOut is expected to be a positive integer. The output of fileSent is expected to be a positive integer. The output of ackReceived is expected to be positive integer of 1. 

AccessNetwork.bat
  This script is to test couple model of AccessNetwork. The accessNetwork has two inputs and two outputs. All the inputs are expected to be a positive integer. All the outputs are expected to an integer of 0 or 1.
  The AccessNetwork is made up of two parts: subnetAToF and subnetFToA. The subnetAToF gets in1 from UA and sends out1 to fileServerReceiver. The subnetFToA gets in2 from the fileServerReceiver and sends out2 to UA. Therefore, there are two inputs and two outputs in this part. The last digit of the input is the alternate bit which is also what in2 should receive from the fileServerReceiver.  

fileserver.bat
 This script is to test couple model of filesExchangeServer. In this part, there are 3 inputs (inAToF, inBToF, requestIn) and 2 outputs (outFToA, outFToB). We put 3 different values (11, 20, 31) into the inAToF. The last digit of the value is alternate bit. Also, we put some alternate bits into inBToF. In order to test the model, we intentionally extend the time between each alternate bits to inBToF.

Network.bat
  In this part, there are three inputs (inAtoN, inBtoN, requestIn), and two outputs (outntoa, outntob). When there is an incoming requestIn, the Network begins to generate the outputs.
As fileExchangeServer module is a sub module of Network, thus the Network will keep sending outputs until it gets an acknowledgement.
 
offLineFileTransfer.bat
  This script is to test couple model of offLineFileTransfer. This is our final system test. The outLineFileTransfer is consisted of 3 parts. UA sends files to UB through Network and gets acknowledgement from the Network. To ensure the stability of the system, if one file lost, it will resent the file. In order to realize the offline transfer function, a requestIn port should be added to send the request to the fileExchangeServer to download the files sent by UA.
¡¡There are two inputs (controlIn, requestIn) and three outputs (fileSent, ackReceived, outData). When a controlIn comes, the UA starts transmit files to the server. When the requestIn comes, the UB starts to get files from the server. 

