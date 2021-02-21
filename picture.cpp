#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <iomanip>
#include <fstream>
#include <curl/curl.h>

using namespace std;

#define MESSAGE_LEN 1024


int main(int argc, char const *argv[])
{
    setlocale(LC_ALL,"zh-CN");//显示中文
    int socket_fd;
    string url;
    int ret=-1;
    char sendbuf[MESSAGE_LEN];
    unsigned char recvbuf[MESSAGE_LEN];
    FILE *fq;
    int len;


    socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd == -1)
    {
        cout << "Failed to create socket!" << endl;
    }//创建SOCKET
    

    url="m.biaoqingb.com";
    struct hostent *website_host = NULL;
	website_host = gethostbyname(url.c_str());//通过域名返回IP地质
    	if (website_host == NULL)
	{
		cout<<"gethostbyname error";
	}
    struct sockaddr_in website_addr;
	bzero((void*)&website_addr, sizeof(website_addr));
	website_addr.sin_family = AF_INET;//对TCP/IP
	website_addr.sin_port = htons(80);//选择80端口
	website_addr.sin_addr.s_addr = ((struct in_addr *)(website_host->h_addr))->s_addr;

    ret = connect(socket_fd, (struct sockaddr*)&website_addr, sizeof(website_addr));
    //建立连接

  url="m.biaoqingb.com";
  cout<<url.c_str();
  sprintf(sendbuf, "GET /uploads/img1/20201001/9cac1f8ad56166a5c89857cc6de4de1e.jpg HTTP/1.1\r\n");
	strcat(sendbuf, "Host: ");
	strcat(sendbuf,url.c_str());
	strcat(sendbuf, "\r\n");
	strcat(sendbuf, "Accept: */*\r\n");
  strcat(sendbuf, "Accept-Encoding: identity\r\n");
	strcat(sendbuf, "User-Agent: curl/7.54.0\r\n");
	strcat(sendbuf, "connection:Keep-Alive\r\n");
	strcat(sendbuf, "\r\n\r\n");       
  send(socket_fd, sendbuf, sizeof(sendbuf), 0);
  ofstream fout("test1.jpg");
  memset(recvbuf, 0, sizeof(recvbuf));
  while (1)
  {
      ret = recv(socket_fd, recvbuf, sizeof(recvbuf), 0);
      if (ret <= 0)
      {
          break;
      }
      cout<<recvbuf; 
        
  }

  fout.close();
  close(socket_fd);

  return 0;
}

