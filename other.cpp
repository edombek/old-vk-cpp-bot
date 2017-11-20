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
	return ctime(&temp);
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