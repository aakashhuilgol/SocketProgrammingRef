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
	server_address.sin_port = htons(18011);
	server_address.sin_addr.s_addr = INADDR_ANY;

	connection = connect(network_socket,(struct sockaddr *)&server_address,sizeof(server_address));
	if(connection==-1)
		printf("Error in connection\n");


	char input[30],divisor[30],rem[30],temp[30],q[30],key[30],flag=0;	
	int i=0,j=0,k=0,msg_len,rem_len,key_len;

	printf("Enter dividend: ");
	scanf("%s",input);
	printf("Enter divisor: ");
	scanf("%s",divisor);

	write(network_socket,input,sizeof(input));
	write(network_socket,divisor,sizeof(divisor));

	read(network_socket,rem,sizeof(rem));

	printf("Quotient: ");
	for(i=0;i<strlen(rem);i++)
		printf("%c",rem[i]);
	
	rem_len = strlen(rem);
	msg_len = strlen(input);
	key_len = strlen(divisor);
	strcpy(key,divisor);

	for(i=0;i<rem_len;i++)
		input[msg_len+i] = rem[i];

	for(i=0;i<key_len;i++)
		temp[i] = input[i];

	for(i=0;i<msg_len;i++)
	{
		q[i] = temp[0];
		if(q[i] == '0')
			for(j=0;j<key_len;j++)
				key[i] = '0';
		else
			for(j=0;j<key_len;j++)
				key[i] = divisor[i];
		for(j=key_len-1;j>0;j--)
		{
			if(temp[i] == key[i])
				rem[j-1] = '0';
			else
				rem[j-1] = '1';	
		}
		rem[key_len-1] = input[i+key_len]; 
		strcpy(temp,rem);
	}
	strcpy(rem,temp);

	printf("Remainder: ");
	for(i=0;i<strlen(rem);i++)
	{
		printf("%c",rem[i]);
	}



	close(network_socket);
	return 0;
}
