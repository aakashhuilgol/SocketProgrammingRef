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

int toDec(char hex[20])
{
	int num;	
	sscanf(hex,"%x",&num);
	return num;
}

unsigned short ck_sum(unsigned short *addr,int count)
{
	register long sum=0;	
	while(count>1)
	{
		sum+=*addr++;
		count-=2;
	}
	
	if(count>0)
	{
		sum+=*addr;
	}

	while(sum>>16)
	{
		sum = (sum & 0xFFFF) + (sum>>16);
	}
	return sum;
}

unsigned short final_sum(unsigned short sum,unsigned short checksum)
{
	register long sum1 =0;
	sum1 = sum + checksum;
	
	while(sum1>>16)
	{
		sum1 = (sum1 & 0xFFFF) + (sum1>>16); 
	}
	return sum1;
}

int main()
{
	int network_socket,binding,client_socket;
	char buff1[100],buff2[100];

	network_socket = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in server_address;
	bzero(&server_address,sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(22002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	binding = bind(network_socket,(struct sockaddr *)&server_address,sizeof(server_address));
	if(binding==-1)
		printf("Error in binding\n");

	listen(network_socket,5);
	printf("Before while");

	while(1){

	client_socket = accept(network_socket,NULL,NULL);
	if(client_socket == -1)
		printf("Error in accept");

	printf("After accept");

	
	 long int data;
	unsigned short sum_server,serv_checksum,summ,Final_sum,cli_socket;
	int count;
	
	printf("Before read");
		read(client_socket,buff1,sizeof(buff1));
		data = toDec(buff1);
	printf("after read");

		read(client_socket,buff2,sizeof(buff2));
		count = toDec(buff2);

		printf("Data received from client : %X",data);
		printf("Data received from client : %d",count);
		
		sum_server = ck_sum(&data,count);

		printf("Sum @ server: %X",sum_server);

		read(client_socket,summ,16);
		cli_socket = toDec(summ);
		
		Final_sum = final_sum(sum_server,cli_socket);

		serv_checksum = (~Final_sum);
		if(serv_checksum == 0x0000)
			printf("no error");
		else
			printf("error in data");
		
	close(client_socket);

	}	
	


	close(network_socket);
	
	return 0;
}
