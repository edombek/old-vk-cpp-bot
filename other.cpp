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