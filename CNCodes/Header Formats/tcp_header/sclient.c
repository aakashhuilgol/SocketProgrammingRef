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

FILE *fp;

fp = fopen("Output.txt", "w");// "w" means that we are going to write on this file
  
	unsigned long long aa,bb;
unsigned long sdp,seq,ack,d,misc,g,mi,l;
unsigned short b,c,e,f,k,t1,t2,p,q,r;
	char source[100],dest[100],seqn[100],ackn[100],ws[100],hll[100];
        
       struct sockaddr_in server;
     
       port=atoi(argv[1]);

        /* create a stream socket */
        if(( sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
                fprintf(stderr, "can't create a socket\n");
                exit(1);
        }

       // bzero((char *)&server, sizeof(struct sockaddr_in));
        server.sin_family = AF_INET;
        server.sin_port = port;
        server.sin_addr.s_addr = inet_addr("127.0.0.1");


        /* connecting to the server */
        if(connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
        {
                fprintf(stderr, "can't connect\n");
                exit(1);
        }

	while(1){	
	printf("Enter first 8 bytes:\n");
//scanf("%x",&sdp);
scanf("%llx",&aa);
printf("Enter next 8 bytes:\n");
//scanf("%x",&seq);
scanf("%llx",&bb);
//printf("Enter next 4 bytes:\n");
//scanf("%x",&ack);
printf("Enter next 4 bytes:\n");
scanf("%x",&misc);

//source and destination port
sdp=aa>>32;

//seq number
seq=aa;

//ack number
ack=bb>>32;

//bits header window
mi=bb;
l=mi;

k=mi>>16; //hl
d=sdp;

q=k;
p=k>>8;
r=q<<4;

printf("Acks is %x\n\n",r);


//header length
f=k>>12;
//window size



e=l;

//right shift
b=sdp>>16;

//left shift
c=d;




sprintf(source,"%x",b);
sprintf(dest,"%x",c);
sprintf(seqn,"%x",seq);
sprintf(ackn,"%x",ack);
sprintf(ws,"%x",e);
sprintf(hll,"%x",f*4);


	if(ackn==1){
	
        write(sd, source, BUFLEN);        /* send it out */
write(sd,dest, BUFLEN); 
write(sd, seqn, BUFLEN); 
write(sd, ackn, BUFLEN); 
write(sd, ws, BUFLEN); 
write(sd, hll, BUFLEN); 
        printf("Echoed Messege:\n**************\n");
       

        printf("%s\n",source); 

	}
	else
	{
		printf("INITIATION BY CLIENT\n");
		printf("source port number= %x\n",b);
printf("Destination port number=%x\n",c);
printf("Sequence number=%x\n",seq);
printf("Acknowledgement number=%x\n",ack);
printf("Window size=%x\n",e);
printf("Header length=%x Bytes\n",f*4);


fprintf(fp, "Initiation by client\n");
fprintf(fp, "source port number= %x\n",b);
fprintf(fp, "Destination port number=%x\n",c);
fprintf(fp, "Sequence number=%x\n",seq);
fprintf(fp, "Acknowledgement number=%x\n",ack);
fprintf(fp, "Window size=%x\n",e);
fprintf(fp, "Header length=%x Bytes\n",f*4);
fclose(fp);
	}
      }
       close(sd);
       return(0); 
}
