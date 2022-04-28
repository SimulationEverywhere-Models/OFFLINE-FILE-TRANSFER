[top]
components : sender@Sender
out : dataOut
out : fileSent
out : ackReceived
in : controlIn
in : ackIn
Link : controlIn controlIn@sender
Link : ackIn ackIn@sender
Link : dataOut@sender dataOut
Link : fileSent@sender fileSent
Link : ackReceived@sender ackReceived

[sender]
preparation : 00:00:10:000
timeout : 00:00:20:000 
