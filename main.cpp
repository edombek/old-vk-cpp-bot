#include "common.h"
#include <iostream>
int main()
{
	net::init();
	cmd::init();
	vk::start();
	module::start();
	other::startTime();
	lp::loop();
	return 0;
}