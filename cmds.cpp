#include "common.h"
#include <iostream>

cmd::cmd_table cmds_money;
void cmds::init()
{
	cmd::add("help", &cmds::money::help, "хелп", 0, &cmds_money);
	cmd::add("баланс", &cmds::money::balanse, "мой баланс", 0, &cmds_money);
	cmd::add("лотерея", &cmds::money::lotery, "лотерея", 0, &cmds_money);
}

table cmds::money::help(message::msg msg, table rmsg)
{
	rmsg["message"]+="команды:<br>";
	for(auto cmd_temp: cmds_money)
	{
		rmsg["message"]+="<br> - ";
		rmsg["message"]+=str::low(cmd_temp.first);
		rmsg["message"]+=" ";
		rmsg["message"]+=cmd_temp.second.info;
	}
	return rmsg;
}

table cmds::money::main(message::msg msg, table rmsg)
{
	if(2 > msg.words.size())
	{
		msg.words.push_back("help");
	}
	rmsg=cmd::start(msg, rmsg, str::low(msg.words[1]), &cmds_money);
	return rmsg;
}

table cmds::money::balanse(message::msg msg, table rmsg)
{
	rmsg["message"] += "баланс: ";
	rmsg["message"] += to_string(module::money::get(other::getId(msg)));
	return rmsg;
}

#define PERSENT 60
table cmds::money::lotery(message::msg msg, table rmsg)
{
	if(3 > msg.words.size())
	{
		msg.words.push_back("0");
	}
	long long int m = str::fromString(msg.words[2]);
	if(m < 1 || m > module::money::get(other::getId(msg)))
	{
		rmsg["message"] += "ошибка(<br>используйте лотерея <ставка><br>ну или у вас недостаточно $";
		return rmsg;
	}
	if(rand() % 100 > PERSENT)
	{
		m = 0 - m;
		rmsg["message"] += "Ты проиграл чо(";
	}
	else rmsg["message"] += "Красава респект и уважуха";
	module::money::add(other::getId(msg), m);
	return rmsg;
}

table cmds::test(message::msg msg, table rmsg) //example
{
	net::send("api.vk.com");
	rmsg["message"]+="¯\\_(ツ)_/¯";
	return rmsg;
}

table cmds::servers(message::msg msg, table rmsg)
{
	rmsg["message"]+="Серва:<br><br>";
	json servers = json::parse(net::send("http://194.67.198.141/api/ms_server.php"));
	for(auto ss: servers)
	{
		if(!ss["info"].is_boolean())
		{
			rmsg["message"] += "<br>";
			rmsg["message"] += ss["info"]["host"];
			rmsg["message"] += " (";
			rmsg["message"] += ss["info"]["gamedir"];
			rmsg["message"] += ") ";
			rmsg["message"] += to_string((int)ss["info"]["ping"]);
			rmsg["message"] += "ms <br>";
			rmsg["message"] += ss["info"]["map"];
			rmsg["message"] += " ";
			rmsg["message"] += ss["ip"];
			rmsg["message"] += ":";
			rmsg["message"] += to_string((int)ss["port"]);
			rmsg["message"] += " ";
			rmsg["message"] += ss["info"]["numcl"];
			rmsg["message"] += "/";
			rmsg["message"] += ss["info"]["maxcl"];
			rmsg["message"] += "<br>#####################";
		}
	}
	return rmsg;
}

table cmds::nick(message::msg msg, table rmsg) //example
{
	if(msg.words.size() < 2)
	{
		rmsg["message"]+="ты забыл написать новый ник";
		return rmsg;
	}
	module::name::set(other::getId(msg), str::summ(msg.words, 1));
	rmsg["message"]+="сменил)";
	return rmsg;
}

table cmds::video(message::msg msg, table rmsg)
{
	if(msg.words.size() < 2)
	{
		rmsg["message"]+="а чо ввести запрос для поиска эт лишнее? Я чо Ванга?";
		return rmsg;
	}
	table params;
	params["q"]+=str::summ(msg.words, 1);
	params["adult"] = "1";
	params["count"] = "200";
	params["hd"] = "1";
	params["sort"] = "2";
	json res = vk::send("video.search", params)["response"]["items"];
	if(res.size()==0)
	{
		rmsg["message"]+="нетю такого(";
		return rmsg;
	}
	rmsg["attachment"]="";
	for(int i = 0; i < 10; i++)
	{
		int index = rand() % res.size();
		rmsg["attachment"]+="video";
		rmsg["attachment"]+=to_string((int)res[index]["owner_id"]);
		rmsg["attachment"]+="_";
		rmsg["attachment"]+=to_string((int)res[index]["id"]);
		rmsg["attachment"]+=",";
	}
	rmsg["message"]+="воть<br>всего:";
	rmsg["message"]+=to_string(res.size());
	return rmsg;
}