/*
TTP3121 TCP/IP Programming(T1 2015/16) 
FILE : server.c
        
GROUP MEMBERS:
	
1) DANIEL BRIAN THOMPSON	1122700901
2) HABINASH BALAKRISHAN	        1122700044 
*/


#include "inet.h"
#define PORT 8000
#define SERVER_IP "127.0.0.1"
#define MAXSZ 1024
int main()
{
int sockfd;
//to create the socket

struct sockaddr_in serverAddress;
//The client will connect to this

int n;
int bfreceiv = 0;
char mmo[MAXSZ+1];
char cr8[25];
char dlt[25];

static struct sigaction act;

void catchin(int);

act.sa_handler = catchin;
sigfillset(&(act.sa_mask));

sigaction(SIGINT, &act, (void *) 0);


//creates the socket
sockfd=socket(AF_INET,SOCK_STREAM,0);
//initializes the socket addresses
memset(&serverAddress,0,sizeof(serverAddress));
serverAddress.sin_family=AF_INET;
serverAddress.sin_addr.s_addr=inet_addr(SERVER_IP);
serverAddress.sin_port=htons(PORT);

//client  connecting to the server
connect(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress));

/* Gets the username */ 
	char *usr; 
	usr=(char *)malloc(10*sizeof(char)); 
	usr=getlogin(); 


	char str[30]; 
	strcpy(str, "/home/"); 
	strcat(str, usr); 
	strcat(str, "/clientFile/"); 
/* Setting the 'clientFile' path */ 

	/* Check the path exist or not, if not, create one */ 
	struct stat s; 
	if(stat(str, &s) == -1){ 
	mkdir(str, 0700); }

//send to sever and receive from server
do
{
bzero( mmo, sizeof(mmo));
recv(sockfd,mmo,MAXSZ,0);

printf("%s\n",mmo);


gets(mmo);

send(sockfd,mmo,MAXSZ,0);

 if(!strcmp(mmo, "1"))
{

	DIR *dir;
	struct dirent *ent;

	char directoryName[30];
   	strcpy(directoryName, "/home/"); 
    	strcat(directoryName, usr); 
    	strcat(directoryName, "/clientFile/");

	if ((dir = opendir (directoryName)) != NULL) {
		
		printf("\n[List of files in Client Directory]\n");
  		// print all the files and directories within directory 
  		while ((ent = readdir (dir)) != NULL) {

		printf("%s\n", ent->d_name);    }

 		closedir (dir);
	}

	printf("\nKindly enter one of the filename from above including extension\n");
	
	bzero( mmo, sizeof(mmo));
	
	gets(mmo);
	send(sockfd,mmo, MAXSZ, 0);

	char NamaFile[40];
	strcpy(NamaFile, "/home/"); 
	strcat(NamaFile, usr); 
	strcat(NamaFile, "/serverFile/");
	    
	strcat(NamaFile, mmo);

	FILE *fp;
    	fp = fopen(NamaFile, "r"); 

	bzero( mmo, sizeof(mmo));
	int nread = fread(mmo,1,256,fp);
    	bfreceiv = recv(sockfd, mmo, MAXSZ, 0);
    	fwrite(mmo,1,bfreceiv,fp);

}


else if(!strcmp(mmo, "2"))
{
	
printf("Write down a filename to be downloaded to the [clientFile]\n\n" );

	//gets(mmo);
	//send(sockfd,mmo, MAXSZ, 0);
	bzero( mmo, sizeof(buffer));
	recv(sockfd, mmo, MAXSZ, 0);
	printf("\n%s\n", mmo); 
	gets(mmo);
	send(sockfd,mmo, MAXSZ, 0);
	
   	char NamaFile[40];
    	strcpy(NamaFile, "/home/"); 
    	strcat(NamaFile, usr); 
    	strcat(NamaFile, "/clientFile/");
    	strcat(NamaFile, mmo);
    
    	FILE *fp;
    	fp = fopen(NamaFile, "ab"); 

		if(NULL == fp)
    	{
        	printf("Error opening file");
        
    	}
    
    	bzero( mmo, sizeof(mmo));
    
    	bfreceiv = recv(sockfd, mmo, MAXSZ, 0);
    	fwrite(mmo,1,bfreceiv,fp);
   
}



else if(!strcmp(mmo, "3"))
{
	printf("Please key in the Directory that attempt to create: ");
	scanf("%s", cr8);

	/* set the path/name of the directory that want to create */ 
	char cr8Direc[40]; 
	strcpy(cr8Direc, "/home/"); 
	strcat(cr8Direc, usr); 
	strcat(cr8Direc, "/"); 
	strcat(cr8Direc, cr8);

	/* Check the path exist or not, if not, create one */ 
	struct stat s; 
	if(stat(cr8Direc, &s) == -1){ 
	mkdir(cr8Direc, 0700); } 
}


else if(!strcmp(mmo, "4"))
{
	printf("Please key in the specified Directory to remove it: ");
	scanf("%s", dlt);

	/* set the path of the directory that want to delete */ 
	char dltDirec[40]; 
	strcpy(dltDirec, "/home/"); 
	strcat(dltDirec, usr); 
	strcat(dltDirec, "/"); 
	strcat(dltDirec, dlt);

	
	char slctSubDirec[60];
	strcpy(slctSubDirec, "exec rm -r ");
	strcat(slctSubDirec, "/home/"); 
	strcat(slctSubDirec, usr); 
	strcat(slctSubDirec, "/"); 
	strcat(slctSubDirec, dlt);
	strcat(slctSubDirec, "/*"); 
        /* select all the files inside the directory that want to remove */
	
	struct stat s; 
	if(stat(dltDirec, &s) != -1){
	system(slctSubDirec);
	rmdir(dltDirec); } 
        /* Checking that the specified path exist or not, if exist, delete it */ 
}

}while (strcmp(mmo, "/q"));
close(sockfd);
return 0;
}

void catchin(int signo){
	printf("\n[ Interruption found in the signal.....]\n");
}

