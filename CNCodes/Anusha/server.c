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
	server_address.sin_port = htons(9000);
	server_address.sin_addr.s_addr = INADDR_ANY;

	binding = bind(network_socket,(struct sockaddr *)&server_address,sizeof(server_address));
	if(binding==-1)
		printf("Error in binding\n");

	listen(network_socket,5);

	client_socket = accept(network_socket,NULL,NULL);
	if(client_socket == -1)
		printf("Error in accept");

	while(1)
	{	
		printf("\nYou: ");
		gets(buff1);
		send(client_socket,buff1,sizeof(buff1),0);
		recv(client_socket,&buff2,sizeof(buff2),0);
		printf("\nServer:%s",buff2);
	}
	close(network_socket);
	close(client_socket);
	return 0;
}
