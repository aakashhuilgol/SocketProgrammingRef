#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main()
{
	int network_socket,connection;
	char buff1[100],buff2[100];

	network_socket = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in server_address;
	bzero(&server_address,sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9000);
	server_address.sin_addr.s_addr = INADDR_ANY;

	connection = connect(network_socket,(struct sockaddr *)&server_address,sizeof(server_address));
	if(connection==-1)
		printf("Error in connection\n");

	while(1)
	{
		recv(network_socket,&buff1,sizeof(buff1),0);
		printf("\nServer: %s",buff1);
		printf("\nYou: ");
		gets(buff2);
		send(network_socket,&buff2,sizeof(buff2),0);
		
	}
	close(network_socket);
	return 0;
}
