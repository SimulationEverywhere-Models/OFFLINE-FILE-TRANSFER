[top]
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


