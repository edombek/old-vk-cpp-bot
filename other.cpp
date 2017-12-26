#include "common.h"
#include <unistd.h>
#include <ctime>

void other::sleep(int ms)
{
	usleep(ms*1000);
}

time_t td;
void other::startTime()
{
	td=time(NULL);
}

string other::getTime()
{
	return ctime(&td);
}

string other::getRealTime()
{
	time_t temp = time(NULL);
	string str = ctime(&temp);
	str.resize(str.size()-1);
	return str;
}

string other::getTime(time_t temp)
{
	string str = ctime(&temp);
	str.resize(str.size()-1);
	return str;
}

string other::getId(message::msg msg)
{
	string id;
	if(msg.msg[6]["from"].is_null()) id = to_string((int)msg.msg[3]);
	else id = msg.msg[6]["from"];
	return id;
}

json other::jsonDifferenceArr(json j1, json j2)
{
	json out;
	for(unsigned int i1 = 0; i1 < j1.size(); i1++)
	{
		bool f = true;
		for(unsigned int i2 = 0; i2 < j2.size(); i2++)
		{
			if(j1[i1]==j2[i2])
			{
				f = false;
				break;
			}
		}
		if(f) out.push_back(j1[i1]);
	}
	return out;
}

void other::fwds(json *in, json *out, unsigned int lvl)
{
	for(auto i: *in)
	{
		json t;
		t["msg"]=i["body"];
		t["user_id"]=i["user_id"];
		t["lvl"]=lvl;
		out->push_back(t);
		if(i["fwd_messages"].is_null())continue;
		other::fwds(&i["fwd_messages"], out, lvl+1);
	}
}