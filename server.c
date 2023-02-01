#include	<sys/socket.h>
#include	<netinet/in.h>
#include   	<arpa/inet.h>
#include 	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<errno.h>
#include 	<unistd.h>
#include	<sys/types.h>
#include 	<sys/select.h>
#include	<time.h>
#include	<ctype.h>
#define MAXLINE 8192
#define	LISTENQ	1024
#define MAX(a,b) (((a)>(b))?(a):(b))

void capitalize_TCP(int connfd){

	int retVal;
	ssize_t n;
	char buffer[MAXLINE], message[MAXLINE];

	memset(&message, 0 , sizeof(message));
        while ( (n = read(connfd, message, MAXLINE)) > 0 ) {
		message[n] = 0;
		printf("Message From TCP client: %s", message);
		memset(&buffer, 0, sizeof(buffer));
		for (int i = 0; i< sizeof(message) ; i++){
			buffer[i]= toupper(message[i]); }
		printf("Buffer to TCP client: %s\n", buffer);
		retVal = write(connfd, buffer , strlen(buffer));
	        if (retVal < 0) perror("write error"); }
	if (n < 0) perror("read error");
}


int main( int argc, char ** argv ) {
	int listenfd, b_retVal1, b_retVal2 , l_retVal, retVal, connfd, udpfd, ready, maxfdp1;
	char buffer[MAXLINE], message[MAXLINE];
	pid_t pid;
	fd_set rset;
	ssize_t n,m;
	socklen_t len;
	struct sockaddr_in cliaddr, servaddr;

	/* create listening TCP socket */
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) perror("main: TCP socket error");

	memset(&cliaddr, 0, sizeof(cliaddr));
	memset(&servaddr,0 ,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	// binding server addr structure to listenfd
	b_retVal1 = bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if (b_retVal1 < 0) perror("main: TCP bind error");

	l_retVal = listen(listenfd, LISTENQ);
	if (l_retVal < 0) perror("main: listen error");

	/* create UDP socket */
	udpfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpfd < 0) perror("UDP socket error");

	// binding server addr structure to udp sockfd
	b_retVal2 = bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        if (b_retVal2 < 0) perror("main: UDP bind error");

	// clear the descriptor set
	FD_ZERO(&rset);

	for (;;) {
		// set listenfd and udpfd in readset
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);
	        // get max fd
		maxfdp1 = MAX(listenfd, udpfd) + 1;

		// select the ready descriptor
		ready = select(maxfdp1, &rset, NULL, NULL, NULL);

		// if tcp socket is readable then handle it by accepting the connection
		if (FD_ISSET(listenfd, &rset)) {
			len = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
			if (connfd < 0) {perror("main: accept error"); printf("not accepted\n");}
			if ((pid = fork()) == 0) {
				retVal = close(listenfd);
				if (retVal < 0) perror("main: child close error");
				capitalize_TCP(connfd);
				close(connfd);
				exit(0);
			}
			close(connfd);
		}
		// if udp socket is readable receive the message.
		if (FD_ISSET(udpfd, &rset)) {
			len = sizeof(cliaddr);
			memset(&message,0 ,sizeof(message));
			n = recvfrom(udpfd, message, sizeof(message), 0,(struct sockaddr*)&cliaddr, &len);
			if(n < 0) perror("recvfrom error");
			message[n] = 0;
			printf("Message from UDP client: %s", message);
			memset(&buffer, 0, sizeof(buffer));
                	for (int i = 0; i<sizeof(message) ; i++){
                        	buffer[i]= toupper(message[i]); }
                	printf("Buffer to UDP client: %s\n", buffer);
			m = sendto(udpfd, (const char*)buffer, strlen(buffer), 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr));
			if (m < 0) perror("sendto error");
	}
}
close(listenfd);
return 0; }