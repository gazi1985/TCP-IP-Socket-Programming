#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handle(char *msg);
#define BUF_SIZE 30

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];
    int read_cnt;
    FILE *fp;

    if (argc != 3)
        printf("Usage :%s <IP> <Port>\n", argv[0]);
    fp = fopen("receive.dat", "wb");
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handle("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handle("connect error");
    else
        printf("Connected..... \n");

    while ((read_cnt = read(sock, buf, BUF_SIZE))!=0)
    {
        fwrite((void*)buf, 1, read_cnt, fp);
		printf("Receive:-%d", read_cnt);
    }

    puts("Received file finished!...\n");
    write(sock, "Thank you", 10);

    fclose(fp);
    close(sock);
    return 0;
}

void error_handle(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
