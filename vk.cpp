#include "common.h"
#include <iostream>
#include <ctime>

string vk_token;
#define config "config.json"

#define vk_version  "5.69"

using namespace vk;

int my_id;

/* Add stats and check token */
void vk::start()
{
	json data_temp;
	if(fs::exists(config))
	{
		data_temp = json::parse(fs::readData(config));
		vk::setToken(data_temp["token"]);
	}
	auto data = vk::send("stats.trackVisitor");
	if (!data["error"].is_null())
	{
		cout << "invalid vk_token" << endl;
		data_temp["token"] = "put vk tocken here (vkhost.github.io)";
		fs::writeData(config, data_temp.dump(4));
	}
	vk::setId(vk::send("users.get")["response"][0]["id"]);
	cout << "my id: " << vk::getId() << endl;
	cout << "my key: " << vk::getToken() << endl;
}

/* Send vk request and get response */
nlohmann::json vk::send(string method, table params, bool sendtoken) 
{
	if (sendtoken) {
		params["access_token"] = vk_token;
	}
	params["v"] = vk_version;
	nlohmann::json request = nlohmann::json::parse(net::send("https://api.vk.com/method/" + method, params));
	if(!request["error"].is_null()) cout << "VK ERROR:" << endl << request/*.dump(4)*/ << endl;
	while(!request["error"].is_null() && request["error"]["error_code"]==14)
	{
		other::sleep(300000);
		request = nlohmann::json::parse(net::send("https://api.vk.com/method/" + method, params));
	}
	other::sleep(200);
	return request;
}

/* Get vk token */
string vk::getToken()
{
	return vk_token;
}

void vk::setToken(string token)
{
	vk_token = token;
}

void vk::setId(int id)
{
	my_id = id;
}

int vk::getId()
{
	return my_id;
}

string vk::upload(string path, string peer_id, string type)
{
	json res;
	table params =
	{
		{"peer_id", peer_id}
	};
	res = vk::send(type+"s.getMessagesUploadServer", params)["response"];
	string tmp = net::upload(res["upload_url"], path);
	if(tmp == "" || str::at(tmp, "504 Gateway Time-out"))
	{
		return "";
	}
	res = json::parse(tmp);
	params = {};
	if(type=="photo"){
		params["server"] = to_string(res["server"].get<int>());
		params["photo"] = res["photo"];
		params["hash"] = res["hash"];
		res = vk::send("photos.saveMessagesPhoto", params);
	}else
	if(type=="doc"){
		params["file"] = res["file"];
		res = vk::send("docs.save", params)/*["response"]*/;
	}
	if(res["response"].is_null())
	{
		return "";
	}
	return type+to_string((int)res["response"][0]["owner_id"])+"_"+to_string((int)res["response"][0]["id"]);
}