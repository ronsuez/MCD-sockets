/*
 ============================================================================
 Name        : main.c
 Author      : Ronald Suez
 Version     :
 Copyright   : Your copyright notice
 Description : Socket Client
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>

void error(char * message){
	perror(message);

	exit(1);
}

int main(void) {

	int socketfd, portno, n;
	struct sockaddr_in serv_addr;
	char buffer[256];
	portno = atoi("2002");
	socketfd = socket(AF_INET,SOCK_STREAM,0);
	if(socketfd < 0)
		error("Error creating socket");


	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(portno);

	if(connect(socketfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		error("Error connecting to Server");

	while(1){
		printf("Please, type your message: ");
		bzero(buffer,256);

		scanf("%s",buffer);


		n = write(socketfd,buffer,strlen(buffer));
		if(n < 0 )
			error("Error writing to socket \n");
		else
			printf("...Message sent \n");



		n = read(socketfd,buffer,sizeof(buffer));
		if(n < 0 )
				error("Error writing to socket \n");
			else
				printf("\n [Length %lu] Message from server: %s \n",strlen(buffer),buffer);

		if(!strcmp("quit",buffer)){
							close(socketfd);
							printf("The connection has been closed \n");
							break;
						}

	}




	return EXIT_SUCCESS;
}
