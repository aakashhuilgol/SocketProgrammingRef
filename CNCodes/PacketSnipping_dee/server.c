#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
 #include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>


#define BUFLEN                 1024        /* buffer length */


int toDec(char hex[20]){
	int num;
	sscanf(hex,"%x",&num);
	return num;
}



/*void printUDP1(unsigned char * Buffer, int Size){
	printf("\n\nThe value is %s\n",Buffer);
	unsigned short iphdrlen;
	struct iphdr *iph=(struct iphdr *)Buffer;
	iphdrlen=(iph->ihl)*4;
	struct udphdr *udph=(struct udphdr*) (Buffer+iphdrlen);
	printf("Source : %x \n",ntohs(udph->source));
	printf("Destination %x \n ",ntohs(udph->dest));
	printf("Length %x \n",ntohs(udph->len));
	printf("Checksum %x \n",ntohs(udph->check));
}*/



void printUDP(unsigned long *a1,unsigned long *a2){
     unsigned short source,dest,length,checksum;

    source = ((*a1)>>16);

    dest = (*a1);

    length = ((*a2)>>16);

    checksum = (*a2);

    printf("\n*********HEX Values*********\n");

    printf("Source port no: %x\n",source);
    printf("Destination port no: %x\n",dest);

    printf("Length: %x\n",length);

    printf("Checksum: %x\n",checksum);

    printf("******************************\n");

    printf("\n*********Decimal Values*****\n");

    printf("Source port no: %d\n",source);

    printf("Destination port no: %d\n",dest);

    printf("Length: %d\n",length);

    printf("Checksum: %d\n",checksum);

    printf("******************************\n");

}
int main(int argc, char **argv)
{
        int n;
	int yes=1;
        int sd, new_sd, client_len, port;
        struct sockaddr_in server, client;
        char  buf[BUFLEN];

         port = atoi(argv[1]);
     
        /* create a stream socket */
        if((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
                fprintf(stderr,"can't create a socket\n");
                exit(1);
        }

        /* Fill the structure fileds with values */

        server.sin_family = AF_INET;
        server.sin_port = port;
        server.sin_addr.s_addr =inet_addr("127.0.0.1");
	
       // Reuse the port and address
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    		perror("setsockopt");
   		 exit(1);
	}

        /* bind an address to the socket */
        if(bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
        {
                fprintf(stderr, "can't bind name to socket\n");
                exit(1);
        }
        /* queue up to 5 connect requests */
        listen(sd,5);
	unsigned long a,b;
        while(1)
        {
		
                client_len = sizeof(client);
                if((new_sd = accept(sd, (struct sockaddr *) &client, &client_len)) == -1)
                {
                        fprintf(stderr, "can't accept client \n");
                        exit(1);
                }
		
                n = read(new_sd, buf, sizeof(buf));
		int s1=toDec(buf);
                
		n = read(new_sd, buf, sizeof(buf));
		int s2=toDec(buf);
    		
		printUDP(&s1,&s2);
		//printUDP1();
		strcpy(buf,"Done");

		
		write(new_sd, buf, BUFLEN);
        }

        close(sd);
        return(0);
}
