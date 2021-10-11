//This is a simple HTTP Web server written in c 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/file.h>
#include <fcntl.h>

#define	LISTENQ	1024	
#define	SA struct sockaddr
#define MAXLINE 100000


void resolve( int f){

	  char buff[MAXLINE]; 
	  char *method;
	  char *filename;
	  char req[1000] = {0};

	  recv(f, buff, MAXLINE, 0);

	  if (strcmp(buff,"GET /index.html HTTP/1.0\r\n\r\n") == 0){ 

		sprintf(req, "200 OK HTTP/1.0\r\n\r\n");
		send(f, req, strlen(req), 0);

		FILE *filename = fopen("index.html", "r");
		
		while(fgets(buff, MAXLINE, filename)){

			send(f, buff, strlen(buff), 0);					
			memset(buff, 0, MAXLINE);

		}

		printf("[+][+]File sent \n");
		exit (1);
	
	
	  } else  if (strcmp(buff,"GET /hello.html HTTP/1.0\r\n\r\n") == 0){ 

	
		sprintf(req, "200 OK HTTP/1.0\r\n\r\n");
		send(f, req, strlen(req), 0);

		
		FILE	*filename = fopen("hello.html", "r");	

	       	while(fgets(buff, MAXLINE, filename)){

			send(f, buff, strlen(buff), 0);					

			memset(buff, 0, MAXLINE);

        	 }

		printf("[+][+]File sent \n");
		exit (1);
		
    	  } else if (strcmp(buff,"GET /home.html HTTP/1.0\r\n\r\n") == 0){ 


		sprintf(req, "200 OK HTTP/1.0\r\n\r\n");
		send(f, req, strlen(req), 0);
	
		FILE	*filename = fopen("home.html", "r");
		
		while(fgets(buff, MAXLINE, filename)){

			send(f, buff, strlen(buff), 0);					

			memset(buff, 0, MAXLINE);

		}

		printf("[+][+]File sent \n");
		exit (1);

       	}else if (strcmp(buff,"PUT /file HTTP/1.0\r\n\r\n") == 0){ 

			sprintf(req, "200 OK File created HTTP/1.0\r\n\r\n");
			send(f, req, strlen(req), 0);
		
  			FILE *fp;
 			char *filename = "file";

  			fp = fopen(filename, "w");

			recv(f, buff, MAXLINE, 0);
			fprintf(fp, "%s", buff);

   			bzero(buff, MAXLINE);

			printf("[+][+]File received and saved.....! \n");
			

			exit (1);

	}else {

			sprintf(req, "404 NOT FOUND HTTP/1.0\r\n\r\n");
			send(f, req, strlen(req), 0);

			FILE	*filename = fopen("404.html", "r");
		
			while(fgets(buff, MAXLINE, filename)){

			send(f, buff, strlen(buff), 0);					
			memset(buff, 0, MAXLINE);


		}
	
	} 
	
}

int main(int argc, char *argv[])
{

	int 			listenfd, connfd, n;
	pid_t 			childpid;
	socklen_t 		clilen;
	struct sockaddr_in 	cliaddr, servaddr;
	uint8_t    		buff[MAXLINE+1];
	uint8_t			recvline[MAXLINE+1];


	clilen = sizeof(cliaddr);

// To perform network I/O the first thing a process must do is call the socket() function, specifying the type of communication protocol

	if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {	 

		fprintf(stderr, "[-][-]Socket creation failed \n");
		exit(1);

	}
		printf("[+][+]Socket creation successful \n");		

		bzero(&servaddr,sizeof(servaddr));			
		
		servaddr.sin_family 		= AF_INET;				
		servaddr.sin_addr.s_addr 	= htonl (INADDR_ANY);		
		servaddr.sin_port 		= htons((unsigned short)strtoul(argv[1], NULL, 0)); 

// The bind() function assigns a local protocol address to the socket.
    
	if ((bind(listenfd,(SA *) &servaddr, sizeof(servaddr))) < 0){
			
		fprintf(stderr,"[-][-]bind failed \n");
		exit(1);
		
	}  

	printf("[+][+]Successfully bound \n");		

// The listen() function is called only by a TCP Server and it converts an unconnected socket in to a passive,
// indicating that the kernel should accept incoming connection requests directed to this socket. 

	if (listen(listenfd, LISTENQ) < 0) {
	
		fprintf(stderr, "[-][-]listen failed \n");
		exit(1);
		
	}  

		printf("[+][+]Listening\n");

// The accept function is called by the TCP Server to return the enxt completed conenction from the front of the completed queue.

	for ( ; ; ) 
   	{
		if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen) ) < 0 ){
		
			fprintf(stderr, "[-][-]accept failed\n");

			exit(1);
				
		}	
// The fork() function is used to create a new process so that each process can handle a new client.
// When a connection is established, accept returns, the server calls fork and the child process services the client on the connected socket. 
	
		if (( childpid =fork()) == 0)

		{
			close(listenfd);	        		
			resolve(connfd);
			close(connfd);		

  		        exit(0);

		}

	}			
// The close() function is also used to close a socket and terminate a TCP connection	
// The shutdown() function can be used to send a FIN on a TCP connection
	close(connfd);
	return 0;

}

