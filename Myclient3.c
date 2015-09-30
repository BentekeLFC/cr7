//The ECHO client using the specified sockets

/*

TTP3121 TCP/IP Programming(T1 2015/16) 
FILE : Myclient3.c
        
GROUP MEMBERS:
	
1) DANIEL BRIAN THOMPSON	1122700901
2) HABINASH BALAKRISHAN	        1122700044 

*/

#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <time.h>
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[2000] , server_reply[2000];
         char ttime[1000];
	FILE *file1;

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //server.sin_addr.s_addr = inet_addr("127.0.0.1");
   // server.sin_addr.s_addr = inet_addr(argv[1]);    //passing server address from user
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
    
     
    //keep communicating with server
    
 



    while(1)
    {
        
        bzero(message, sizeof(message)); //buffer empty

        printf("Enter message : ");
        gets(message);
        

	time_t rawtime;
            struct tm * timeinfo;

  		time ( &rawtime );
 		timeinfo = localtime ( &rawtime );
		strcpy(ttime,  asctime (timeinfo));
		strcat(message," ");
       
		strcat(message,ttime);
       
         
        //Send some data

        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
	else{
		file1 = fopen("./clientHistory.txt", "a");
		
		fputs(message, file1);
		fclose(file1);	
	}
         
        //Receive a reply from the server

        
        bzero(server_reply, sizeof(server_reply));   //buffer empty 
       

        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        else
        {
        puts("Server reply :");
        puts(server_reply);
	file1 = fopen("./clientHistory.txt", "a");
		
fputs(server_reply, file1);
	fclose(file1);

        }
        

    }
     
    close(sock);
    return 0;
}

