// Client side C/C++ program to demonstrate Socket programming
Client.c 
	#include <stdlib.h>
#include <netinet/in.h>
#include<stdio.h> //printf
#include<string.h> //strcpy
#include<sys/socket.h>
#include<netdb.h> //hostent
#include<arpa/inet.h>
#define PORT 8080  
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
char hello[20];
printf("please input the data:");
    scanf("%s",&hello);
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) //建立客户端的套接字，在IPv4域中，TCP
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.206.135"); //指定要连接的ip为192.168.206.135，这是我设置的Server端的IP地址
    if(inet_pton(AF_INET, "192.168.206.135", &serv_addr.sin_addr)<=0) //将点分十进制的ip地址转化为用于网络传输的数值格式 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
}

// connect（）系统调用将由文件描述符sockfd引用的套接字连接到由addr指定的地址。Client和Server连接上了
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
