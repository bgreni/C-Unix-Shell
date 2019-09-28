#pragma once

#include <string>
#include <vector>
#include "dshelltypes.h"
#include "dshelljob.h"
#include <map>

// no idea why I need to forward declare this here or else compiler complains
struct DShellCommand;

void parseInput(string &input, vector<string> &tokens, const string &delims);
vector<string> findDelims(string &input);
FuncMap setupMap();
bool isNotDelim(string &input);
bool isPipeToken(string &input);
bool isBackToken(string &input);
bool isNotSemiColon(string &input);
bool isNoArg(string &input);
bool isRedicrectToken(string &input);
bool isExitCommand(string &input);
bool isCdCommand(string &input);
bool isa2Path(string &input);
bool isSimpleCommand(DShellCommand *command);
vector<bool> isCompoundCommand(string &input);
string boolToString(bool b);