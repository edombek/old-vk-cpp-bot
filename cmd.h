#pragma once

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

#define MAX4(x,y,z,w) ((MAX((x),(y))) > (MAX((z),(w))) ? (MAX((x),(y))) : (MAX((z),(w))))
#define MIN4(x,y,z,w) ((MIN((x),(y))) < (MIN((z),(w))) ? (MIN((x),(y))) : (MIN((z),(w))))

#define MAXX(x) MAX4(x[0],x[2],x[4],x[6])
#define MINX(x) MIN4(x[0],x[2],x[4],x[6])
#define MAXY(x) MAX4(x[1],x[3],x[5],x[7])
#define MINY(x) MIN4(x[1],x[3],x[5],x[7])

namespace cmd
{
	typedef table(*msg_func)(message::msg, table); // rmsg func(msg, rmsg)
	typedef struct{
		string info;
		cmd::msg_func func;
		int cost;
		bool admin_cmd;
		bool disp;
	}cmd_dat;
	typedef map<string, cmd_dat> cmd_table;
	
	void init();
	void add(string command, cmd::msg_func func, bool disp=false, string info="", int cost=0, cmd::cmd_table *cmd_temp=NULL, bool admin_cmd = false);
	table start(message::msg msg, table rmsg, string command, cmd::cmd_table *cmd_temp=NULL);
	string helpList(cmd::cmd_table *cmd_temp, message::msg msg);
}