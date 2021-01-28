#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
FILE *fp;
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
int written=0;
written= fwrite(ptr, size, nmemb, (FILE*)stream);
return written;
}


int main()
{
int ret;
CURL *curl=curl_easy_init();
CURLcode res;
FILE *file = NULL;
char HttpBuf[200];
file = fopen("tu.jpg", "wb");
if(curl)
{
    curl_easy_setopt(curl, CURLOPT_URL,"https://c-ssl.duitang.com/uploads/blog/202008/09/20200809084621_yjrjg.jpg");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)file);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    res = curl_easy_perform(curl);
    if((res != CURLE_OK) && (res != CURLE_WRITE_ERROR))
    {
        printf("%s curl error no=%d\n", __FUNCTION__, res);
        return -1;
    }
}

curl_easy_cleanup(curl);
//fclose(file);
return 0;
}
