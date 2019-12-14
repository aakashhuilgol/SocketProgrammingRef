/**
    Handle multiple socket connections with select and fd_set on Linux
*/
  
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
  
#define TRUE   1
#define FALSE  0
#define PORT 8888
#define MAXLINE 256
#define MAXSIZE 1024
#define ACK                   2
#define NACK                  3
#define REQUESTFILE           100
#define COMMANDNOTSUPPORTED   150
#define COMMANDSUPPORTED      160
#define BADFILENAME           200
#define FILENAMEOK            400
  
int main(int argc , char *argv[])
{
    int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;
    int max_sd;
    struct sockaddr_in address;
      
    char buffer[1025];  //data buffer of 1K
      
    //set of socket descriptors
    fd_set readfds;
      
    //a message
    char *message = "ECHO Daemon v1.0 \r\n";
  
    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
      
    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
  
    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
  
    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);
     
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
      
    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");
     
    while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
  
        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;
         
        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++) 
        {
            //socket descriptor
            sd = client_socket[i];
             
            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);
             
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }
  
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
    
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }
          
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds)) 
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          
            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
        
            //send new connection greeting message
            doftp(new_socket);
              
            puts("Transfer successful\n");
              
            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++) 
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                     
                    break;
                }
            }
        }
          
        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++) 
        {
            sd = client_socket[i];
              
            if (FD_ISSET( sd , &readfds)) 
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                      
                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }
                  
                //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end of the data read
                    buffer[valread] = '\0';
                    send(sd , buffer , strlen(buffer) , 0 );
                }
            }
        }
    }
      
    return 0;
}

