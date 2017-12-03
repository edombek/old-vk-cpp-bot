#pragma once

namespace net
{
	void init();
	string c_nettest(vector<string> cmd_args);
	string urlEncode(string str);
	size_t writeToBuffer(char *ptr, size_t size, size_t nmemb, void *data);
	size_t writeToFile(char *ptr, size_t size, size_t nmemb, void *data);
	string send(string url, const char *post = nullptr, fs::file *_file = nullptr);
	string send(string url, table* params);
	string send(string url, table params);
	int upload(string file, string url);
};