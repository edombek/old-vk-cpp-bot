#include <curl/curl.h>
#include "common.h"
#include<iostream>

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
	if (!esc_text) throw runtime_error("Can not convert string to URL");
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

		CURLcode code = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		if (code != CURLE_OK)
		{
			cout << "CURL (" << code << "): " << curl_easy_strerror(code) << endl;
			throw new net::exception(curl_easy_strerror(code), code);
		}

		if (_file != nullptr)
			return nullptr;
		string data = *memory;
		delete memory;
		//cout << data << endl;
		return data;
	}
	throw new net::exception("curl not created", -1);
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

/* CLASS EXCEPTION */
net::exception::exception(string awhat, int acode)
{
	this->what_str = awhat;
	this->code = acode;
}

string net::exception::what()
{
	return to_string(this->code)+" | "+this->what_str;
}