
#pragma once
#include <string>
using namespace std;

struct DShellJob {
    string op1;
    string op2;
    bool isPiped;
    bool isBackground;
    bool hasArg;
    string args1;
    string args2;
};