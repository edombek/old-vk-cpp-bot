#pragma once
namespace other
{
	void sleep(int ms);
	void startTime();
	string getTime();
	string getRealTime();
	string getId(message::msg msg);
	json jsonDifferenceArr(json j1, json j2);
}