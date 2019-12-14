    #include <sys/socket.h> 
    #include <netinet/in.h>
    #include <string.h>
    #include <stdio.h>
  
    #include <stdlib.h>
    int main(int argc,char *argv[])
    {
      struct sockaddr_in server;
    
      int sock;
      int choice;
      char buf[100], buf3[100],command[100],cmd[50],next[50];
      int k, size, status;
      int filehandle;

      sock = socket(AF_INET, SOCK_STREAM, 0);             // Socket Creation
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

    while(1)
	{
        printf("\nEnter the ftp command to execute : ");
	gets(cmd);
        strcpy(buf,cmd);
        send(sock, buf, 100, 0);
	if(!strcmp(buf,"ls"))
	{
		printf("\nEnter the name of folder to display the list\n");     
		gets(next);
		strcpy(buf,next);
        	send(sock, buf, 100, 0);

	}
	recv(sock, buf, 100, 0);
	if(!strcmp(buf,"no"))
	{
		printf("\nEnter the Valid Command !\n");
		continue;
	}
	if(!strcmp(buf,"nopar"))
	{
		printf("\nNo parent directory in server !\n");
		continue;
	}
	printf("\nThe recieved List of files in parent directoy are : \n");
	while(strcmp(buf,"over")){
		printf("%s\n",buf);
		recv(sock, buf, 100, 0);
			
	}

       }   	
       
  }
     
