#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
int main(){
int network_socket;
char buf[100];
char buf1[100];
network_socket=socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in server_address;
server_address.sin_family=AF_INET;
server_address.sin_port=htons(9014);
server_address.sin_addr.s_addr=INADDR_ANY;
int binding;
binding=bind(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));
if(binding==-1){
printf("error in binding");
}
listen(network_socket,5);
int client_socket;
client_socket=accept(network_socket,NULL,NULL);
/*while(1)
{
printf("\nYou:");
gets(buf);
send(client_socket,buf,sizeof(buf),0);
printf("Client:");
recv(client_socket,buf1,sizeof(buf1),0);
printf("%s",buf1);
}*/

//hamming code
int data[10];
printf("Please input 4 bits of data\n");
scanf("%d",&data[0]);
scanf("%d",&data[1]);
scanf("%d",&data[2]);
scanf("%d",&data[4]);

//calculation of encoded data to clients
data[6] = data[4]^data[2]^data[0];
data[5] = data[4]^data[1]^data[0];
data[3] = data[0]^data[2]^data[1];

//sending the encoded data to clients
int n;
for(int i=0;i<7;i++)
{
	n = write(client_socket,&data[i],sizeof(int)); 
	if(n<0)
		printf("Error while transmitting the data");
}

printf("\nThe data sent is: ");
for(int i=0;i<7;i++)
	printf("%d",data[i]);

close(client_socket);
close(network_socket);
return 0;
}
