#pragma once
#include <map>
#include <vector>
#include <string>

using namespace std;

typedef vector<string> args;
typedef map<string, string> table;

#include "json.hpp"

using json = nlohmann::json;

#include "fs.h"
#include "net.h"
#include "vk.h"
#include "lp.h"
#include "msg.h"
#include "str.h"
#include "cmd.h"
#include "cmds.h"
#include "other.h"
#include "modules.h"

#define botname {"кот", "!"}