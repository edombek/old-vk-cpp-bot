#pragma once
namespace cmds
{
	void init();
	table test(message::msg msg, table rmsg); //example
	table nick(message::msg msg, table rmsg);
	table video(message::msg msg, table rmsg);
	table f(message::msg msg, table rmsg);
	table doc(message::msg msg, table rmsg);
	table con(message::msg msg, table rmsg);
	table who(message::msg msg, table rmsg);
	table upload(message::msg msg, table rmsg);
	table execute(message::msg msg, table rmsg);
	table citata(message::msg msg, table rmsg);
	table art(message::msg msg, table rmsg);
	table weather(message::msg msg, table rmsg);
	table unicode(message::msg msg, table rmsg);
	table ban(message::msg msg, table rmsg);
	table unban(message::msg msg, table rmsg);
	namespace money
	{
		table help(message::msg msg, table rmsg);
		table main(message::msg msg, table rmsg);
		table balanse(message::msg msg, table rmsg);
		table lotery(message::msg msg, table rmsg);
		table top(message::msg msg, table rmsg);
		table send(message::msg msg, table rmsg);
	}
}