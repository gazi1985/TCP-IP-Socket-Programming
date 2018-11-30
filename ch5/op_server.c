#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RTL_SIZE 4
#define OPSZ	 4

void error_handle(char *msg);
int getOperResult(int opr_cnt, int oprnds[], char opr);

int main(int argc, char*argv[])
{
	int serv_sock;
	int clnt_sock;
	int rev_len;
	int rev_cnt;
	int oprn_cnt;
	int clnt_result;
	int i;	

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
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
	{
		error_handle("sock() error!");
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handle("bind() error!");
	}
	
	if(listen(serv_sock, 5) == -1)
	{
		error_handle("listen() error!");
	}
	
	clnt_addr_size = sizeof(clnt_addr);
	for(i = 0; i < 5; ++i)
	{
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	    if(clnt_sock == -1)
            error_handle("accept() error!");
		else
			printf("Connected client %d \n", i + 1);
		
		oprn_cnt = 0;
		read(clnt_sock, &oprn_cnt, 1);
		rev_len = 0;
		printf("Receive client:%d operaor count:%d \n", i, oprn_cnt);
		while(rev_len < (oprn_cnt * OPSZ +1))
		{
			rev_cnt = read(clnt_sock, &message[rev_len], BUF_SIZE - 1);
			rev_len += rev_cnt;
			printf("Receive:%d total:%d \n", rev_cnt, rev_len);
		}
		printf("Begin caculate... \n");
		clnt_result = getOperResult(oprn_cnt, (int*)message, message[rev_len - 1]);
		write(clnt_sock, (char*)clnt_result, sizeof(clnt_result));
		close(clnt_sock);
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

int getOperResult(int opr_cnt, int oprnds[], char opr)
{
	int result = oprnds[0], i;
	switch(opr)
	{
		case '+':
		for(i = 0; i < opr_cnt; ++i) result += oprnds[i];
		break;
		case '-':
		for(i = 0; i < opr_cnt; ++i) result -= oprnds[i];
		break;
		case '*':
		for(i = 0; i < opr_cnt; ++i) result *= oprnds[i];
		break;
	}	
	return result; 
}
