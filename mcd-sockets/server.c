/*
 ============================================================================
 Name        : main.c
 Author      : Ronald Suez
 Version     :
 Copyright   : Your copyright notice
 Description : Socket Server
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

char  response[255] = "I Got your message my beloved client \n";

void error(char * message){
	perror(message);

	exit(1);
}

void handleClient(int newsocketfd){

	 printf("\n Handling a new client \n");
		int n;
		char buffer[256];


			while(1){

				bzero(buffer,256); //Cleaning the buffer

				if((n = read(newsocketfd,buffer,sizeof(buffer))) > 0){
						//buffer[n] = '\0';
						printf("(Mlength %i: %lu) message from Client:  %s\n",n,strlen(buffer),buffer);

				}

				if(!strcmp("quit",buffer)){
						   strcpy(response,"quit");
							printf("Closing connection \n");
							break;
					}

					if(write(newsocketfd,response,sizeof(response)) > 0)
								printf("Writing response to Client socket \n");
					else
								error("Error writing message \n");


			}

}

int main(void) {

	/*
	 * Variable's description
	 * socketfd: socket file descriptor
	 * newsocketfd: new socket file descriptor
	 * portno : port number
	 * clilen : client length
	 * buffer : buffer array
	 * serv_addr: server address
	 * cli_addr: client address
	 */
	int socketfd, newsocketfd, portno,pidchild;
	unsigned int  clilen;


	struct sockaddr_in serv_addr, cli_addr;



	printf("\n --Servidor TCP-- \n");


	/*
	 * Creating the socketfd
	 * family AF_INET: IPV4 Protocols
	 * type SOCK_STREAM: Stream Socket / SOCK_DGRAM: Datagram socket
	 * protocol 0: default protocol (depends on family/type combination)
	 */
	 if((socketfd = socket(AF_INET, SOCK_STREAM,0)) < 0)
		 error("Error opening Socket");
	 else
		 printf("the socket descriptor has been created \n");

	 //Clear the complete structure
	 bzero((char *) &serv_addr, sizeof(serv_addr));

	 //Initalizing the server struct
	 portno = atoi("2002");
	 serv_addr.sin_family = AF_INET;
	 serv_addr.sin_addr.s_addr = INADDR_ANY;
	 serv_addr.sin_port = htons(portno);

	 //binding the socket to the server struct
	 if(bind(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		 	 error("Error Binding socket \n");
	 	else
	 		printf("Listening at %d \n",portno);

	 //Listening for N connections
	  listen(socketfd,5);
	  clilen = sizeof(cli_addr);


	while(1){

			 if((newsocketfd = accept(socketfd,(struct sockaddr *) &cli_addr, &clilen)) < 0)
				 error("\n Error on Accept \n");
			 //From here, the execution should be continue using a process or thread child.
			 //Create child process

			 if((pidchild = fork()) < 0 ){
				 error("\n Error creating child process \n");
			 }else if( pidchild == 0){//The child process
				 	 close(socketfd);
				 	 handleClient(newsocketfd);
				 	 exit(0);
			 }else if(pidchild > 0){//the Parent process

				 close(newsocketfd);
				 printf("\n Waiting for the next client \n");
			 }
	}




	return EXIT_SUCCESS;
}
