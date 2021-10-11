// This is a simple HTTP Client written in c

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
#include <netdb.h>


#define	SA	struct sockaddr
#define BUF_SIZE 100000

void GET(int sockfd, char *path) {

  char req[1000] = {0};

  sprintf(req, "GET %s HTTP/1.0\r\n\r\n", path);

  send(sockfd, req, strlen(req), 0);

}


int main(int argc, char *argv[]){

	int sockfd, n , sendbytes ;
	struct sockaddr_in servaddr;

	char buf[BUF_SIZE];

	struct in_addr *inetaddrp [2];
	struct in_addr inetaddr;
	struct in_addr **pptr;
	struct hostent *hp;

	FILE *fp;
 	char *filename = "file";


	if (argc != 5){

		printf("Required arguments for client : <IP Address> <PORT> <Method> <path> \n");

		return 1;
	}

	
	if ( ( hp = gethostbyname( argv[1]) ) == NULL){

		if (inet_aton (argv [1], &inetaddr) == 0) {

			printf("hostname error for %s: %s", argv [1] );

			} else {

			inetaddrp [0] = &inetaddr;
			inetaddrp [1] = NULL;
			pptr = inetaddrp;

			}

		} else {

			pptr = (struct in_addr **) hp->h_addr_list;

		}
		

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){

		printf("socket error \n");
		return 1;

	}

	printf("[+][+]Successfully created socket \n");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons((unsigned short)strtoul(argv[2], NULL, 0));
	memcpy (&servaddr.sin_addr, *pptr, sizeof (struct in_addr) ) ;

	printf ("[+][+]trying %s\n", inet_ntop (AF_INET, &servaddr.sin_addr, buf, sizeof(buf)));


// The connect function is used by a TCP clientto establish a conenction with a TCP Server. 

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0 ) {

		printf("Connect error \n");
		return 1; 

	}

	printf("[+][+]Successfully connected .....!! \n");
	
	if(!strcmp(argv[3],"GET")){	
	
 		 GET(sockfd, argv[4]);

		 printf("[+][+]GET Request Sent successfully....! \n");

 	 	 while (recv(sockfd, buf, BUF_SIZE, 0) > 0) {

 	   	 fputs(buf, stdout);
 	   	 memset(buf, 0, BUF_SIZE);

		
	 	 }


	} else if(!strcmp(argv[3],"PUT")){

		char req[1000] = {0};

		sprintf(req, "PUT %s HTTP/1.0\r\n\r\n", argv[4]);
		send(sockfd, req, strlen(req), 0);
		memset(req, 0, BUF_SIZE);

		fp = fopen(filename, "r");
		
		fgets(buf, BUF_SIZE, fp);
		send(sockfd, buf, strlen(buf), 0);
		memset(buf, 0, BUF_SIZE);
	
		printf("[+][+]File data sent successfully.\n");

		recv(sockfd, buf, BUF_SIZE, 0);
		fputs(buf, stdout);
 	   	memset(buf, 0, BUF_SIZE);

	} else
	
		printf("[-][-]Method should be either PUT or GET \n");
	
	exit(0);

}
