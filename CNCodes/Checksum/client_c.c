#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>


//Compute Internet checksum for count bytes beginning at location addr.
unsigned short cksum(unsigned short *addr, int count)
{
	register long sum = 0;

	while ( count > 1 ) 
	{
	 	sum += *addr++;
	 	count -=2;
	}


	//Adding of the left over bits
	if ( count > 0 )
		sum += *addr;


	//Fold 32-bit sum to 16 bits
	while (sum >>16)
		sum = (sum & 0xffff) + (sum >> 16);

	return sum;		//returns sum
}

int main(int argc, char *argv[])
{
	int sd = 0,port;
    	char buf[40]="";
    	struct sockaddr_in server;
	port = atoi(argv[1]);


    	//Creation of socket first
    	if((sd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    	{
        	printf("\n Failed to create socket \n");
        	return 1;
    	}


    	//Initialize the datafields in sockaddr
    	server.sin_family = AF_INET;
    	server.sin_port =port;
    	server.sin_addr.s_addr = inet_addr("127.0.0.1");


    	//Connecting to the server
    	if(connect(sd, (struct sockaddr *)&server, sizeof(server))<0)
    	{
        	printf("\n Failed to connect \n");
        	return 1;
    	}


	
  	unsigned short a, sum_client,checksum_client;
    	int count;
    	char value[12];
	unsigned long int *p;
	p=&a;

	printf("\n---------------------------------\n\n");
    	printf("Enter the data :\n");
	printf("Input : ");
	scanf("%X",&a);			//Reads input from the user
	printf("Count : ");
    	scanf("%d",&count);		//Read the count from user

    	sprintf(value,"%X",*p);		//Converts the data into hex
    	write(sd, value, 16);		//Send the data to the server

    	sprintf(value,"%X",count);	//Converts the count to hex
    	write(sd, value, 16);		//Send the count to the server

    	sum_client=cksum(&a,count);

	checksum_client=(~sum_client);
    	sprintf(value,"%X",checksum_client);	//Converts the checksum to hex

    	printf("Sum @ client : %X \n",sum_client);
	printf("Checksum @ client : %X \n",checksum_client);

   	write(sd, value, 16);		//send the checksum to the server

    	int n=read(sd,buf,sizeof(buf));		//Recieved from the server
    	printf("%s\n",buf);			//Acknowledgement
	printf("\n---------------------------------\n");
    	return 0;
}
