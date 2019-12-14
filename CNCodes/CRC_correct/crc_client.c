#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#define SERV_TCP_PORT 5035
int main(int argc,char * * argv)
{
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int i,j,k,flag=0,divis=0,divid;	
	char divisor[100],dividend[100],quotient[100],crc[100],temp[100];
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(SERV_TCP_PORT);
	connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	printf("Enter dividend:");
	scanf("%s",dividend);
	printf("Enter divisor:");
	scanf("%s",divisor);
	write(sockfd,dividend,100);
	write(sockfd,divisor,100);
	printf("\n");
	printf("\nServer result:");
	read(sockfd,quotient,100);
	read(sockfd,crc,100);
	printf("\n\nQuotient=%s",quotient);
	printf("\n\nCRC values=%s\n",crc);
	close(sockfd);
	return 0;
}
