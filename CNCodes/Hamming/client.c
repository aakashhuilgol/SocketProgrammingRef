#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

int main(){
int network_socket;
network_socket=socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in server_address;
server_address.sin_family=AF_INET;
server_address.sin_port=htons(9014);
server_address.sin_addr.s_addr=INADDR_ANY;

int connection;
connection=connect(network_socket, (struct sockaddr *) &server_address,sizeof(server_address));
if(connection==-1)
{
printf("error in connect function\n");
exit(0);

}
char buf[100];
char buf1[100];
/*while(1){

recv(network_socket,&buf,sizeof(buf),0);
printf("Server:%s\n",buf);
printf("You:");
gets(buf1);
send(network_socket,buf1,sizeof(buf),0);
}*/

//hamming code
int data[10];
int test[10];
int p,p1,p2,p4,n;

printf("The data received is: ");
for(int i=0;i<7;i++)
{
	n =read(network_socket,&data[i],sizeof(int));
	if(n<0)
		printf("Error on receiving data	");
		printf("%d",data[i]);
}

printf("\nPlease enter the data to be tested");
for(int i=0;i<7;i++)
	scanf("%d",&test[i]);

p1 = test[6]^test[4]^test[2]^test[0];
p2 = test[5]^test[4]^test[1]^test[0];
p4 = test[3]^test[0]^test[2]^test[1];

p = (4*p4) + (2*p2) + p1;

printf("\nThe data for testing is:");
for(int i=0;i<7;i++)
	printf("%d",test[i]);

if(p == 0)
{
	printf("No error in data");
}
else
{
	printf("\nThe error is at position %d",p);
	printf("\nThe correct data is:");
	if(test[7-p] ==0)
		test[7-p]=1;
	else
		test[7-p]=0;
	for(int i=0;i<7;i++)
		printf("%d",test[i]);
	
}


close(network_socket);
return 0;
}

