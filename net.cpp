#include <curl/curl.h>
#include "common.h"
#include<iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#define net_agent "EVGESHAd agent"

/* Init network settings */
void net::init()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

string net::urlEncode(string str)
{
	char* esc_text = curl_easy_escape(NULL, str.c_str(), str.length());
	string result = esc_text;
	
	curl_free(esc_text);
	return result;
}

string net::send(string url, table param, bool post)
{
	string paramline = "";
	table *params = &param;
	for (auto iter = params->begin(); iter != params->end(); iter++) {
		paramline += iter->first + "=" + urlEncode(iter->second) + "&";
	}
	if(post)
		return net::send(url, paramline);
	return net::send(url+"?"+paramline);
}

string buffer;
size_t writer(char *data, size_t size, size_t nmemb, string *buffer)
{
	int result = 0;
	if (buffer != NULL)
	{
		buffer->append(data, size * nmemb);
		result = size * nmemb;
	}
	return result;
}

string net::send(string url, string params)
{
	buffer ="";
	CURL *curl;
	CURLcode result;
    curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, net_agent);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		if (params != "") {
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.c_str());
		}
		result = curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);
	cout << endl << other::getRealTime() << ": " << url << "-" << params << endl << "	" << buffer << endl;
	if (result == CURLE_OK)
		return buffer;
	return "";
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}

string net::upload(string url, string filename, string params)
{
	buffer ="";
	CURL *curl;
	CURLcode result;
    curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, net_agent);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		if (params != "") {
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.c_str());
		}
		struct curl_httppost *formpost=NULL;
		struct curl_httppost *lastptr=NULL;
		curl_formadd(&formpost,
									&lastptr,
									CURLFORM_COPYNAME, "file",
									CURLFORM_FILENAME, filename.c_str(),
									CURLFORM_FILE, filename.c_str(),
									CURLFORM_CONTENTTYPE, "multipart/form-data",
									CURLFORM_END);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		result = curl_easy_perform(curl);
	}
	curl_easy_cleanup(curl);
	//cout << endl << other::getRealTime() << ": " << url << "-" << params << endl << "	" << buffer << endl;
	if (result == CURLE_OK)
		return buffer;
	return "";
}

void net::download(string url, string filename, string params)
{
	CURL *curl;
	FILE *file = NULL;
    curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, net_agent);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		if (params != "") {
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.c_str());
		}
		file = fopen(filename.c_str(), "wb");
		if(file){
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
			curl_easy_perform(curl);
			fclose(file);
		}
	}
	curl_easy_cleanup(curl);
	return;
}