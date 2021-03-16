#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <fstream>

using namespace std;

#define MESSAGE_LEN 1200

int main(int argc, char const *argv[])
{
  setlocale(LC_ALL,"zh-CN");//显示中文
  int socket_fd;
  string url;
  int ret=-1;
  char sendbuf[MESSAGE_LEN];
  char recvbuf[MESSAGE_LEN];
  int contentL=9362;//信息长度

  memset(recvbuf, 0, MESSAGE_LEN);

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

  ret = connect(socket_fd, (struct sockaddr*)&website_addr, sizeof(website_addr));//建立连接

  url="m.biaoqingb.com";
  sprintf(sendbuf, "GET /uploads/img1/20200622/cb4f78a788f721b1cb11e67928059305.jpg!300X300.jpg HTTP/1.1\r\n");
  strcat(sendbuf, "Host: ");
  strcat(sendbuf,url.c_str());
  strcat(sendbuf, "\r\n");
  strcat(sendbuf, "Accept: */*\r\n");
  strcat(sendbuf, "Accept-Encoding: identity\r\n");
  strcat(sendbuf, "User-Agent: curl/7.54.0\r\n");
  strcat(sendbuf, "connection:Keep-Alive\r\n");
  strcat(sendbuf, "\r\n\r\n");       
  send(socket_fd, sendbuf, strlen(sendbuf), 0);
  memset(recvbuf, 0, 1200);

  int total=0;//字节总长
  int i;
  int flag=0;
  ofstream fout("test.jpg", ios::app);

  while(total<contentL)
  {
    i=0;
    ret = recv(socket_fd, recvbuf, sizeof(recvbuf), 0);

    if(flag==0)
    {    
      for( i=3 ; i<sizeof(recvbuf) ; i++)
      {
       if(recvbuf[i]=='\n'&&recvbuf[i-1]=='\r'&&recvbuf[i-2]=='\n'&&recvbuf[i-3]=='\r')
        {
        flag=1;
        i++;
        break;
        }
      }
    }// 跳过响应头消息

    for( ; i<sizeof(recvbuf) && total<contentL ; i++)
    {
      fout<<recvbuf[i];//将字节写入文件
      total++;//计算字节数
    }

  }

  fout.close();
  close(socket_fd);
  return 0;
}



  