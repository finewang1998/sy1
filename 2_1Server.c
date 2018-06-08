// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080     //设置端口号为8080
int main(int argc, char const *argv[])
{
int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[1024] = {0};
char hello[20] ="Hello from server";//若待会儿在客户机上接通并收到消息就会给客户机发送此消息。
// 创建套接字的文件描述符
// socket()函数的原型如下，这个函数建立一个协议族为domain、协议类型为type、协议编号为protocol的套接字文件描述符。如果函数调用成功，会返回一个标识这个套接字的文件描述符，失败的时候返回-1。
//SOCKET_STREAM（流式套接字）: TCP连接，提供序列化的、可靠的、双向连接的字节流。支持带外数据传输
//AF_INET：Ipv4网络协议
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) //生成一socket,失败则退出
{
perror("socket failed"); // perror(s) 用来将上一个函数发生错误的原因输出到标准设备(stderr)。
exit(EXIT_FAILURE);
}
//把建好的套接字连接到端口8080
//这个套接字选项通知内核，如果端口忙，但TCP状态位于TIME_WAIT ，可以重用端口。如果端口忙，而TCP状态位于其他状态，重用端口时依旧得到一个错误信息，指明"地址已经使用中"。
if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
&opt, sizeof(opt)))
{
perror("setsockopt");
exit(EXIT_FAILURE);
}
address.sin_family = AF_INET; //指定通信域为IPv4
address.sin_addr.s_addr = INADDR_ANY; //指定地址为0.0.0.0的地址，这个地址事实上表示不确定地址，或"所有地址"、"任意地址"。 一般来说，在各个系统中均定义成为0值。
address.sin_port = htons( PORT ); //指定端口号为8080

// Forcefully attaching socket to the port 8080
if (bind(server_fd, (struct sockaddr *)&address,
sizeof(address))<0) //创建套接字后，bind函数将套接字绑定到addr中指定的地址和端口号。 
{
perror("bind failed");
exit(EXIT_FAILURE);
}
if (listen(server_fd, 3) < 0) //将服务器套接字置于被动模式，等待客户端接近服务器以建立连接。3，是backlog, 定义了sockfd挂起的连接队列可能增长的最大长度。 如果连接请求在队列满时到达，则客户端可能会收到错误并显示ECONNREFUSED。
{
perror("listen");
exit(EXIT_FAILURE);
}
if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
(socklen_t*)&addrlen))<0) //提取监听套接字的挂起连接队列中的第一个连接请求，sockfd，创建一个新的连接套接字，并返回一个引用该套接字的新文件描述符。 此时，客户端和服务器之间建立连接，并准备好传输数据。
{
perror("accept");
exit(EXIT_FAILURE);
}
valread = read( new_socket , buffer, 1024); //读取客户端传来的字符，放在buffer里
printf("%s\n",buffer );
send(new_socket , hello , strlen(hello) , 0 ); //把自己这边的字符传过去以达到交互目的。
printf("Server message sent\n");
return 0;
}
