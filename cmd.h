#pragma once

namespace cmd
{
	typedef table(*msg_func)(message::msg, table); // rmsg func(msg, rmsg)
	typedef struct{
		string info;
		cmd::msg_func func;
		int cost;
	}cmd_dat;
	typedef map<string, cmd_dat> cmd_table;
	
	void init();
	void add(string command, cmd::msg_func func,string info="", int cost=0, cmd::cmd_table *cmd_temp=NULL);
	table start(message::msg msg, table rmsg, string command, cmd::cmd_table *cmd_temp=NULL);
}