#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
 #include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>


#define BUFLEN 1024        /* buffer length */

int main(int argc, char **argv)
{
        int n;
	int yes=1,i;
        int sd, new_sd, client_len, port;
        struct sockaddr_in server, client;
        char  buf[BUFLEN];

	FILE *fp;
	fp = fopen("Output.txt", "w");// "w" means that we are going to write on this file
		

	char source[100],dest[100],seqn[100],ackn[100],ws[100],hll[100],vers[100],header[100],tts[100],tlen[100],ttlc[100],protc[100],checksumc[100],idents[100],flagc[100],fragc[100];
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

        while(1)
        {
                client_len = sizeof(client);

                if((new_sd = accept(sd, (struct sockaddr *) &client, &client_len)) == -1)
                {
                        fprintf(stderr, "can't accept client \n");
                        exit(1);
                }

                while(1){
                n = read(new_sd, source, sizeof(buf));
		n = read(new_sd, dest, sizeof(buf));
		n = read(new_sd, vers, sizeof(buf));
		n = read(new_sd, header, sizeof(buf));
		n = read(new_sd, tts, sizeof(buf));
		n = read(new_sd, tlen, sizeof(buf));
		n = read(new_sd, ttlc, sizeof(buf));
		n = read(new_sd, protc, sizeof(buf));
		n = read(new_sd, idents, sizeof(buf));
		n = read(new_sd, flagc, sizeof(buf));
		n = read(new_sd, fragc, sizeof(buf));
		n = read(new_sd, checksumc, sizeof(buf));
		printf("Version : %s \n",vers);
printf("Header length : %s \n",header);
printf("Type of service : %s \n",tts);
printf("Total Length : %s \n",tlen);
printf("Identifier : %s \n",idents);
printf("Flags : %s \n",flagc);
printf("Fragmentation Offset : %s \n",fragc);
printf("Time to live : %s\n",ttlc);
printf("Protocol : %s \n",protc);
printf("Internet Checksum : %s \n",checksumc);
printf("Source Address : %s\n",source);
printf("Destination Address : %s\n",dest);

//File printing

fprintf(fp, "Initiation by server");
fprintf(fp, "Version : %s \n",vers);
fprintf(fp, "Header length : %s \n",header);
fprintf(fp, "Type of service : %s \n",tts);
fprintf(fp, "Total Length : %s \n",tlen);
fprintf(fp, "Identifier : %s \n",idents);
fprintf(fp, "Flags : %s \n",flagc);
fprintf(fp, "Fragmentation Offset : %s \n",fragc);
fprintf(fp, "Time to live : %s\n",ttlc);
fprintf(fp, "Protocol : %s \n",protc);
fprintf(fp, "Internet Checksum : %s \n",checksumc);
fprintf(fp, "Source Address : %s\n",source);
fprintf(fp, "Destination Address : %s\n",dest);

fclose(fp);

                }
 		close(new_sd);

        }

        close(sd);
        return(0);
}
