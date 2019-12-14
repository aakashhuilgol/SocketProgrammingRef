/*

//////  Author Details
//////  Name : Chethan Phani
//////  Roll Number : 43
//////  USN : 01fe15bcs047
//////  Batch : A2

//////  SERVER
*/
#include <sys/socket.h>
#include <netinet/in.h>				// Unix headers for Socket Programming
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<fcntl.h>
#include <dirent.h>
DIR *d;    					// DIR structure to hold the list of directories 
DIR *d2,*d1;
struct dirent *dir,*dir2,*dir1;			// dirent structure to hold the information of each file

int main(int argc,char *argv[])
{
  struct sockaddr_in server, client;
  int sock1, sock2;
  char buf[100],buf1[100],buf3[100],command[100], filename[20];
  int k, i=0,j=0, size, len, c;
  sock1 = socket(AF_INET, SOCK_STREAM, 0);       		// Creation of the Socket
  if(sock1 == -1)
    {
      printf("Socket creation failed");
      exit(1);
    }

  server.sin_family = AF_INET;
  server.sin_port = atoi(argv[1]);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  k = bind(sock1,(struct sockaddr*)&server,sizeof(server));           // Binding the Socket to publish the IP address.
  if(k == -1)
    {
      printf("Binding error");
      exit(1);
    }
  k = listen(sock1,1);						// Make the socket to listen
  if(k == -1)
    {
      printf("Listen failed");
      exit(1);
    }
  len = sizeof(client);
  sock2 = accept(sock1,(struct sockaddr*)&client, &len);     // Accept the incoming connection 
  i = 1;

	char o_data[1000],DAT[1000],o1_data[1000];
	char key[1000];
	
	printf("Enter the 16 bit data to be sent : ");
	scanf("%s",o1_data);
	strcpy(DAT,o1_data);

	printf("Enter the Key to be used ");
	scanf("%s",key);
	int len_o = strlen(o1_data);
	len=strlen(key);
	printf("%d   %d\n",len_o,len);
	int totlen=len_o+len-1;
	char temp1[100],temp2[100];
	for(i=0;i<len-1;i++){
		o1_data[len_o+i]='0';
	}
	for(i=0;i<totlen;i++){
		o_data[i]=o1_data[i];
	}
	printf("\nAfter adding 0's to data : %s\n",o_data);
	len_o=strlen(o_data);
	

	printf("Len : %d\n",len_o);
	int lim = len-1;
	for(i=0;i<len_o-lim;i++){
		temp1[i]=o_data[i];
		if(temp1[i]=='0'){
			for(j=1;j<len;j++){
				temp2[j-1]=o_data[i+j];
			}	
		}else if(temp1[i]=='1'){

			for(j=0;j<len;j++){
				if(o_data[i+j]==key[j])
					o_data[i+j]='0';
				else
					o_data[i+j]='1';
				if(j>0)
					temp2[j-1]=o_data[i+j];				
				
			}
		}
		
	}
	printf("\n\nCRC : %s\n",temp2);
	len_o=strlen(DAT);
	for(i=0;i<len-1;i++){
		DAT[len_o+i]=temp2[i];
	}
	printf("\n\ndata to be sent : %s\n\n*********** Data Sent *****************\n",DAT);
	strcpy(buf3,DAT);
	send(sock2, buf3, 100, 0);
	strcpy(buf3,key);
	send(sock2, buf3, 100, 0);
      

    
  return 0;
}
