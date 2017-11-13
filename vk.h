#pragma once
namespace vk
{
	void start();
	nlohmann::json send(string method, table params = {}, bool sendtoken = true);
	string getToken();
	void setToken(string token);
	void setId(int id);
	int getId();
}