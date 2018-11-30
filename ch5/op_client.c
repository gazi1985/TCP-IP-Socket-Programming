#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handle(char *msg);
#define BUF_SIZE 1024
#define RTL_SIZE 4
#define OPSZ	 4

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	int result;
	int oprn_cnt;
	int i;
	
	char buf[BUF_SIZE];
	//memset(buf, 0, sizeof(buf));
	
	if(argc != 3)
	{
		printf("Usage :%s <IP> <Port>\n", argv[0]);
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handle("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handle("connect error");
	else
		printf("Connected Oper Server..... \n");
	
	//Get Operand count
	fputs("OPerand count: " , stdout);
	scanf("%d", &oprn_cnt);
	
	buf[0] = (char)oprn_cnt;
	for(i = 0; i < oprn_cnt; ++i)
	{
		printf("Operator %d :", i + 1);
		scanf("%d", (int*)&buf[OPSZ * i + 1]);
	}
	
	fgetc(stdin);
	fputs("Operator: ", stdout);
	scanf("%c", &buf[oprn_cnt * OPSZ + 1]);
	write(sock, &buf, oprn_cnt * OPSZ + 2);
	read(sock, &result, RTL_SIZE);
	
	printf("Operation result: %d \n", result);
	close(sock);
	return 0;
}

void error_handle(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
