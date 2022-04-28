[top]
components : UA@Sender
components : Network
components : UB@Receiver

out : fileSent ackReceived outData
in : controlIn requestIn

Link : controlIn controlIn@UA
Link : dataOut@UA inAToN@Network
Link : outNToB@Network in@UB
Link : out@UB inBToN@Network 
Link : outNToA@Network ackIn@UA 
Link : fileSent@UA fileSent
Link : ackReceived@UA ackReceived
Link : outData@UB outData
Link : requestIn requestIn@Network


[UA]
preparation : 00:00:10:000
timeout : 00:00:20:000 

[UB]
preparation : 00:00:10:000 

[Network]
components : AccessNetWorkA
components : AccessNetWorkB
components : FileExchangeServer

in : inAtoN inBToN requestIn
out : outNToA outNToB

Link : inAtoN inAtoS@AccessNetWorkA
Link : outSToA@AccessNetWorkA outNToA 
Link : inBToN inBToS@AccessNetWorkB
Link : outSToB@AccessNetWorkB outNToB
Link : outSToF@AccessNetWorkA inAToF@FileExchangeServer
Link : outFToA@FileExchangeServer inFToS@AccessNetWorkA
Link : outSToF@AccessNetWorkB inBToF@FileExchangeServer
Link : outFToB@FileExchangeServer inFToS@AccessNetWorkB
Link : requestIn requestIn@FileExchangeServer


[FileExchangeServer]
components : tmpfilereadthread@TmpFileReadThread
components : tmpfilewritethread@TmpFileWriteThread
components : fileserverreceiver@Receiver
components : fileserversender@FileServerSender

in : inAToF inBToF requestIn
out : outFToA outFToB

Link : inAToF in@fileserverreceiver
Link : out@fileserverreceiver outFToA
Link : outData@fileserverreceiver storeData@tmpfilewritethread
Link : requestIn requestIn@fileserversender
Link : ackReceived@fileserversender deleteData@tmpfilereadthread
Link : dataOut@tmpfilereadthread dataIn@fileserversender
Link : requestData@fileserversender requestData@tmpfilereadthread
Link : dataOut@fileserversender  outFToB
Link : inBToF ackIn@fileserversender

[tmpfilewritethread]


[tmpfilereadthread]

[fileserverreceiver]
preparation : 00:00:10:000

[fileserversender]
preparation : 00:00:10:000
timeOut : 00:00:20:000

[AccessNetWorkA]
components : subnetAToF@Subnet
components : subnetFToA@Subnet

in : inAToS inFToS
out : outSToA outSToF

Link : inAToS in@subnetAToF
Link : out@subnetAToF outSToF
Link : inFToS in@subnetFToA
Link : out@subnetFToA outSToA


[subnetAToF]
distribution : normal
mean : 3
deviation : 1

[subnetFToA]
distribution : normal
mean : 3
deviation : 1 

[AccessNetWorkB]
components : subnetBToF@Subnet
components : subnetFToB@Subnet

in : inBToS inFToS
out : outSToB outSToF

Link : inBToS in@subnetBToF
Link : out@subnetBToF outSToF
Link : inFToS in@subnetFToB
Link : out@subnetFToB outSToB


[subnetBToF]
distribution : normal
mean : 3
deviation : 1

[subnetFToB]
distribution : normal
mean : 3
deviation : 1 


