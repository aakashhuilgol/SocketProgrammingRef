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

FILE *fp;

fp = fopen("Output.txt", "w");// "w" means that we are going to write on this file
        char buf[BUFLEN];
	unsigned long long aa,bb;
unsigned long sdp,seq,ack,d,misc,g,mi,l, p,pq,h,cd,destn,sour;
unsigned short b,c,e,f,k,t1,t2, q,r,s, tot_len,ser,ver,head,ident,flags,frag,qr,checksum,ttl,prot;
	char source[100],dest[100],seqn[100],ackn[100],ws[100],hll[100],vers[100],header[100],tts[100],tlen[100],ttlc[100],protc[100],checksumc[100],idents[100],flagc[100],fragc[100];
        
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


p=aa>>32;
h=aa;
q=p>>16;

tot_len=p;

r=q>>8;


ser=q;
ver=r>>4;
head=r;

ident=h>>16;
s=h;
flags=h>>3;
frag=h;

pq=bb>>32;
sour=bb;
cd=pq;
qr=cd>>16;
checksum=cd;
ttl=qr>>8;
prot=qr;


destn=misc;

sprintf(source,"%x",sour);
sprintf(dest,"%x",destn);
sprintf(vers,"%x",ver);
sprintf(flagc,"%x",flags);
sprintf(fragc,"%x",frag);
sprintf(idents,"%x",ident);
sprintf(header,"%x",head);
sprintf(tts,"%x",ser);
sprintf(tlen,"%x",tot_len);
sprintf(ttlc,"%x",ttl);
sprintf(protc,"%x",prot);
sprintf(checksumc,"%x",checksum);



	if(ver==1){
	
        write(sd, source, BUFLEN);        /* send it out */
	write(sd,dest, BUFLEN); 
	write(sd, vers, BUFLEN); 
	write(sd, header, BUFLEN); 
	write(sd, tts, BUFLEN); 
	write(sd, tlen, BUFLEN); 
	write(sd, ttlc, BUFLEN); 
	write(sd, protc, BUFLEN); 
	write(sd, idents, BUFLEN); 
	write(sd, flagc, BUFLEN); 
	write(sd, fragc, BUFLEN); 
	write(sd, checksumc, BUFLEN); 
        printf("Echoed Messege:\n**************\n");
       

        printf("%s\n",source); 

	}
	else
	{
		printf("\n\n\nINITIATION BY CLIENT\n");
printf("Version : %x \n",ver);
printf("Header length : %x \n",head);
printf("Type of service : %x \n",ser);
printf("Total Length : %x \n",tot_len);
printf("Identifier : %x \n",ident);
printf("Flags : %x \n",flags);
printf("Fragmentation Offset : %x \n",frag);
printf("Time to live : %x\n",ttl);
printf("Protocol : %x \n",prot);
printf("Internet Checksum : %x \n",checksum);
printf("Source Address : %x\n",sour);
printf("Destination Address : %x\n",destn);

//File printing

fprintf(fp, "Initiation by client\n");
fprintf(fp, "Version : %x \n",ver);
fprintf(fp, "Header length : %x \n",head);
fprintf(fp, "Type of service : %x \n",ser);
fprintf(fp, "Total Length : %x \n",tot_len);
fprintf(fp, "Identifier : %x \n",ident);
fprintf(fp, "Flags : %x \n",flags);
fprintf(fp, "Fragmentation Offset : %x \n",frag);
fprintf(fp, "Time to live : %x\n",ttl);
fprintf(fp, "Protocol : %x \n",prot);
fprintf(fp, "Internet Checksum : %x \n",checksum);
fprintf(fp, "Source Address : %x\n",sour);
fprintf(fp, "Destination Address : %x\n",destn);
fclose(fp);	
      }



}
/*
printf("source port number= %x\n",sour);
printf("Destination port number=%x\n",c);
printf("Sequence number=%x\n",seq);
printf("Acknowledgement number=%x\n",ack);
printf("Window size=%x\n",e);
printf("Header length=%x Bytes\n",f*4);
*/
       close(sd);
       return(0); 
}
