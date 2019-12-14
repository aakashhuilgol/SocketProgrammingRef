/*

//////  Author Details
//////  Name : Chethan Phani
//////  Roll Number : 43
//////  USN : 01fe15bcs047
//////  Batch : A2

//////  CLIENT  

*/

    /*FTP Client*/
    #include <sys/socket.h>  // Unix headers for Socket Programming
    #include <netinet/in.h>
    #include <string.h>
    #include <stdio.h>
  
    #include <stdlib.h>


	int LOGIN_FLAG=0;
    int main(int argc,char *argv[])
    {
      struct sockaddr_in server;
    
      int sock;
      int choice;
      char buf[100], buf3[100],command[100],cmd[50],next[50];
      int k, size, status;
      int filehandle;

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
    LOGIN_FLAG=0;
    char user[50],pass[50];
    while(1)
	{


	if(LOGIN_FLAG==0){

		recv(sock, buf, 100, 0);
		if(!strcmp(buf,"login_please")){
			printf("\nUsername : ");
			scanf("%s",user);
			printf("\nPassword : ");
			scanf("%s",pass);
			
			strcpy(buf,user);
			send(sock, buf, 100, 0);
			strcpy(buf,pass);
			send(sock, buf, 100, 0);

			recv(sock, buf, 100, 0);
			if(!strcmp(buf,"true")){
				LOGIN_FLAG=1;
				printf("\nLogin Successfull !\n");
			}else if(!strcmp(buf,"false")){
				printf("\nInvalid Try Again !\n");
			}
			 
			
			
		}


	}else if(LOGIN_FLAG==1){
	
		printf("\nEnter the ftp command to execute : ");       // Enter the ftp command to execute
		gets(cmd);
		strcpy(buf,cmd);
		send(sock, buf, 100, 0);
		if(!strcmp(buf,"ls"))
		{
			printf("\nEnter the name of folder to display the list !\n");     // Enter the next folder path to list the files
			gets(next);
			strcpy(buf,next);
			send(sock, buf, 100, 0);

		}
		if(!strcmp(buf,"mkdir"))
		{
			printf("\nEnter the name of folder to create !\n");     // Enter the next folder path create
			gets(next);
			strcpy(buf,next);
			send(sock, buf, 100, 0);
			printf("\nSuccessfully created\n\n");
			continue;

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
		printf("\n******************************************************\n\nThe recieved List of files in parent directoy are : \n\n");
		while(strcmp(buf,"over")){
			printf("%s\n",buf);
			recv(sock, buf, 100, 0);
			
		}
		printf("\n***********************************************************\n\n");

	       }

     }   	
       
  }
     
