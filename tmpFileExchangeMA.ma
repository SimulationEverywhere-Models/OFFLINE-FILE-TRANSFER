[top]
components : tmpfilereadthread@TmpFileReadThread
components : tmpfilewritethread@TmpFileWriteThread

out : readDataOut
in : deleteData requestData
in : storeData 

Link : storeData storeData@tmpfilewritethread
Link : deleteData deleteData@tmpfilereadthread
Link : requestData requestData@tmpfilereadthread
Link : dataOut@tmpfilereadthread readDataOut

[tmpfilewritethread]


[tmpfilereadthread]
