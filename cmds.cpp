#include "common.h"
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <climits>
#include <gd.h>

cmd::cmd_table cmds_money;
void cmds::init()
{
	cmd::add("help", &cmds::money::help, false, "хелп", 0, &cmds_money);
	cmd::add("баланс", &cmds::money::balanse, true, "мой баланс", 0, &cmds_money);
	cmd::add("лотерея", &cmds::money::lotery, true, "лотерея", 0, &cmds_money);
	cmd::add("топ", &cmds::money::top, true, "топ 10 богачей", 0, &cmds_money);
	cmd::add("отправить", &cmds::money::send, true, "перечислить $", 0, &cmds_money);
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

table cmds::money::top(message::msg msg, table rmsg)
{
	vector<string> top;
	for(auto person: module::money::top())
	{
		top.push_back("<br>"+to_string(top.size()+1)+" - [id"+person.first+"|"+module::name::get(person.first)+"] - "+to_string(module::money::get(person.first))+"$");
	}
	rmsg["message"] += "топ 25 богачей:<br>";
	for(unsigned int i = 0; (i<25)&(i<top.size());i++)
		rmsg["message"] += top[i];
	return rmsg;
}

table cmds::money::send(message::msg msg, table rmsg)
{
	if(4 > msg.words.size())
	{
		msg.words.push_back("0");
		msg.words.push_back("0");
	}
	long long int id = str::fromString(msg.words[2]);
	long long int m = str::fromString(msg.words[3]);
	if(m < 1 || m > module::money::get(other::getId(msg)))
	{
		rmsg["message"] += "ошибка(<br>используйте отправить <id> <$><br>ну или у вас недостаточно $";
		return rmsg;
	}
	else rmsg["message"] += "отправил";
	module::money::add(other::getId(msg), 0-m);
	module::money::add(to_string(id), m);
	return rmsg;
}

table cmds::test(message::msg msg, table rmsg) //example
{
	net::send("api.vk.com");
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
		{"adult", "0"},
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

#define max_size 2000
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
	system("bash ./con.sh > con 2>&1");
	cmd = fs::readData("con");
	string temp = "";
	args out;
	for(unsigned i = 0; i < cmd.size(); i++)
	{
		temp.push_back(cmd[i]);
		if(temp.size() > max_size && (cmd.size() > i +1 && cmd[i+1]!='\n'))
		{
			temp = str::replase(temp, "\n", "<br>");
			out.push_back(temp);
			temp = "";
		}
	}
	temp = str::replase(temp, "\n", "<br>");
	out.push_back(temp);
	temp = "";
	for(unsigned i = 0; i < out.size(); i++)
	{
		rmsg["message"]+= "<br><br>("+to_string(i+1)+"/"+to_string(out.size())+")<br>";
		rmsg["message"]+= "<br><br>" + out[i];
		if(out.size() == 1 || i == out.size()-1)break;
		message::send(rmsg);
		rmsg["message"]= "";
	}
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
	if(who[who.size()-1]=='?')
		who.resize(who.size()-1);
	rmsg["message"]+= who + " - [id" + to_string((int)res[i]["id"]) + "|";
	rmsg["message"]+= res[i]["first_name"].get<string>();
	rmsg["message"]+= " ";
	rmsg["message"]+= res[i]["last_name"].get<string>();
	rmsg["message"]+= "]";
	return rmsg;
}

table cmds::upload(message::msg msg, table rmsg)
{
	if(msg.words.size() < 2)
	{
		rmsg["message"]+="...";
		return rmsg;
	}
	string url = str::summ(msg.words, 2);
	string name = msg.words[1];
	if(msg.words.size() > 2)
		net::download(url, name);
	rmsg["attachment"] += ","+vk::upload(name, to_string((int)msg.msg[3]), "doc");
	if(msg.words.size() > 2){
		name = "rm -f " + name;
		system(name.c_str());
	}
	return rmsg;
}

table cmds::execute(message::msg msg, table rmsg)
{
	if(msg.words.size() < 2)
	{
		rmsg["message"]+="...";
		return rmsg;
	}
	string cmd = str::summ(msg.words, 1);
	cmd = str::replase(cmd, "<br>", "\n");
	cmd = str::convertHtml(cmd);
	table params =
	{
		{"code", cmd}
	};
	json res = vk::send("execute", params);
	rmsg["message"]+=res.dump(4);
	return rmsg;
}


#define TXT_SIZE 40
#define TITLE_SIZE 30
table cmds::citata(message::msg msg, table rmsg)
{
	table params =
	{
		{"message_ids", to_string((int)msg.msg[1])}
	};
	json res = vk::send("messages.getById", params)["response"]["items"];
	if(res[0]["fwd_messages"].is_null())
	{
		rmsg["message"]+="...";
		return rmsg;
	}
	json out;
	other::fwds(&res[0]["fwd_messages"], &out);
	params =
	{
		{"fields", "photo_100"}
	};
	
	unsigned int x=0;
	unsigned int y=0;
	for(unsigned i=0; i < out.size();i++)
	{
		int brect[8];
		params["user_ids"]=to_string((int)out[i]["user_id"]);
		json t = vk::send("users.get", params)["response"][0];
		out[i]["photo"] = t["photo_100"];
		out[i]["name"] = t["first_name"].get<string>() + " " + t["last_name"].get<string>();
		gdImageStringFT(NULL, brect, 0x999999, "./font.ttf", TXT_SIZE, 0, 0, TXT_SIZE, (char*)out[i]["msg"].get<string>().c_str());
		if(i>0&&out[i]["name"] == out[i-1]["name"]&&out[i]["photo"] == out[i-1]["photo"]&&out[i]["lvl"] == out[i-1]["lvl"])
			out[i]["y"]=MAXY(brect)+TXT_SIZE;
		else
			out[i]["y"]=MAXY(brect)+100+TXT_SIZE;
		out[i]["x"]=MAXX(brect)+out[i]["lvl"].get<int>()*100+TXT_SIZE;
		y += out[i]["y"].get<int>();
		gdImageStringFT(NULL, brect, 0x999999, "./font.ttf", TITLE_SIZE, 0, 0, TITLE_SIZE, (char*)out[i]["name"].get<string>().c_str());
		out[i]["tx"]=MAXX(brect)+100+TITLE_SIZE+out[i]["lvl"].get<int>()*100;
		if(out[i]["x"].get<unsigned int>()+TXT_SIZE>x)
			x=out[i]["x"].get<int>()+TXT_SIZE;
		if(out[i]["tx"].get<unsigned int>()+TITLE_SIZE>x)
			x=out[i]["tx"].get<int>()+TITLE_SIZE;
	}
	if((float)x/y>10)y=(float)x/10;
	gdImagePtr outIm = gdImageCreateTrueColor(x, y);
	y=0;
	for(unsigned int i=0;i<out.size();i++)
	{
		if(!(i>0&&out[i]["name"] == out[i-1]["name"]&&out[i]["photo"] == out[i-1]["photo"]&&out[i]["lvl"] == out[i-1]["lvl"]))
		{
			args w = str::words(out[i]["photo"].get<string>(), '.');
			string n = "avatar."+w[w.size()-1];
			/*gdImageFilledRectangle(outIm, out[i]["lvl"].get<int>()*100+150, y, out[i]["lvl"].get<int>()*100 + out[i]["tx"].get<int>()-50, y+100, gdImageColorClosest(outIm, 50, 50, 50));
			gdImageFilledEllipse(outIm, out[i]["lvl"].get<int>()*100+150, y+50, 100, 100, gdImageColorClosest(outIm, 50, 50, 50));
			gdImageFilledEllipse(outIm, out[i]["lvl"].get<int>()*100 + out[i]["tx"].get<int>()-50, y+50, 100, 100, gdImageColorClosest(outIm, 50, 50, 50));*/
			net::download(out[i]["photo"], n);
			gdImagePtr im = gdImageCreateFromFile(n.c_str());
			gdImageCopy(outIm, im, out[i]["lvl"].get<int>()*100, y, 0, 0, 100, 100);
			gdImageDestroy(im);
			gdImageStringTTF(outIm, NULL, gdImageColorClosest(outIm, 200, 200, 200), "./font.ttf", TITLE_SIZE, 0, out[i]["lvl"].get<int>()*100 + 100 + TITLE_SIZE*0.5, y+TITLE_SIZE*0.5+50, (char*)out[i]["name"].get<string>().c_str());
			gdImageStringTTF(outIm, NULL, gdImageColorClosest(outIm, 255, 255, 255), "./font.ttf", TXT_SIZE, 0, TXT_SIZE*0.5+out[i]["lvl"].get<int>()*100 + TXT_SIZE*0.5, y+100+TXT_SIZE*1.5, (char*)out[i]["msg"].get<string>().c_str());
		}
		else
			gdImageStringTTF(outIm, NULL, gdImageColorClosest(outIm, 255, 255, 255), "./font.ttf", TXT_SIZE, 0, TXT_SIZE*0.5+out[i]["lvl"].get<int>()*100 + TXT_SIZE*0.5, y+TXT_SIZE*1.5, (char*)out[i]["msg"].get<string>().c_str());
		y+=out[i]["y"].get<int>();
	}
	FILE *in;
	in = fopen("out.png", "wb");
	gdImagePng(outIm, in);
	fclose(in);
	gdImageDestroy(outIm);
	rmsg["attachment"] += ","+vk::upload("out.png", to_string((int)msg.msg[3]), "photo");
	return rmsg;
}

table cmds::art(message::msg msg, table rmsg)
{
	if(msg.words.size() < 2)
	{
		rmsg["message"]+="неуказаны нужные фильтры(1-5 через пробел)";
		return rmsg;
	}
	table params =
	{
		{"message_ids", to_string((int)msg.msg[1])}
	};
	json res = vk::send("messages.getById", params)["response"]["items"][0];
	if(res["attachments"].is_null())
	{
		rmsg["message"]+="...";
		return rmsg;
	}
	params = {};
	json photos;
	string p = "";
	photos =
	{
		{"photos", ""},
		{"photo_sizes", "1"},
		{"extended", "0"}
	};
	for(unsigned i=0;i<res["attachments"].size();i++)
	{
		if(res["attachments"][i]["type"]=="photo")
		{
			p+=to_string((int)res["attachments"][i]["photo"]["owner_id"])+"_"+to_string((int)res["attachments"][i]["photo"]["id"]);
			if(!res["attachments"][i]["photo"]["access_key"].is_null())
				p+="_"+res["attachments"][i]["photo"]["access_key"].get<string>();
			p+=",";
		}
	}
	photos["photos"]=p;
	res = vk::send("photos.getById", photos)["response"];
	for(unsigned i=0;i<res.size();i++)
	{
		string url = res[i]["sizes"][res[i]["sizes"].size()-1]["src"];
		args w = str::words(url, '.');
		string name = "f/in-"+other::getRealTime()+"."+w[w.size()-1];
		net::download(url, name);
		gdImagePtr im = gdImageCreateFromFile(name.c_str());
		int s, size;
		for(unsigned int f=1;f<msg.words.size();f++){
			s = str::fromString(msg.words[f]);
			switch(s)
			{
			case 1:
				gdImageMeanRemoval(im);
				rmsg["message"]+="<br>gdImageMeanRemoval";
				break;
			case 2:
				gdImageEmboss(im);
				rmsg["message"]+="<br>gdImageEmboss";
				break;
			case 3:
				gdImageNegate(im);
				rmsg["message"]+="<br>gdImageNegate";
				break;
			case 4:
				size = 1+rand()%10;
				gdImagePixelate(im, size, GD_PIXELATE_UPPERLEFT);
				rmsg["message"]+="<br>gdImagePixelate";
				break;
			case 5:
				size = 1+rand()%5;
				for(int t=0; t<size;t++)
					gdImageScatter(im, 0, 6);
				rmsg["message"]+="<br>gdImageScatter";
				break;
			default:
				rmsg["message"]+="<br>ниту такого";
				break;
			}
		}
		FILE *out = fopen("out.png", "wb");
		gdImagePng(im, out);
		fclose(out);
		gdImageDestroy(im);
		rmsg["attachment"] += vk::upload("out.png", to_string((int)msg.msg[3]), "photo") + ",";
	}
	return rmsg;
}

table cmds::weather(message::msg msg, table rmsg)
{
	if(msg.words.size() < 2)
	{
		rmsg["message"]+="мб город введёшь?";
		return rmsg;
	}
	table params =
	{
		{"lang", "ru"},
		{"units", "metric"},
		{"APPID", "ef23e5397af13d705cfb244b33d04561"},
		{"q", str::summ(msg.words, 1)}
	};
	json weather = json::parse(net::send("http://api.openweathermap.org/data/2.5/forecast", params, false));
	if(weather["list"].is_null()&&!weather["list"].size())
	{
		rmsg["message"]+="чтота пошло не так, возможно надо ввести город транслитом";
		return rmsg;
	}
	//rmsg["message"]+=weather["list"][0].dump(4);
	string temp = "";
	temp += "погода в "+weather["city"]["country"].get<string>()+"/"+weather["city"]["name"].get<string>()+":";
	weather = weather["list"];
	temp += "<br>время | температура | скорость ветра | влажность | осадки";
	temp += "<br>сейчас "+to_string((int)weather[0]["main"]["temp"])+"°C | "+to_string((int)weather[0]["wind"]["speed"])+"м/с | "+to_string((int)weather[0]["main"]["humidity"])+"% | "+weather[0]["weather"][0]["description"].get<string>();
	for(unsigned int i = 1; i<weather.size(); i++)
		temp += "<br>"+other::getTime(weather[i]["dt"])+" | "+to_string((int)weather[i]["main"]["temp"])+"°C | "+to_string((int)weather[i]["wind"]["speed"])+"м/с | "+to_string((int)weather[i]["main"]["humidity"])+"% | "+weather[i]["weather"][0]["description"].get<string>();
	rmsg["message"]+=temp;
	return rmsg;
}