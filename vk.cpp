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
	if(!request["error"].is_null()) cout << "VK ERROR:" << endl << request.dump(4) << endl;
	while(!request["error"].is_null() && request["error"]["error_code"]==14)
	{
		other::sleep(60000);
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