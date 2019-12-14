
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#define SERV_TCP_PORT 10000

int main(int argc, char **argv)
{
int sockfd,newsockfd,clength;
struct sockaddr_in serv_addr,cli_addr;
char input[100],key[30],temp[30],quot[100],rem[30],key1[30];
int i,j,keylen,msglen;
sockfd=socket(AF_INET,SOCK_STREAM,0);
serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=INADDR_ANY;
serv_addr.sin_port=htons(SERV_TCP_PORT);
printf("\n Binded...");
bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
listen(sockfd,5);
clength=sizeof(cli_addr);
newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clength);
read(newsockfd,input,30);
read(newsockfd,key,30);
keylen=strlen(key);
msglen=strlen(input);
strcpy(key1,key);
printf("\n Dividend:%s",input);
printf("\n Divisor:%s",key);

for(i=0;i<keylen-1;i++)
	{
	input[msglen+i]='0';
	}			
for(i=0;i<keylen;i++)
	temp[i]=input[i];
for(i=0;i<msglen;i++)			
	{
	quot[i]=temp[0];
	if(quot[i]=='0')
		for(j=0;j<keylen;j++)
			key[j]='0';else
		for(j=0;j<keylen;j++)
			key[j]=key1[j];
	      for(j=keylen-1;j>0;j--)
			{
			if(temp[j]==key[j])
				rem[j-1]='0';else
				rem[j-1]='1';	
			}
			rem[keylen-1]=input[i+keylen];
			strcpy(temp,rem);
	}
	strcpy(rem,temp);
	printf("\n Quotient is ");
	for(i=0;i<msglen;i++)
		printf("%c",quot[i]);
	printf("\n Remainder is ");
	for(i=0;i<keylen;i++)
		printf("%c",rem[i]);
	printf("\n Final data is ");
	for(i=0;i<msglen;i++)	
		printf("%c",input[i]);
	for(i=0;i<keylen-1;i++)
		printf("%c",rem[i]);


write(newsockfd,quot,30);
write(newsockfd,rem,30);
write(newsockfd,input,100);
close(sockfd);
return 0;
}
