#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 128

void error_handle(char *msg);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in from_addr;
	int str_len;
	int oprn_cnt;
	int i;
	socklen_t adr_sz;
	
	char message[BUF_SIZE];
	memset(message, 0, sizeof(message));
	
	if(argc != 3)
	{
		printf("Usage :%s <IP> <Port>\n", argv[0]);
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error_handle("UPD socket creation error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	
	while(1)
	{
		fputs("Insert message(q to qiut):", stdout);
		fgets(message, sizeof(message), stdin );
		
		if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		adr_sz = sizeof(from_addr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_addr, sizeof(from_addr));
		message[str_len] = 0;
		printf("Message from server:%s", message);
	}
	
	close(sock);
	return 0;
}

void error_handle(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
