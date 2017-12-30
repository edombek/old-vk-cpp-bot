#include "common.h"
#include <iostream>
#include <cstdlib>
#include <ctime> 
void module::start()
{
	srand(time(NULL));
	module::name::read();
	module::money::read();
	module::admin::read();
	module::ban::read();
}

long long int oldbalance;
table module::TR(message::msg msg, table rmsg)
{
	oldbalance = module::money::get(other::getId(msg));
	rmsg["message"] = module::name::get(other::getId(msg));
	rmsg["message"] += ", ";
	return rmsg;
}

table module::postTR(message::msg msg, table rmsg)
{
	if(oldbalance != module::money::get(other::getId(msg)))
	{
		rmsg["message"]+="<br><br>$";
		rmsg["message"]+=to_string(module::money::get(other::getId(msg))-oldbalance);
		rmsg["message"]+="<br>$: ";
		rmsg["message"]+=to_string(module::money::get(other::getId(msg)));
	}
	return rmsg;
}


// name system
#define names_path "names.json"
json names;
void module::name::read()
{
	if(fs::exists(names_path))
	{
		names = json::parse(fs::readData(names_path));
	}
}
void module::name::save()
{
	fs::writeData(names_path, names.dump(4));
}
string module::name::get(string id)
{
	if(names[id].is_null())
	{
		names[id] = module::name::getVk(id);
		module::name::save();
	}
	return names[id];
}
void module::name::set(string id, string name)
{
	names[id] = str::replase(str::replase(name, "#", "[*]"), ".", "[*]");
	module::name::save();
}
string module::name::getVk(string id)
{
	table params;
	params["user_ids"] = id;
	params["fields"] = "";
	params["name_case"] = "Nom";
	json temp = vk::send("users.get", params);
	if(!temp["response"][0]["first_name"].is_null()) return temp["response"][0]["first_name"];
	return "";
}

// money system
#define moneys_path "moneys.json"
#define START_MIN 100
#define START_MAX 500
json moneys;
void module::money::read()
{
	if(fs::exists(moneys_path))
	{
		moneys = json::parse(fs::readData(moneys_path));
	}
}

void module::money::save()
{
	fs::writeData(moneys_path, moneys.dump(4));
}

long long int module::money::get(string id)
{
	if(moneys[id].is_null())
	{
		moneys[id] = rand() % (START_MAX - START_MIN +1 ) + START_MIN;
		module::money::save();
	}
	return moneys[id];
}

void module::money::add(string id, long long int money)
{
	moneys[id] = module::money::get(id) + money;
	module::money::save();
}

bool pred(const pair<string, long long int> &a, const pair<string, long long int> &b)
{
	return a.second > b.second;
}

vector<pair<string, long long int>> module::money::top()
{
	map<string, long long int> m;
	m = moneys.get<map<string, long long int>>();
	vector<pair<string, long long int>> out(m.begin(), m.end());
    std::sort(out.begin(), out.end(), pred);
	return out;
}

// admin system
#define admins_path "admins.json"
json admins;
void module::admin::read()
{
	if(fs::exists(admins_path))
	{
		admins = json::parse(fs::readData(admins_path));
	}else
	{
		admins["default"] = false;
		module::admin::save();
	}
}
void module::admin::save()
{
	fs::writeData(admins_path, admins.dump(4));
}
bool module::admin::get(string id)
{
	if(admins[id].is_null())
	{
		admins[id] = admins["default"];
	}
	return admins[id];
}
void module::admin::set(string id, bool admin)
{
	admins[id] = admin;
	module::admin::save();
}

// ban system
#define bans_path "bans.json"
json bans;
void module::ban::read()
{
	if(fs::exists(bans_path))
	{
		bans = json::parse(fs::readData(bans_path));
	}else
	{
		bans["default"] = false;
		module::ban::save();
	}
}
void module::ban::save()
{
	fs::writeData(bans_path, bans.dump(4));
}
bool module::ban::get(string id)
{
	if(bans[id].is_null())
	{
		bans[id] = bans["default"];
	}
	return bans[id];
}
void module::ban::set(string id, bool ban)
{
	bans[id] = ban;
	module::ban::save();
}