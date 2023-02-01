#include	<sys/socket.h>
#include	<netinet/in.h>
#include   	<arpa/inet.h>
#include 	<netinet/in.h>
#include 	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<errno.h>
#include 	<unistd.h>
#define MAXLINE 8192


int main(int argc, char ** argv){
	int sockfd, c_retVal, retVal;
	char buff[MAXLINE];
 	struct sockaddr_in servaddr;
 	socklen_t addr_size;
	ssize_t n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) perror("socket error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	servaddr.sin_port = htons(atoi(argv[2]));

	addr_size = sizeof(servaddr);

	c_retVal = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	if (c_retVal < 0) perror("main: connect error");

  	while(1){
		printf("Please write text you want to capitalize:\n");
		if(fgets(buff,sizeof(buff),stdin) != NULL){
			retVal = write(sockfd, buff, strlen(buff));
                        if (retVal < 0) perror("write error");
		}
		else break;
    		n = read(sockfd, buff, MAXLINE);
		if (n < 0) perror("write error");
    		buff[n] = 0;
		printf("Received from server: %s\n",buff);
	}
  	exit(0);
  	return 0;
}