

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

int toDec(char hex[20]){
	int num;
	sscanf(hex,"%x",&num);
	return num;
}
int main(int argc, char *argv[])
{
    int sd = 0,port;
    int bytesReceived = 0;
    char buf[40]="";
    struct sockaddr_in server;
	port = atoi(argv[1]);
    /* Create a socket first */
    if((sd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    /* Initialize sockaddr_in data structure */
    server.sin_family = AF_INET;
    server.sin_port =port; // port
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Attempt a connection */
    if(connect(sd, (struct sockaddr *)&server, sizeof(server))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }
    unsigned long a1,a2;
    char hhh[12];
    unsigned long int *p1,*p2;
    p1=&a1;
    p2=&a2;
    printf("Enter udp string\n");
    scanf("%x",&a1);
    sprintf(hhh,"%x",*p1);
    write(sd, hhh, 16);
    scanf("%x",&a2);
    sprintf(hhh,"%x",*p2);
    write(sd, hhh, 16);
    
    int n=read(sd,buf,sizeof(buf));
    printf("%s\n",buf);
    return 0;
}
