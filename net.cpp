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

/* Nettest command */
string net::c_nettest(std::vector<std::string> cmd_args)
{
	if (cmd_args.size() != 2)
		return "Use \"nettest <url>\"";
	string ret = net::send(cmd_args[1]);
	return ret;
}

/* Encode string to url */
string net::urlEncode(string str)
{
	char* esc_text = curl_easy_escape(NULL, str.c_str(), str.length());
	string result = esc_text;
	
	curl_free(esc_text);
	return result;
}

/* Write data to string buffer */
size_t net::writeToBuffer(char *ptr, size_t size, size_t nmemb, void *data)
{
	((string*)data)->append(ptr);
	return size * nmemb;
}

/* Write data to file */
size_t net::writeToFile(char *ptr, size_t size, size_t nmemb, void *data)
{
	((fs::file*)data)->writeBinary(ptr, nmemb);
	return size * nmemb;
}

/* Send request and return data or write to file */
string net::send(string url, const char *post, fs::file* _file)
{
	CURL* curl = curl_easy_init();
	if (curl) {
		string* memory = nullptr;
		if(_file == nullptr)
			memory = new string();

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, net_agent);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		if (_file == nullptr) {
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)memory);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, net::writeToBuffer);
		}
		else {
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)_file);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, net::writeToFile);
		}
		if (post != nullptr) {
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
		}
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		if (_file != nullptr)
			return nullptr;
		string data = *memory;
		delete memory;
		//cout << data;
		return data;
	}
	return "...";
}

/* Send for parametrs */
string net::send(string url, table* params)
{
	// Build paramline
	string paramline = "";
	for (auto iter = params->begin(); iter != params->end(); iter++) {
		paramline += iter->first + "=" + urlEncode(iter->second) + "&";
	}
	// Send
	return net::send(url, paramline.c_str());
}

/* Send for static params */
string net::send(string url, table params)
{
	return net::send(url, &params);
}

int net::upload(string file, string url)
{
  CURL *curl;
  CURLcode res;
  struct stat file_info;
  double speed_upload, total_time;
  FILE *fd;

  fd = fopen(file.c_str(), "rb"); /* open file to upload */
  if(!fd)
    return 1; /* can't continue */

  /* to get the file size */
  if(fstat(fileno(fd), &file_info) != 0)
    return 1; /* can't continue */

  curl = curl_easy_init();
  if(curl) {
    /* upload to this place */
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    /* tell it to "upload" to the URL */
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* set where to read from (on Windows you need to use READFUNCTION too) */
    curl_easy_setopt(curl, CURLOPT_READDATA, fd);

    /* and give the size of the upload (optional) */
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)file_info.st_size);

    /* enable verbose for easier tracing */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    }
    else {
      /* now extract transfer info */
      curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
      curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

      fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n",
              speed_upload, total_time);

    }
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  fclose(fd);
  return 0;
}
