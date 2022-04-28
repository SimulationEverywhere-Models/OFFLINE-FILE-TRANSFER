[top]

components : fileserversender@FileServerSender

out : requestData ackReceived fileSent dataOut
in  : requestIn dataIn ackIn
 
Link : dataIn dataIn@fileserversender
Link : ackIn ackIn@fileserversender
Link : requestIn requestIn@fileserversender
Link : requestData@fileserversender requestData
Link : ackReceived@fileserversender ackReceived
Link : fileSent@fileserversender fileSent
Link : dataOut@fileserversender dataOut

[fileserversender]
preparation : 00:00:10:000
timeOut : 00:00:10:000