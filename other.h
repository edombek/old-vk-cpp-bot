#pragma once
namespace other
{
	void sleep(int ms);
	void startTime();
	string getTime();
	string getRealTime();
	string getId(message::msg msg);
	json jsonDifferenceArr(json j1, json j2);
	void fwds(json *in, json *out, unsigned int lvl=0);
}