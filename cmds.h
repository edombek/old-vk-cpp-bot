#pragma once
namespace cmds
{
	void init();
	table test(message::msg msg, table rmsg); //example
	table servers(message::msg msg, table rmsg); 
	table nick(message::msg msg, table rmsg);
	table video(message::msg msg, table rmsg);
	table phrase(message::msg msg, table rmsg);
	table f(message::msg msg, table rmsg);
	table doc(message::msg msg, table rmsg);
	table con(message::msg msg, table rmsg);
	namespace money
	{
		table help(message::msg msg, table rmsg);
		table main(message::msg msg, table rmsg);
		table balanse(message::msg msg, table rmsg);
		table lotery(message::msg msg, table rmsg);
	}
}