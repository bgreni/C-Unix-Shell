#include <string>
#include <vector>
#include "dshelltypes.h"
#include "tokenize.h"
#include "builtinjobs.h"
#include "dshellconstants.h"
#include <map>
#include "dshellhelpers.h"
#include "dshelljob.h"

void parseInput(string &input, vector<string> &tokens, const string &delims) {
  tokens = tokenize(input,delims.c_str());
}

vector<string> findDelims(string &input) {
  vector<string> output;
  for (int i = 0; i < input.length(); i++) {
    string s(1,input.at(i));
    if (!isNotDelim(s)) {
      output.push_back(s);
    }
  }
  return output;
}

FuncMap setupMap() {
  FuncMap map;
  map["pwd"] = myPwd;
  map["cd"] = myCd;
  map["exit"] = myExit;
  map["a2path"] = myPath;
  map["$PATH"] = printPath;
  map[""] = NULL;

  return map; 
}

bool isNotDelim(string &input) {
  string allDelimTokens[] = {";","|","&"};
  for (int i = 0; i < 3; i++) {
    if (input.compare(allDelimTokens[i]) == 0) {
      return false;
    }
  }
  return true;
}

bool isPipeToken(string &input) {
  return input.compare(PIPETOKEN) == 0;
}

bool isBackToken(string &input) {
  return input.compare(BACKTOKEN) == 0;
}

bool isNotSemiColon(string &input) {
  return input.compare(SEMICOLON) != 0;
}

bool isNoArg(string &input) {
  return input.compare(NOARG) == 0;
}

bool isExitCommand(string &input) {
  return input.compare(EXIT) == 0;
}

bool isCdCommand(string &input) {
  return input.compare(CD) == 0;
}

bool isa2Path(string &input) {
  return input.compare(A2PATH) == 0;
}

bool isRedicrectToken(string &input) {
  return input.compare(REDIRECTOKEN) == 0;
}

bool isSimpleCommand(DShellCommand *command) {
  return command->isPiped == command->isOutRedirected == false;
}

vector<bool> isCompoundCommand(string &input) {
  vector<bool> commandBools;
  for (int i = 0; i < 2; i++) {
    commandBools.push_back(false);
  }
  for (int i = 0; i < input.length(); i++) {
    if (input.at(i) == '|') {
      commandBools[0] = true;
    } else if (input.at(i) == '>') {
      commandBools[1] = true;
    }
  }
  return commandBools;
}


string boolToString(bool b) {
  return b ? "true" : "false";
}
