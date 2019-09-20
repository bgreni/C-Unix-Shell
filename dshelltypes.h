#pragma once

#include <string>
#include <map>
using namespace std;

typedef void(*FunctionPointer)(string args);
typedef map<string, FunctionPointer> FuncMap;