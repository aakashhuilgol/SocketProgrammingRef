#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#define MAX 80
#define PORT 4000
#define SA struct sockaddr
int main()
{
char buff[MAX],concat[MAX],copy[MAX];
int sockfd,len,n;
struct sockaddr_in servaddr;
sockfd=socket(AF_INET,SOCK_DGRAM,0);
if(sockfd==-1)
{
printf("socket creation failed...");
exit(0);
}
else
printf("Socket successfully created..");
bzero(&servaddr,sizeof(len));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
servaddr.sin_port=htons(PORT);
len=sizeof(servaddr);
for(;;)
{
bzero(buff,sizeof(buff));
printf("\nEnter string: ");
n=0;
while((buff[n++]=getchar())!='\n');
sendto(sockfd,buff,sizeof(buff),0,(SA *)&servaddr,len);
strcpy(copy,buff);
bzero(buff,sizeof(buff));
recvfrom(sockfd,buff,sizeof(buff),0,(SA *)&servaddr,&len);
strcat(copy,buff);
strcat(concat,copy);
printf("From Server: %s\n",concat);
if(strncmp("exit",buff,4)==0)
{
printf("Client Exit...");
break;
}
bzero(buff,sizeof(buff));
}
close(sockfd);
}
