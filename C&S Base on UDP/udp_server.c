#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 128

void error_handle(char *msg);

int main(int argc, char*argv[])
{
	int serv_sock;
	int str_len;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	
	char message[BUF_SIZE];
	memset(message, 0, sizeof(message));
	
	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(serv_sock == -1)
		error_handle("UDP socket creation error!");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handle("bind() error!");

	while(1)
	{
		clnt_addr_size = sizeof(clnt_addr);
		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		
		if(!strcmp(message, "code:mayday"))
			break;
		
		sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_addr, clnt_addr_size);
	}
	
	close(serv_sock);
	return 0;
}

void error_handle(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

