#pragma once
namespace message
{
	typedef struct{
		json msg;
		args words;
	} msg;
	void load(json dat);
	bool isChat(message::msg msg);
	json send(table msg);
	table postTR(table rmsg);
	bool toMe(string word, args names);
}