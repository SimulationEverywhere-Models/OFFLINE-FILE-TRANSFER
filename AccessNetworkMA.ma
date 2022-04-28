[top]
components : subnet1@Subnet
components : subnet2@Subnet

out : in1 in2
out : out1 out2

Link : in1 in@subnet1
Link : out@subnet1 out1 
Link : in2 in@subnet2
Link : out@subnet2 out2 

[subnet1]
distribution : normal
mean : 3
deviation : 1

[subnet2]
distribution : normal
mean : 3
deviation : 1 

