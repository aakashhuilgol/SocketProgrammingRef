#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main()
{
	int network_socket,binding,client_socket;
	char buff1[100],buff2[100];

	network_socket = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in server_address;
	bzero(&server_address,sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(18011);
	server_address.sin_addr.s_addr = INADDR_ANY;

	binding = bind(network_socket,(struct sockaddr *)&server_address,sizeof(server_address));
	if(binding==-1)
		printf("Error in binding\n");

	listen(network_socket,5);

	client_socket = accept(network_socket,NULL,NULL);
	if(client_socket == -1)
		printf("Error in accept");

	
	int i=0,j=0,k=0,msg_len,key_len,flag=0;	
	char input[30],key[30],key1[30],temp[30],q[30],rem[30];

	read(client_socket,input,sizeof(input));
	read(client_socket,key,sizeof(key));
	printf("Divisor: %s Dividend: %s",input,key);

	strcpy(key1,key);
	msg_len = strlen(input);
	key_len = strlen(key);
	printf("msg_len: %d,key_len: %d",msg_len,key_len);
	
	for(i=0;i<key_len-1;i++)
	{
		input[msg_len+i] = '0';
	}

	for(i=0;i<key_len;i++)
		temp[i] = input[i];

	for(i=0;i<msg_len;i++)
	{
		q[i] = temp[0];
		if(q[i] == '0')
			for(j=0;j<key_len;j++)
				key[j] = '0';
		else
			for(j=0;j<key_len;j++)
				key[j] = key1[j];

		for(j=key_len-1;j>0;j--)
		{
			if(temp[j] == key[j])
				rem[j-1] = '0';
			else
				rem[j-1] = '1';
		}
		rem[key_len-1]=input[i+key_len];
		strcpy(temp,rem);
	}
	strcpy(rem,temp);
	
	printf("\nQuotient: ");
	for(i=0;i<strlen(q);i++)
		printf("%c",q[i]);
	
	printf("\nRemainder: ");
	for(i=0;i<strlen(rem);i++)
	{		
		printf("%c",rem[i]);
	}
	
	write(client_socket,rem,sizeof(rem));
	close(network_socket);
	close(client_socket);
	return 0;
}
