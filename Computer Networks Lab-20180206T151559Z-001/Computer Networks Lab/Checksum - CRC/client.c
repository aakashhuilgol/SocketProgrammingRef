    /*FTP Client*/
    #include <sys/socket.h>  // Unix headers for Socket Programming
    #include <netinet/in.h>
    #include <string.h>
    #include <stdio.h>
  
    #include <stdlib.h>

    typedef unsigned short CRC;

    #define WIDTH    (8 * sizeof(CRC))
    #define MSB   (1 << (WIDTH - 1))
    #define KEY 0x8001

    int main(int argc,char *argv[])
    {
      struct sockaddr_in server;
    
      int sock;
      int choice;
      char buf[100], buf3[100],command[100],cmd[50],next[50];
      int k, size, status;
      int filehandle;

	char message[64], resp, dummy[16];
    CRC crcCode;
    int l;

      sock = socket(AF_INET, SOCK_STREAM, 0);             // Creation of the Socket
      if(sock == -1)
        {
          printf("socket creation failed");
          exit(1);
        }
      server.sin_family = AF_INET;
      server.sin_port = atoi(argv[1]);
      server.sin_addr.s_addr = inet_addr("127.0.0.1");

      k = connect(sock,(struct sockaddr*)&server, sizeof(server));   // Connect to the established ftp Server
      if(k == -1)
        {
          printf("Connect Error");
          exit(1);
        }

	while(1){
	recv(sock, buf, 100, 0);
	strcpy(message,buf);
   	
    CRC  remainder = 0;
    int byte,nBytes=strlen(message);
    unsigned char bit;
    for ( byte = 0; byte < nBytes; ++byte)
    {
	 remainder ^= (message[byte] << (WIDTH - 8));
	 for ( bit = 8; bit > 0; --bit)
        {     
    	    if (remainder & MSB)
                remainder = (remainder << 1) ^ KEY;
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    if( remainder != 0 )
        printf("********* Error ******\n");
    else
        printf("*********** No error *********\n");  

	}
}
     
