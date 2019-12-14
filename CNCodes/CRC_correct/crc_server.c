#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#define SERV_TCP_PORT 5035
int main(int argc, char **argv)
{
	int sockfd,newsockfd,clength;
	struct sockaddr_in serv_addr,cli_addr;
	int i,j,k,flag=0,divis=0,divid;	
	char divisor[100],dividend[100],quotient[100],crc[100],temp[100];
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(SERV_TCP_PORT);
	printf("\n Binded...");
	bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	listen(sockfd,5);
	clength=sizeof(cli_addr);
	newsockfd=accept(sockfd,(struct sockaddr*)&cli_addr,&clength);
	read(newsockfd,dividend,100);
	read(newsockfd,divisor,100);
	printf("Calculating CRC...\n");
	for(i=0;i<strlen(divisor)-1;i++)				//Appending n-1 0s to dividend, n is the number of bits in divisor
	{
		strcat(dividend,"0");
	}
	for(j=0;dividend[j]!='\0';j++)					//Calculation of CRC Starts
	{
		if(flag==0){						//Check if it is the first round of calculation
			for(i=0;i<strlen(divisor);i++)
			{
				temp[i] = dividend[i];			//Take first n bits of dividend and store it in a temp string 
			}
			temp[i]='\0';
			flag=1;
			if(temp[0]=='1'){				
			for(k=0;k<strlen(divisor);k++)
			{
				if((temp[k]=='1'&&divisor[k]=='1')||(temp[k]=='0'&&divisor[k]=='0'))
					crc[k] = '0';
				else
					crc[k] = '1';
			}
			crc[k]='\0';
			strcat(quotient,"1");
			}
			else
				strcat(quotient,"0");
			j=i-1;
		}
		else
		{
			for(k=0;k<strlen(divisor)-1;k++)
			{
				crc[k] = crc[k+1];
			}
			crc[k] = dividend[j];
			crc[k+1]='\0';
			strcpy(temp,crc);
			if(temp[0]=='1'){
			for(k=0;k<strlen(divisor);k++)
			{
				if((temp[k]=='1'&&divisor[k]=='1')||(temp[k]=='0'&&divisor[k]=='0'))
					crc[k] = '0';
				else
					crc[k] = '1';
			}
			crc[k]='\0';
			strcat(quotient,"1");
			}
			else
				strcat(quotient,"0");
		}
	}	
	for(k=0;k<strlen(divisor)-1;k++)
		{
				crc[k] = crc[k+1];
		}
		crc[k] = '\0';
	printf("Quotient: %s\n",quotient);
	printf("CRC :%s\n",crc);
	write(newsockfd,quotient,100);
	write(newsockfd,crc,100);
	close(sockfd);
	return 0;
}
