#pragma once
namespace module
{
	void start();
	table TR(message::msg msg, table rmsg);
	table postTR(message::msg msg, table rmsg);
	namespace name
	{
		void read();
		void save();
		string get(string id);
		void set(string id, string name);
		string getVk(string id);
	}
	
	namespace money
	{
		void read();
		void save();
		long long int get(string id);
		void add(string id, long long int money);
	}
	
	namespace admin
	{
		void read();
		void save();
		bool get(string id);
		void set(string id, bool admin);
	}
	
	namespace phrase
	{
		void read();
		void save();
		string get();
		void add(string add);
	}
}