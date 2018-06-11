/*================================================================
*   Copyright (C) 2017 Guangzhou GYT Ltd. All rights reserved.
*   
*   文件名称：unixstr_serv.c
*   创 建 者：luhuadong
*   创建日期：2017年10月23日
*   描    述：
*
================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UNIXSTR_PATH "/tmp/unix.str"
#define LISTENQ 5
#define BUFFER_SIZE 256

int main(void)
{
	int listenfd, connfd;
	socklen_t len;
	struct sockaddr_un servaddr, cliaddr;

	if(-1 == (listenfd = socket(AF_LOCAL, SOCK_STREAM, 0)))
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	unlink(UNIXSTR_PATH);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);
	if(-1 == bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	listen(listenfd, LISTENQ);

	len = sizeof(cliaddr);

	if(-1 == (connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)))
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	char buf[BUFFER_SIZE];

	while(1)
	{
		bzero(buf, sizeof(buf));
		if(read(connfd, buf, BUFFER_SIZE) == 0) break;
		printf("Receive: %s", buf);
	}

	close(listenfd);
	close(connfd);
	unlink(UNIXSTR_PATH);

	return 0;
}



