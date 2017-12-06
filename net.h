#pragma once

namespace net
{
	void init();
	string urlEncode(string str);
	string send(string url, string params = "");
	string send(string url, table param);
	string upload(string url, string filename, string params = "");
	void download(string url, string filename, string params = "");
};