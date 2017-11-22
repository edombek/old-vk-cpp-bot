#pragma once

namespace cmd
{
	typedef table(*msg_func)(message::msg, table); // rmsg func(msg, rmsg)
	typedef struct{
		string info;
		cmd::msg_func func;
		int cost;
		bool admin_cmd;
		bool disp;
	}cmd_dat;
	typedef map<string, cmd_dat> cmd_table;
	
	void init();
	void add(string command, cmd::msg_func func, bool disp=false, string info="", int cost=0, cmd::cmd_table *cmd_temp=NULL, bool admin_cmd = false);
	table start(message::msg msg, table rmsg, string command, cmd::cmd_table *cmd_temp=NULL);
	string helpList(cmd::cmd_table *cmd_temp, message::msg msg);
}