doftp(int newsd)
  {       
    int i,fsize,fd,msg_ok,fail,fail1,req,c,ack;
    int no_read ,num_blks , num_blks1,num_last_blk,num_last_blk1,tmp;
    char fname[MAXLINE];
    char out_buf[MAXSIZE];
    FILE *fp;
      
     no_read = 0;
     num_blks = 0;
     num_last_blk = 0; 

    
     /* START SERVICING THE CLIENT */ 
  
     /* get command code from client.*/
     /* only one supported command: 100 -  get a file */
     req = 0;
     if((readn(newsd,(char *)&req,sizeof(req))) < 0)
	 {printf("server: read error %d\n",errno);exit(0);}
     req = ntohs(req);
     printf("server: client request code is: %d\n",req);
     if (req!=REQUESTFILE) {
	 printf("server: unsupported operation. goodbye\n");
         /* reply to client: command not OK  (code: 150) */
         msg_ok = COMMANDNOTSUPPORTED; 
         msg_ok = htons(msg_ok);
         if((writen(newsd,(char *)&msg_ok,sizeof(msg_ok))) < 0)
            {printf("server: write error :%d\n",errno);exit(0);}
         exit(0);
         }

     /* reply to client: command OK  (code: 160) */
     msg_ok = COMMANDSUPPORTED; 
     msg_ok = htons(msg_ok);
     if((writen(newsd,(char *)&msg_ok,sizeof(msg_ok))) < 0)
             {printf("server: write error :%d\n",errno);exit(0);}
  
    fail = FILENAMEOK;
    if((read(newsd,fname,MAXLINE)) < 0) {
        printf("server: filename read error :%d\n",errno);
        fail = BADFILENAME ;
        }
   
     /* IF SERVER CANT OPEN FILE THEN INFORM CLIENT OF THIS AND TERMINATE */
     if((fp = fopen(fname,"r")) == NULL) /*cant open file*/
        fail = BADFILENAME;

     tmp = htons(fail);
     if((writen(newsd,(char *)&tmp,sizeof(tmp))) < 0)
        {printf("server: write error :%d\n",errno);exit(0);   }
     if(fail == BADFILENAME) {printf("server cant open file\n");
                            close(newsd);exit(0);}
     printf("server: filename is %s\n",fname);
  
    req = 0;
    if ((readn(newsd,(char *)&req,sizeof(req))) < 0)
       		      {printf("server: read error :%d\n",errno);exit(0);}
    printf("server: start transfer command, %d, received\n", ntohs(req));

   
   /*SERVER GETS FILESIZE AND CALCULATES THE NUMBER OF BLOCKS OF 
     SIZE = MAXSIZE IT WILL TAKE TO TRANSFER THE FILE. ALSO CALCULATE
     NUMBER OF BYTES IN THE LAST PARTIALLY FILLED BLOCK IF ANY. 
     SEND THIS INFO TO CLIENT, RECEIVING ACKS */
    printf("server: starting transfer\n");
    fsize = 0;ack = 0;   
    while ((c = getc(fp)) != EOF) {fsize++;}
    num_blks = fsize / MAXSIZE; 
    num_blks1 = htons(num_blks);
    num_last_blk = fsize % MAXSIZE; 
    num_last_blk1 = htons(num_last_blk);
    if((writen(newsd,(char *)&num_blks1,sizeof(num_blks1))) < 0)
             {printf("server: write error :%d\n",errno);exit(0);}
    printf("server: told client there are %d blocks\n", num_blks);  
    if((readn(newsd,(char *)&ack,sizeof(ack))) < 0)
        {printf("server: ack read error :%d\n",errno);exit(0); }          
    if (ntohs(ack) != ACK) {
      printf("client: ACK not received on file size\n");
      exit(0);
      }
    if((writen(newsd,(char *)&num_last_blk1,sizeof(num_last_blk1))) < 0)
       {printf("server: write error :%d\n",errno);exit(0);}
    printf("server: told client %d bytes in last block\n", num_last_blk);  
    if((readn(newsd,(char *)&ack,sizeof(ack))) < 0)
        {printf("server: ack read error :%d\n",errno);exit(0); }
    if (ntohs(ack) != ACK) {
      printf("server: ACK not received on file size\n");
      exit(0);
      }
    rewind(fp);    
    
  
    /* ACTUAL FILE TRANSFER STARTS  BLOCK BY BLOCK*/       
       
  
  for(i= 0; i < num_blks; i ++) { 
      no_read = fread(out_buf,sizeof(char),MAXSIZE,fp);
      if (no_read == 0) {printf("server: file read error\n");exit(0);}
      if (no_read != MAXSIZE)
              {printf("server: file read error : no_read is less\n");exit(0);}
      if((writen(newsd,out_buf,MAXSIZE)) < 0)
                 {printf("server: error sending block:%d\n",errno);exit(0);}
      if((readn(newsd,(char *)&ack,sizeof(ack))) < 0)
                 {printf("server: ack read  error :%d\n",errno);exit(0);}
      if (ntohs(ack) != ACK) {
          printf("server: ACK not received for block %d\n",i);
          exit(0);
          }
      printf(" %d...",i);
      }

   if (num_last_blk > 0) { 
      printf("%d\n",num_blks);
      no_read = fread(out_buf,sizeof(char),num_last_blk,fp); 
      if (no_read == 0) {printf("server: file read error\n");exit(0);}
      if (no_read != num_last_blk) 
            {printf("server: file read error : no_read is less 2\n");exit(0);}
      if((writen(newsd,out_buf,num_last_blk)) < 0)
                 {printf("server: file transfer error %d\n",errno);exit(0);}
      if((readn(newsd,(char *)&ack,sizeof(ack))) < 0)
	         {printf("server: ack read  error %d\n",errno);exit(0);}
      if (ntohs(ack) != ACK) {
          printf("server: ACK not received last block\n");
          exit(0);
          }
      }
    else printf("\n");
                                                  
   /* FILE TRANSFER ENDS */
   printf("server: FILE TRANSFER COMPLETE on socket %d\n",newsd);
   fclose(fp);
   close(newsd);
  }


/*
  TO TAKE CARE OF THE POSSIBILITY OF BUFFER LIMMITS IN THE KERNEL FOR THE
 SOCKET BEING REACHED (WHICH MAY CAUSE READ OR WRITE TO RETURN FEWER CHARACTERS
  THAN REQUESTED), WE USE THE FOLLOWING TWO FUNCTIONS */  
   
int readn(int sd,char *ptr,int size)
{         int no_left,no_read;
          no_left = size;
          while (no_left > 0) 
                     { no_read = read(sd,ptr,no_left);
                       if(no_read <0)  return(no_read);
                       if (no_read == 0) break;
                       no_left -= no_read;
                       ptr += no_read;
                     }
          return(size - no_left);
}

int writen(int sd,char *ptr,int size)
{         int no_left,no_written;
          no_left = size;
          while (no_left > 0) 
                     { no_written = write(sd,ptr,no_left);
                       if(no_written <=0)  return(no_written);
                       no_left -= no_written;
                       ptr += no_written;
                     }
          return(size - no_left);
}


     
