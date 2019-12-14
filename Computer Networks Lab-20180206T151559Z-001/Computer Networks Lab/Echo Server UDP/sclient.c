#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>


#define  BUFLEN                   1024                 /* buffer length */

int main(int argc, char **argv)
{
        int n;
        int sd, port;
        char buf[BUFLEN];
	int server_port = 4001;
        
       struct sockaddr_in server;
     
	server.sin_family = AF_INET;
      //  server.sin_port = port;
		server.sin_port = htons(server_port);        
		server.sin_addr.s_addr = inet_addr("35.202.19.80");
	//server.sin_addr.s_addr = htonl(INADDR_ANY);

       port=atoi(argv[1]);
while(1){
        /* create a stream socket */
        if(( sd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
        {
                fprintf(stderr, "can't create a socket\n");
                exit(1);
        }

       // bzero((char *)&server, sizeof(struct sockaddr_in));
        

        /* connecting to the server */
        if(connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
        {
                fprintf(stderr, "can't connect\n");
                exit(1);
        }

        printf("Enter the message to be echoed: ");       
        scanf("%s",buf);                /* get user's text */
	if(!strcmp(buf,"exit"))
		break;
	int len = sendto(sd, buf, BUFLEN,0,(struct sockaddr *)&server,sizeof(server));        
//write(sd, buf, BUFLEN);        /* send it out */
      //  printf("Echoed Messege:\n**************\n");
       	char buffer[100];
       // n = read(sd, buf, sizeof(buf));
	recvfrom(sd, buf, sizeof(buf),0,NULL,NULL);
       // printf("%s\n",buf); 
  //    }
}
       close(sd);
       return(0); 
}

