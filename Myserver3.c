// The C socket server 

/*

TTP3121 TCP/IP Programming(T1 2015/16) 
FILE : Myserver3.c
        
GROUP MEMBERS:
	
1) DANIEL BRIAN THOMPSON	1122700901
2) HABINASH BALAKRISHAN	        1122700044 

*/

 
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <time.h>
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    char ttime[1000];
	FILE *file1;

     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
     
    //Receive a message from client
           
      

    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        
        //Send the message back to client
        puts(client_message);

	file1 = fopen("./serverHistory.txt", "a");
	
	fputs(client_message, file1);
	fclose(file1);	
	
        printf("Enter Message : ");
        gets(client_message);

	time_t rawtime;
            struct tm * timeinfo;

  		time ( &rawtime );
 		timeinfo = localtime ( &rawtime );
  		
		strcpy(ttime,  asctime (timeinfo));

	
	strcat(client_message," ");
	
	strcat(client_message,ttime);

        write(client_sock , client_message , strlen(client_message));
         file1 = fopen("./serverHistory.txt", "a");
		
		fputs(client_message, file1);
		fclose(file1);

 
        bzero(client_message, sizeof(client_message));  
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
     
    return 0;
}

