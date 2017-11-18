#include "common.h"
#include <iostream>
#include <sstream>
#include <chrono>

#define out 0x02

std::chrono::time_point<std::chrono::system_clock> start, stop;

#define PERSENT 10

void message::load(json dat)
{
	start = std::chrono::system_clock::now();
	table params;
	message::msg msg;
	table rmsg;
	msg.msg=dat;
	char flags = (int)msg.msg[2];
	if(msg.msg.is_null() || flags & out)return;
	rmsg["peer_id"] = to_string((int)msg.msg[3]);
	if(message::isChat(msg))
	{
		if(rand()%100000+1<1000*PERSENT)
		{
			rmsg["message"]=module::phrase::get();
			message::send(rmsg);
		}
		rmsg["forward_messages"] = to_string((int)msg.msg[1]);
	}
	msg.words = str::words(msg.msg[5]);
	if(!msg.words.size())return;
	if(message::isChat(msg) && !message::toMe(msg.words[0], botname)) return;
	if((message::toMe(msg.words[0], botname))) msg.words.erase(msg.words.begin());
	if(1 > msg.words.size())
	{
		msg.words.push_back("help");
	}
	rmsg = module::TR(msg, rmsg);
	rmsg = cmd::start(msg, rmsg, str::low(msg.words[0]));
	rmsg = module::postTR(msg, rmsg);
	rmsg = message::postTR(rmsg);
	message::send(rmsg);
}

bool message::isChat(message::msg msg)
{
	return (int)msg.msg[3] > 2000000000;
}

json message::send(table msg)
{
	return vk::send("messages.send", msg, true);
}

table message::postTR(table rmsg)
{
	rmsg["message"] += "<br><br>Запущен: ";
	rmsg["message"] += other::getTime();
	rmsg["message"] += "Сейчас: ";
	rmsg["message"] += other::getRealTime();
	rmsg["message"] += "Обрабатывалось: ";
	stop = std::chrono::system_clock::now();
	rmsg["message"] += to_string(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count());
	rmsg["message"] += "мс";
	return rmsg;
}

bool message::toMe(string word, args names)
{
	if(word[word.size()-1] == ',') word.resize(word.size()-1);
	for(unsigned int i = 0; i < names.size(); i++)
	{
		if(str::low(names[i]) == str::low(word)) return true;
	}
	return false;
}