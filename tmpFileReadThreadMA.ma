[top]
components : tmpfilereadthread@TmpFileReadThread 
out : dataOut 
in : deleteData requestData 
Link : deleteData deleteData@tmpfilereadthread
Link : requestData requestData@tmpfilereadthread
Link : dataOut@tmpfilereadthread dataOut

[tmpfilereadthread]


