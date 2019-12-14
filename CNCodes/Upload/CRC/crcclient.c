
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#define SERV_TCP_PORT 10000
int main(int argc,char * * argv)
{
       int sockfd;
       struct sockaddr_in serv_addr;
       struct hostent *server;
       char input[100],key[30],temp[30],quot[100],rem[30],key1[30];
       sockfd=socket(AF_INET,SOCK_STREAM,0);
       serv_addr.sin_family=AF_INET;
       serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
       serv_addr.sin_port=htons(SERV_TCP_PORT);
       connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
       printf("\nEnter the dividend:");
       scanf("%s",input);
       printf("\nEnter the divisor:");
       scanf("%s",key);
       write(sockfd,input,30);
       write(sockfd,key,30);
       printf("\n");
       printf("\nServer result:");
       read(sockfd,quot,30);
       read(sockfd,rem,30);
       read(sockfd,input,100);
       printf("\n\nQuotient=%s",quot);
       printf("\n\nRemainder=%s",rem);
  
       close(sockfd);
       return 0;
}
