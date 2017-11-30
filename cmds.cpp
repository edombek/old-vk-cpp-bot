#include "common.h"
#include <iostream>
#include <stdlib.h>

cmd::cmd_table cmds_money;
void cmds::init()
{
	cmd::add("help", &cmds::money::help, false, "хелп", 0, &cmds_money);
	cmd::add("баланс", &cmds::money::balanse, true, "мой баланс", 0, &cmds_money);
	cmd::add("лотерея", &cmds::money::lotery, true, "лотерея", 0, &cmds_money);
}

table cmds::money::help(message::msg msg, table rmsg)
{
	rmsg["message"]+=cmd::helpList(&cmds_money, msg);
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

table cmds::nick(message::msg msg, table rmsg)
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
	table params =
	{
		{"q", str::summ(msg.words, 1)},
		{"adult", "1"},
		{"count", "200"},
		{"hd", "1"},
		{"sort", "2"}
	};
	json res = vk::send("video.search", params)["response"]["items"];
	args videos;
	for(unsigned int i = 0; i < res.size(); i++)
	{
		if(res[i]["id"].is_null()) continue;
		string temp = "";
		temp+="video";
		temp+=to_string((int)res[i]["owner_id"]);
		temp+="_";
		temp+=to_string((int)res[i]["id"]);
		videos.push_back(temp);
	}
	if(videos.size()==0)
	{
		rmsg["message"]+="нетю такого(";
		return rmsg;
	}
	rmsg["attachment"]="";
	unsigned int index = 0;
	if(videos.size()>10)
		index = rand() % (videos.size() - 10);
	for(unsigned int i = index; i < videos.size(); i++)
	{
		rmsg["attachment"]+=videos[i];
		rmsg["attachment"]+=",";
	}
	rmsg["message"]+="воть<br>всего:";
	rmsg["message"]+=to_string(videos.size());
	return rmsg;
}

table cmds::phrase(message::msg msg, table rmsg)
{
	if(msg.words.size() < 2)
	{
		rmsg["message"]+="я чо Ванга?";
		return rmsg;
	}
	module::phrase::add(str::summ(msg.words, 1));
	rmsg["message"]+="добавил)";
	return rmsg;
}

table cmds::f(message::msg msg, table rmsg)
{
	if(msg.words.size() < 2)
	{
		rmsg["message"]+="а чо ввести запрос для поиска эт лишнее? Я чо Ванга?";
		return rmsg;
	}
	table params =
	{
		{"q", str::summ(msg.words, 1)},
		{"adult", "1"},
		{"count", "200"},
		{"hd", "1"},
		{"sort", "2"}
	};
	json res1 = vk::send("video.search", params)["response"]["items"];
	params["adult"] = "0";
	json res2 = vk::send("video.search", params)["response"]["items"];
	json res = other::jsonDifferenceArr(res1, res2);
	args videos;
	for(unsigned int i = 0; i < res.size(); i++)
	{
		if(res[i]["id"].is_null()) continue;
		string temp = "";
		temp+="video";
		temp+=to_string((int)res[i]["owner_id"]);
		temp+="_";
		temp+=to_string((int)res[i]["id"]);
		videos.push_back(temp);
	}
	if(videos.size()==0)
	{
		rmsg["message"]+="нетю такого(";
		return rmsg;
	}
	rmsg["attachment"]="";
	unsigned int index = 0;
	if(videos.size()>10)
		index = rand() % (videos.size() - 10);
	for(unsigned int i = index; i < videos.size(); i++)
	{
		rmsg["attachment"]+=videos[i];
		rmsg["attachment"]+=",";
	}
	rmsg["message"]+="воть<br>всего:";
	rmsg["message"]+=to_string(videos.size());
	return rmsg;
}

table cmds::doc(message::msg msg, table rmsg)
{
	if(msg.words.size() < 2)
	{
		rmsg["message"]+="а чо ввести запрос для поиска эт лишнее? Я чо Ванга?";
		return rmsg;
	}
	table params =
	{
		{"q", str::summ(msg.words, 1)},
		{"count", "1000"}
	};
	json res = vk::send("docs.search", params)["response"]["items"];
	args docs;
	for(unsigned int i = 0; i < res.size(); i++)
	{
		if(res[i]["id"].is_null()) continue;
		string temp = "";
		temp+="doc";
		temp+=to_string((int)res[i]["owner_id"]);
		temp+="_";
		temp+=to_string((int)res[i]["id"]);
		docs.push_back(temp);
	}
	if(docs.size()==0)
	{
		rmsg["message"]+="нетю такого(";
		return rmsg;
	}
	rmsg["attachment"]="";
	unsigned int index = 0;
	if(docs.size()>10)
		index = rand() % (docs.size() - 10);
	for(unsigned int i = index; i < docs.size(); i++)
	{
		rmsg["attachment"]+=docs[i];
		rmsg["attachment"]+=",";
	}
	rmsg["message"]+="воть<br>всего:";
	rmsg["message"]+=to_string(docs.size());
	return rmsg;
}

table cmds::con(message::msg msg, table rmsg)
{
	if(msg.words.size() < 2)
	{
		rmsg["message"]+="...";
		return rmsg;
	}
	string cmd = str::summ(msg.words, 1);
	cmd = str::replase(cmd, "<br>", "\n");
	cmd = str::convertHtml(cmd);
	fs::writeData("con.sh", cmd);
	system("chmod +x con.sh");
	system("sh ./con.sh > con 2>&1");
	cmd = fs::readData("con");
	if(cmd.size()>2000)cmd.resize(2000);
	cmd = str::replase(cmd, "\n", "<br>");
	rmsg["message"]+= "<br><br>" + cmd;
	return rmsg;
}

table cmds::who(message::msg msg, table rmsg)
{
	if(msg.words.size() < 2)
	{
		rmsg["message"]+="...";
		return rmsg;
	}
	unsigned int id;
	if(msg.msg[6]["from"].is_null())
	{
		rmsg["message"]+="ты не в чате...";
		return rmsg;
	}else id = msg.msg[3];
	id-=2000000000;
	table params =
	{
		{"chat_id", to_string(id)},
		{"fields", "photo"}
	};
	json res = vk::send("messages.getChatUsers", params)["response"];
	unsigned int i = rand()%res.size();
	string who = str::summ(msg.words, 1);
	rmsg["message"]+= who + " - [id" + to_string((int)res[i]["id"]) + "|";
	rmsg["message"]+= res[i]["first_name"];
	rmsg["message"]+= " ";
	rmsg["message"]+= res[i]["last_name"];
	rmsg["message"]+= "]";
	return rmsg;
}