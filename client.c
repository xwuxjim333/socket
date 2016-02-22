#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char * argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in server_addr;
	struct hostent *server;
	char buffer[256];
	if (argc < 3) {
		fprintf(stderr, "usage: %s hostname port\n", argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, nosuch host\n");
		exit(1);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
	server_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		error("ERROR connecting");

	printf("Enter message to send to host: ");
	memset(buffer, 0, 256);
	fgets(buffer, 255, stdin);
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0)
		error("ERROR writing to socket");
	
	memset(buffer, 0, 256);
	n = read(sockfd, buffer, 255);
	if (n < 0)
		error("ERROR reading from socket");
	
	printf("Message from host: %s\n", buffer);

	return 0;
}
