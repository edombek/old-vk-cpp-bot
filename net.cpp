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

string net::send(string url, table param)
{
	string paramline = "";
	table *params = &param;
	for (auto iter = params->begin(); iter != params->end(); iter++) {
		paramline += iter->first + "=" + urlEncode(iter->second) + "&";
	}
	return net::send(url, paramline);
}

string buffer;
int writer(char *data, size_t size, size_t nmemb, string *buffer)
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
	//cout << url << "-" << params << "-" << buffer << endl;
	if (result == CURLE_OK)
		return buffer;
	return "";
}

string net::upload(string url, string filename, string params)
{
	buffer ="";
	CURL *curl;
	CURLcode res;
	struct stat file_info;
	double speed_upload, total_time;
	FILE *fd;
	fd = fopen(filename.c_str(), "rb"); /* open file to upload */
	if(fstat(fileno(fd), &file_info) != 0)
		return "";
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READDATA, fd);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, net_agent);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		else {
			curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
			curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
			fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n", speed_upload, total_time);
		}
		curl_easy_cleanup(curl);
	}
	fclose(fd);
	return buffer;
}
	