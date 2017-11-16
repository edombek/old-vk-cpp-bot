#include "common.h"
#include <iostream>
cmd::cmd_table cmd_d;

table help(message::msg msg, table rmsg)
{
	rmsg["message"]+="команды:<br>";
	for(auto cmd_temp: cmd_d)
	{
		if(!module::admin::get(other::getId(msg))&cmd_temp.second.admin_cmd) continue;
		rmsg["message"]+="<br> - ";
		rmsg["message"]+=str::low(cmd_temp.first);
		rmsg["message"]+=" ";
		rmsg["message"]+=cmd_temp.second.info;
	}
	return rmsg;
}

void cmd::init()
{
	cmd::add("help", &help, "да, да это то что вы видите");
	cmd::add("$", &cmds::money::main,  "валюта");
	cmd::add("тест", &cmds::test, "тест", 0, &cmd_d, true); //example
	cmd::add("маста", &cmds::servers, "сервера хача3д");
	cmd::add("ник", &cmds::nick,  "смена ника");
	cmd::add("видосы", &cmds::video,  "видосы", 10);
	cmds::init();
}
void cmd::add(string command, cmd::msg_func func,string info, int cost, cmd::cmd_table *cmd_temp, bool admin_cmd)
{
	if(cmd_temp==NULL)cmd_temp=&cmd_d;
	(*cmd_temp)[str::low(command)].func=func;
	(*cmd_temp)[str::low(command)].info=info;
	(*cmd_temp)[str::low(command)].cost=cost;
	(*cmd_temp)[str::low(command)].admin_cmd=admin_cmd;
}
table cmd::start(message::msg msg, table rmsg, string command, cmd::cmd_table *cmd_temp)
{
	if(cmd_temp==NULL)cmd_temp=&cmd_d;
	cout << &cmd_d << " " << cmd_temp << " '" << (*cmd_temp).find(command)->first << "'" << endl;
	if((*cmd_temp).find(command)->first != "")
	{
		if(module::money::get(other::getId(msg))<(*cmd_temp)[command].cost)
		{
			rmsg["message"] += "чот баблишка тебе не хватает";
			return rmsg;
		}
		if(!module::admin::get(other::getId(msg))&(*cmd_temp)[command].admin_cmd)
		{
			rmsg["message"] += "эээ куды полез?";
			return rmsg;
		}
		module::money::add(other::getId(msg), 0-(*cmd_temp)[command].cost);
		rmsg = (*cmd_temp)[command].func(msg,rmsg);
	}
	else
	{
		rmsg["message"]+="нипонимаю";
		rmsg["message"]+="(";
		rmsg["message"]+=command;
		rmsg["message"]+=")";
	}
	return rmsg;
}