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


 //Compute Internet checksum for count bytes beginning at location addr
unsigned short cksum(unsigned short *addr, int count)
{
	register long sum = 0;
	while ( count > 1 ) 
	{
	 	sum += *addr++;
	 	count -=2;
	}

	
	//Adding the left over bits
	if ( count > 0 )
		sum += *addr;


	//Folding 32 bits to 16 bits
	while (sum >>16)
		sum = (sum & 0xffff) + (sum >> 16);


	return sum;		//returns sum
}



int main(int argc, char **argv)
{
        int n;
	int yes=1,i;
        int sd, new_sd, client_len, port;
        struct sockaddr_in server, client;
        char  buf[100]="";
	port = atoi(argv[1]);
    

        //Creation of the socket
        if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
                fprintf(stderr,"can't create a socket\n");
                exit(1);
        }


        
	//Initialize the data fields in sockaddr
        server.sin_family = AF_INET;
        server.sin_port = port;
        server.sin_addr.s_addr =inet_addr("127.0.0.1");
	


        // Reuse the port and address
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    		perror("setsockopt");
   		 exit(1);
	}


        //bind an address to the socket
        if(bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
        {
                fprintf(stderr, "can't bind name to socket\n");
                exit(1);
        }


        //queue up to 5 connect requests
        listen(sd,5);


        while(1)
        {
                client_len = sizeof(client);
		printf("Loading.....\n");

                if((new_sd = accept(sd, (struct sockaddr *) &client, &client_len)) == -1)
                {
                        fprintf(stderr, "can't accept client \n");
                        exit(1);
                }
		
		
		int n;
		long int data;
		char summ[50],cnt[10];
		unsigned short sum_server,ch_client,Final_sum,checksum_server;
		int c=0;

		n=read(new_sd,summ,16);		//Recieves data from Client
		data=toDec(summ);
		
		n=read(new_sd,cnt,16);		//Recieves count from client
		c=toDec(cnt);

		printf("\n---------------------------------\n\n");
		printf("Input received from the client : %X \n",data);
		printf("Count received from the client : %d \n",c);
				
		sum_server=cksum(&data,c);	//sum 
		printf("Sum @ server : %X\n",sum_server);

		n=read(new_sd,summ,16);		//Recieves checksum from client
		ch_client=toDec(summ);
		printf("Checksum received from client : %X\n",ch_client);

		Final_sum=sum_server+ch_client;		//final sum : Sum @ server side with checksum

		checksum_server = ~Final_sum;		//Checksum @ server side
		printf("Checksum @ server : %X\n",checksum_server);
		printf("\n---------------------------------\n");


		//Check whether the any error is generated or no.
		if(checksum_server==0x0000)
		{
			strcpy(buf,"Acknowledgement received!!");
		}
		else 
			strcpy(buf,"Error detected!!");

        	write(new_sd, buf, 256);		//Sends acknowledgement or alert message to the client
        	close(new_sd);

        }

        close(sd);
        return(0);
}
