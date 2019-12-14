#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

unsigned short ck_sum(unsigned short *addr,int count)
{
	register long sum=0;
	while(count>1)
	{
		sum= sum + *addr++;
		count= count-2;
	}
	if(count>0)
	{
		sum+=*addr++;
	}
	
	while(sum>>16)
	{
		sum = (sum & 0xFFFF) + (sum >>16);
	}
	
	return sum;
}

int main()
{
	int network_socket,connection;
	char buff1[100],buff2[100];

	network_socket = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in server_address;
	bzero(&server_address,sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(22002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	connection = connect(network_socket,(struct sockaddr *)&server_address,sizeof(server_address));
	if(connection==-1)
		printf("Error in connection\n");

	
	int count;
	unsigned short a,sum_client,cli_checksum;
	unsigned long int *p;
	char value[100];
	p=&a;
	
	
		printf("Enter the data: ");
		scanf("%X",&a);

		printf("Enter count: ");
		scanf("%d",&count);

		sprintf(value,"%X",(*p));
		write(network_socket,value,16);
		sprintf(value,"%X",count);
		write(network_socket,value,16);

		sum_client = ck_sum(&a,count);
		
		cli_checksum = (~sum_client);
		
		sprintf(value,"%X",(cli_checksum));
		write(network_socket,value,16);

		printf("\nSum @ client: %X:",sum_client);
		printf("\nChecksum @ client: %X:",cli_checksum);

		//read(network_socket,&buff1,sizeof(buff1));
		//printf("ACK received %s",buff1);
		
	
	close(network_socket);
	return 0;
}
