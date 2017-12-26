#include "common.h"
#include <iostream>
cmd::cmd_table cmd_d;

string cmd::helpList(cmd::cmd_table *cmd_temp, message::msg msg)
{
	string out = "";
	out+="команды:<br>";
	for(auto cmds: (*cmd_temp))
	{
		if(!cmds.second.disp&!module::admin::get(other::getId(msg)))continue;
		if(!module::admin::get(other::getId(msg))&cmds.second.admin_cmd) continue;
		out+="<br> - \"";
		out+=str::low(cmds.first);
		out+="\" - ";
		out+=cmds.second.info;
		if(cmds.second.cost)
		{
			out+=" - ";
			out+=to_string(cmds.second.cost);
			out+="$";
		}
	}
	return out;
}

table help(message::msg msg, table rmsg)
{
	rmsg["message"]+=cmd::helpList(&cmd_d, msg);
	return rmsg;
}

void cmd::init()
{
	cmd::add("help", &help, false);
	cmd::add("помощь", &help, false);
	cmd::add("$", &cmds::money::main, true, "валюта");
	cmd::add("тест", &cmds::test, true, "тест на пинг", 0); //example
	cmd::add("ник", &cmds::nick, true, "смена ника");
	cmd::add("видосы", &cmds::video, true, "видосы", 5);
	cmd::add("f", &cmds::f, false, "видосы с правками)", 10);
	cmd::add("-f", &cmds::f, false, "видосы с правками)", 10);
	cmd::add("доки", &cmds::doc, true, "доки", 50);
	cmd::add("con", &cmds::con, true, "консолька)", 0, &cmd_d, true);
	cmd::add("кто", &cmds::who, true, "ыыы", 1);
	cmd::add("u", &cmds::upload, true, "выгрузить в контач", 0, &cmd_d, true);
	cmd::add("цитата", &cmds::citata, true, "создать цитату", 5, &cmd_d, false);
	cmd::add("арт", &cmds::art, true, "создать арт", 5, &cmd_d, false);
	cmd::add("exe", &cmds::execute, true, "vk execute", 0, &cmd_d, true);
	cmd::add("погода", &cmds::weather, true, "погодка", 2, &cmd_d, false);
	cmds::init();
}
void cmd::add(string command, cmd::msg_func func, bool disp, string info, int cost, cmd::cmd_table *cmd_temp, bool admin_cmd)
{
	if(cmd_temp==NULL)cmd_temp=&cmd_d;
	(*cmd_temp)[str::low(command)].func=func;
	(*cmd_temp)[str::low(command)].disp=disp;
	(*cmd_temp)[str::low(command)].info=info;
	(*cmd_temp)[str::low(command)].cost=cost;
	(*cmd_temp)[str::low(command)].admin_cmd=admin_cmd;
}
table cmd::start(message::msg msg, table rmsg, string command, cmd::cmd_table *cmd_temp)
{
	if(cmd_temp==NULL)cmd_temp=&cmd_d;
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