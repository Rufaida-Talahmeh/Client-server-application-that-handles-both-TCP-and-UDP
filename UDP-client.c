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
	int sockfd;
	char buff[MAXLINE];
 	struct sockaddr_in servaddr;
 	socklen_t addr_size;
	ssize_t x,y;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) perror("socket error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	servaddr.sin_port = htons(atoi(argv[2]));

	addr_size = sizeof(servaddr);

  	while(1){
		printf("Please write text you want to capitalize:\n");
		if(fgets(buff,sizeof(buff),stdin) != NULL){
    			x = sendto(sockfd,buff,strlen(buff),0,(struct sockaddr *)&servaddr,addr_size);
    			if (x < 0) perror("sendto error");
		}
		else break;
    		y = recvfrom(sockfd,buff,sizeof(buff),0,NULL, NULL);
                if (y < 0) perror("recvfrom error");
    		buff[y] = 0;
		printf("Received from server: %s\n",buff);
	}
  	exit(0);
  	return 0;
}