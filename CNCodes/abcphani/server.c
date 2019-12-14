#include <sys/socket.h>
#include <netinet/in.h>			
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<fcntl.h>
#include <dirent.h>
DIR *d;    		//Structure to hold the list of directories 
DIR *d2,*d1;
struct dirent *dir,*dir2,*dir1;	 // dirent structure to hold the information of each file

int main(int argc,char *argv[])
{
  struct sockaddr_in server, client;
  int sock1, sock2;
  char buf[100],buf1[100],buf3[100],command[100], filename[20];
  int k, i, size, len, c;
  sock1 = socket(AF_INET, SOCK_STREAM, 0);       //Socket Creation
  if(sock1 == -1)
    {
      printf("Socket creation failed");
      exit(1);
    }

  server.sin_family = AF_INET;
  server.sin_port = atoi(argv[1]);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  k = bind(sock1,(struct sockaddr*)&server,sizeof(server));     //Socket Binding
  if(k == -1)
    {
      printf("Binding error");
      exit(1);
    }
  k = listen(sock1,1);			
  if(k == -1)
    {
      printf("Listen failed");
      exit(1);
    }
  len = sizeof(client);
  sock2 = accept(sock1,(struct sockaddr*)&client, &len);     // Accept the incoming connection 
  i = 1;
   while(1){
      
      recv(sock2, buf, 100, 0);
      
	if(!strcmp(buf, "cd up"))
	{

	    int flag=1,count1=0,count2=0;
	    d1 = opendir("..");
	    d2 = opendir(".");				 											
	    if (d1)							
	    {								
		while ((dir1 = readdir(d1)) != NULL)			
		{							
		 	count1++;   					
		}							
		closedir(d1);        //Logic to check if Parent doesn't exists;
	    }								
	    if (d2)							
	    {								
		while ((dir2 = readdir(d2)) != NULL)			
		{							
		 	count2++;   					
		}							
		closedir(d2);						
	    }
	    if(count1==count2){
	  	flag=0;		
	    }


		if(flag==1){

	    d = opendir("..");            // Open parent directory
	    if (d)
	    {
		while ((dir = readdir(d)) != NULL)   // Fetch file contents in directory
		{
		    //printf("%s\n", dir->d_name); 
		    strcpy(buf3,dir->d_name);
		    send(sock2, buf3, 100, 0);	// sending file name to client
		}
		strcpy(buf3,"over");
		send(sock2, buf3, 100, 0);
		closedir(d);
	    }
	}else if(flag==0){
		strcpy(buf3,"nopar");
		send(sock2, buf3, 100, 0);
	}

	}
	else if(!strcmp(buf, "ls"))
	{

	    recv(sock2, buf, 100, 0); // get the next folder name
	    d = opendir(buf);                                     // Opening the next directory
	    if (d)
	    {
		while ((dir = readdir(d)) != NULL)       // Fetch file contents in directory
		{
		    //printf("%s\n", dir->d_name);
		    strcpy(buf3,dir->d_name);
		    send(sock2, buf3, 100, 0); // send file name to client
		}
		strcpy(buf3,"over");
		send(sock2, buf3, 100, 0);


		closedir(d);
	    }
	}
	else {
		strcpy(buf3,"no");	// For Invalid Command
		 send(sock2, buf3, 100, 0);
	}

	
      
  }
    
  return 0;
}
