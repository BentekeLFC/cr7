/*

TTP3121 TCP/IP Programming(T1 2015/16) 
FILE : Myserver.c
        
GROUP MEMBERS:
	
1) DANIEL BRIAN THOMPSON	1122700901
2) HABINASH BALAKRISHAN	        1122700044 

*/

#include "inet.h"
#define PORT 8000
#define MAXSZ 1024
#define BUFSIZE 2048
int main()
{
int sockfd;//to create socket
int newsockfd;//to accept connection
int clilen;
char msg1[BUFSIZE+1];

struct sockaddr_in serverAddress;//server receive on this address
struct sockaddr_in clientAddress;//server sends to client on this address

int n;
char msg[MAXSZ];
int clientAddressLength;
int pid;
int bytereceive = 0;

//create socket
sockfd=socket(AF_INET,SOCK_STREAM,0);
//initialize the socket addresses
memset(&serverAddress,0,sizeof(serverAddress));
serverAddress.sin_family=AF_INET;
serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
serverAddress.sin_port=htons(PORT);

//bind the socket with the server address and port
bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));


/* Get the user name */ 
	char *usr; 
	usr=(char *)malloc(10*sizeof(char)); 
	usr=getlogin(); 

	/* set the 'server_file' path */ 
	char str[30]; 
	strcpy(str, "/home/"); 
	strcat(str, usr); 
	strcat(str, "/serverFile/"); 

	/* Check the path exist or not, if not, create one */ 
	struct stat s; 
	if(stat(str, &s) == -1){ 
	mkdir(str, 0700); }


//listen for connection from client maximum 5
listen(sockfd,5);

while(1)
{
//parent process waiting to accept a new connection
printf("\n*****server waiting for client connection:*****\n");
clientAddressLength=sizeof(clientAddress);
newsockfd=accept(sockfd,(struct sockaddr*)&clientAddress,&clientAddressLength);
printf("++.........A client connected......++++++++++++\n");

//child process is created for serving each new clients
pid=fork();
if(pid==0)//child process rec and send
{
 strcpy(msg1,"** Hello, you are connecting to the server please..** \n\nChoose your action...\n1)Send File\t\t2)Download File\n3)Create Directory\t4)Delete Directory\n[type /q to quit] : \nEnter (1,2,3,4):\n");
	  send(newsockfd, msg1, BUFSIZE, 0);
//rceive from client
do
{
n=recv(newsockfd,msg,MAXSZ,0);

 if(!strcmp(msg, "1"))
	{
		
		bzero( msg, sizeof(msg));
		recv(newsockfd, msg, BUFSIZE, 0);

		char filename[30];
		strcpy(filename, "/home/"); 
		strcat(filename, usr); 
		strcat(filename, "/serverFile/");
		strcat(filename, msg);
		

		FILE *fp;
   	 	fp = fopen(filename, "ab"); 
		bzero( msg, sizeof(msg));
		bytereceive = recv(newsockfd, msg, BUFSIZE, 0);
		fwrite(msg,1,bytereceive,fp);
		fclose(fp);

		bzero( msg, sizeof(msg));
		strcat(msg,"Managed to send... [/q to quit]");
	}

 if(!strcmp(msg, "2"))
	{	
		bzero( msg, sizeof(msg));
		strcat(msg,"Please enter[/q]\n\n");

		DIR *dir;
		struct dirent *ent;

		char directoryName[30];
   		strcpy(directoryName, "/home/"); 
    		strcat(directoryName, usr); 
    		strcat(directoryName, "/serverFile/");

		if ((dir = opendir (directoryName)) != NULL) {
		
			//print all the files and directories within directory 
 			while ((ent = readdir (dir)) != NULL) {

			strcat(msg, ent->d_name);
			strcat(msg, "\n");
 			}
 		 closedir (dir);
		send(newsockfd, msg, BUFSIZE, 0);
		 
		}else {

  		// could not open directory 
 		 perror ("Directory does not exist...");
  		return EXIT_FAILURE;
		}

	bool exist = true; 

	do{

        
	bzero( msg, sizeof(msg));
	recv(newsockfd, msg, BUFSIZE, 0);

	char filename[30];
    	strcpy(filename, "/home/"); 
    	strcat(filename, usr); 
    	strcat(filename, "/serverFile/");
    	strcat(filename, msg);
    
	
	FILE *fp = fopen(filename, "r");
	if(fp==NULL)
        {
            strcpy(msg," There is no such file in server...Please enter the correct filename with extension...");
	    send(newsockfd, msg, BUFSIZE, 0);  
	    exist = false; 
	   
        }  
	
	if(exist == true )
	{
	bzero( msg, sizeof(msg));
	int nread = fread(msg,1,256,fp);
	send(newsockfd, msg, nread, 0);
	}

	bzero( msg, sizeof(msg));
	strcpy(msg,"Managed to Download... [/q to quit]");

	}while(exist == false);
	}
		
	 
	if(!strcmp(msg, "3"))
	{
		bzero( msg, sizeof(msg));
		strcat(msg,"Managed to create Directory... [/q to quit]");
	}

	if(!strcmp(msg, "4"))
	{
		bzero( msg, sizeof(msg));
		strcat(msg,"Managed to delete Directory... [/q to quit]");
	}
if(n==0)
{
close(newsockfd);
break;
}
msg[n]=0;
send(newsockfd,msg,BUFSIZE,0);

//printf("%s\n",msg);
}while(strcmp(msg1, "/q"));//close do while
exit(0);
}
else
{
close(newsockfd);//sock is closed BY PARENT
}
}//close exterior while

return 0;
}

