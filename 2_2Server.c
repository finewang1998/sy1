#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#define    MAXLINE        1024  
int main(int argc,char **argv)   
{  
    struct sockaddr_in     serv_addr;  
    struct sockaddr_in     clie_addr;  
    char                   buf[MAXLINE];  
    int                    sock_id;  
    int                    link_id;  
    int                    recv_len;  
    int                    write_leng;  
    int                    clie_addr_len;  
    FILE                   *fp;  
  
    if (argc != 3) 
    {  
        printf("usage :%s portnum filename\n", argv[0]);  
        exit(0);
    }  
/*<-----------------------------------------socket---------------------------------------------->*/
    if ((sock_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {  
        perror("Create socket failed\n");  
        exit(0);  
    }  
/*<-----------------------------------------bind------------------------------------------------->*/
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_port = htons(atoi(argv[1]));  //atoi()将argv[1]的字符端口号形式转换为整型
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
  
//创建套接字后，bind函数将套接字绑定到addr中指定的地址和端口号
    if (bind(sock_id, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ) 
    {  
        perror("Bind socket failed\n");  
        exit(0);  
    }  
/*<-----------------------------------------listen----------------------------------------------->*/
    if (-1 == listen(sock_id, 10)) 
    {  
        perror("Listen socket failed\n");  
        exit(0);  
    }  
/*<-------------------------------------server receive part---------------------------------->*/
    while (1) {  
        //”a+“ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留）
        if ((fp = fopen(argv[2], "a+")) == NULL) 
        {  
            perror("Open file failed\n");  
            exit(0);  
        }  
        clie_addr_len = sizeof(clie_addr); 
/*<-----------------------------------------accept----------------------------------------------->*/        
        link_id = accept(sock_id, (struct sockaddr *)&clie_addr, &clie_addr_len);  //一个套接口接受的一个连接。本函数从Server的等待连接队列中抽取第一个连接，创建一个与Server同类的新的套接口并返回句柄。
        if (-1 == link_id) {  
            perror("Accept socket failed\n");  
            exit(0);  
        }  
        bzero(buf, MAXLINE);  
        while (recv_len = recv(link_id, buf, MAXLINE, 0)) //如果Server的发送缓冲中没有数据或者数据被协议成功发送完毕后，recv先检查套接字Server的接收缓冲区，如果Server接收缓冲区中没有数据或者协议正在接收数据，那么recv就一直等待，直到协议把数据接收完毕。当协议把数据接收完毕，recv函数就把Server的接收缓冲中的数据copy到buf中(注意协议接收到的数据可能大于buf的长度，所以在这种情况下要调用几次recv函数才能把s的接收缓冲中的数据copy完。recv函数仅仅是copy数据，真正的接收数据是协议来完成的);返回值为copy的字节数
        {  
            if(recv_len < 0) 
            {  
                printf("Recieve Data From Server Failed!\n");  
                break;  
            }  
            printf("#");  
            write_leng = fwrite(buf, sizeof(char), recv_len, fp);  //将接受的文件复制到本地
            if (write_leng < recv_len) 
            {  
                printf("Write file failed\n");  
                break;  
            }  
            bzero(buf,MAXLINE);  
        }  
        printf("\nFinish Recieve\n");  
        fclose(fp);  
        close(link_id);  
    }  
    close(sock_id);   
    return 0;  
}  
