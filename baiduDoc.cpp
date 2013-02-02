/*By Hacksign*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <iostream>
#include <string>

using namespace std;

#define TRUE 1
#define FALSE 0
#define bool int

string data;

size_t curlCallBack(void *ptr, size_t size, size_t nmemb, string *stream){
	stream->append((const char *)ptr, size*nmemb);

	return size*nmemb;
}

bool getData(const char * address){
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, address);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlCallBack);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		return TRUE;
	}

	return FALSE;
}
void parPageInfo(int *sPage, int *ePage, int *tPage){
  /*
	 *cout<<data.substr(0,70)<<endl;
   */
	string::size_type sIndex = data.find_first_of(":") + 2;
	string::size_type eIndex = data.find("\"", sIndex);
	*tPage = atoi(data.substr(sIndex, eIndex - sIndex).c_str());
  /*
	 *cout<<*tPage<<endl;
   */

	sIndex = data.find(":", eIndex) + 1;
	eIndex = data.find("\"", sIndex + 1);
	*sPage = atoi(data.substr(sIndex + 1, eIndex - sIndex).c_str());
  /*
	 *cout<<*sPage<<endl;
   */

	sIndex = data.find(":", eIndex) + 1;
	eIndex = data.find("\"", sIndex + 1);
	*ePage = atoi(data.substr(sIndex + 1, eIndex - sIndex).c_str());
  /*
	 *cout<<*ePage<<endl;
   */
}
void parSwfInfo(int sPage, int ePage, int tPage){
	string::size_type sIndex;
	string::size_type eIndex = 0;

	while(sPage <= ePage){
		sIndex = data.find("CWS\t", eIndex);
		eIndex = data.find("CWS\t", sIndex + 4);
		if(eIndex == string::npos){
			eIndex = data.length();
		}
//construct page num
//page num is a string like this "01 02 03"
		char num[10] = {0},*p;
		p = num;
		int countTotal = 1, countCurrent = 1;
		int tmpSPage = sPage, tmpTotal = tPage;
		while(tmpTotal / 10 != 0){
			countTotal++;
			tmpTotal /= 10;
		}
		while(tmpSPage / 10 != 0){
			countCurrent++;
			tmpSPage /= 10;
		}

		for(int i = 0; i < countTotal - countCurrent; i++){
			strcpy(p++,"0");
		}
		sprintf(p++, "%d", sPage);
		strcat(p, ".swf");
		FILE *fp = fopen(num, "wb+");
		fwrite(data.substr(sIndex, eIndex - sIndex).c_str(), eIndex - sIndex, 1, fp);
		fclose(fp);

		sPage++;
	}
}
string parArg(char **argv){
	string address(argv[1]);
	string::size_type dotIndex = address.rfind(".");
	string::size_type eIndex = address.rfind("/");
	return address.substr(eIndex + 1, dotIndex - eIndex - 1);
}

int main(int argc,char ** argv){
	bool bRet;
	cout<<"Download articles form Baidu.\nsupported type:*.pdf *.doc *.ppt\nhttp://www.hacksign.cn\n\t\tby Hacksign\n==========================="<<endl;
	if(argc != 2){
		cout<<"usage:"<<argv[0]<<" <address of article>"<<endl;
		exit(0);
	}
	string md5 = parArg(argv);
	char url[] = "http://ai.wenku.baidu.com/play/%s?pn=%d&rn=5";
	char address[500] = {0};
	int startPage,endPage = 1,totalPage;

	do{
		sprintf(address, url, md5.c_str(), endPage);
    /*
		 *cout<<address<<endl;
     */
		getData((const char *)address);
		parPageInfo(&startPage, &endPage, &totalPage); 
		parSwfInfo(startPage, endPage, totalPage);
		endPage++;
		data.clear();
	}while(endPage <= totalPage);

	return 0;
}
