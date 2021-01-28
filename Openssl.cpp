#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<string.h>
#include <stdio.h>
#include <netdb.h>
#include<iomanip>
#include <fstream>
#include <openssl/ssl.h>
#include <openssl/err.h>
using namespace std;

#define MESSAGE_LEN 1024


int main(int argc, char const *argv[])
{
    setlocale(LC_ALL,"zh-CN");//显示中文
    int socket_fd;
    string url;
    int ret=0;
    string sendbuf;
    unsigned char recvbuf[MESSAGE_LEN];
    FILE *fq=NULL;
    int len;


    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        cout << "Failed to create socket!" << endl;
    }//创建SOCKET
    
    url="c-ssl.duitang.com";
    struct hostent *website_host = NULL;
	website_host = gethostbyname(url.c_str());//通过域名返回IP地质
    	if (website_host == NULL)
	{
		cout<<"gethostbyname error";
	}
    struct sockaddr_in website_addr;
	bzero((void*)&website_addr, sizeof(website_addr));
	website_addr.sin_family = AF_INET;//对TCP/IP
	website_addr.sin_port = htons(443);//https选择443端口
	website_addr.sin_addr.s_addr = ((struct in_addr *)(website_host->h_addr))->s_addr;

    if(connect(socket_fd, (struct sockaddr*)&website_addr, sizeof(website_addr))!=0)
    {
        perror("Connect ");
        exit(errno);
    }
    //建立连接
    sendbuf=sendbuf+"GET /uploads/blog/202008/09/20200809084621_yjrjg.jpg HTTP/1.1\r\n"
                   +"Host: "+url.c_str()+"\r\n"
                   +"Accept: image/*\r\n"
                   +"User-Agent: Mozilla/4.0(compatible)\r\n"
                   +"connection:Keep-Alive\r\n"
                   +"\r\n\r\n";

    //初始化ssl

    ERR_load_BIO_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_CTX *sslContext = SSL_CTX_new(SSLv23_client_method());
    if (sslContext == NULL)
    {
        ERR_print_errors_fp(stderr);
        return false;
    }
    SSL *sslHandle = SSL_new(sslContext);
    if (sslHandle == NULL)
    {
        ERR_print_errors_fp(stderr);
        return false;
    }
    if (!SSL_set_fd(sslHandle, socket_fd))
    {
        ERR_print_errors_fp(stderr);
        return false;
    }
    if (SSL_connect(sslHandle) != 1)
    {
        ERR_print_errors_fp(stderr);
        return false;
    }

    SSL_write(sslHandle, sendbuf.c_str(), sendbuf.length());
    ofstream fout("tu1.jpg");
    while((ret=SSL_read(sslHandle, recvbuf,100)) > 0)
       {

            recvbuf[ret] = '\0';
            fout<<recvbuf;
            memset(recvbuf, 0, sizeof(recvbuf));

        }


    fout.close();
    //ofileAgain.close();
    close(socket_fd);
    SSL_shutdown(sslHandle);
    SSL_free(sslHandle);
    SSL_CTX_free(sslContext);

    return 0;
}


