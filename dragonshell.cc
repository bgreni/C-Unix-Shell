#include <vector>
#include <map>
#include <queue>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <sys/types.h>
#include <signal.h>
#include "dshellconstants.h"
#include "dshelltypes.h"
#include "dshelljob.h"
using namespace std;



// forward declaration of functions
void myPwd(string input);
void myCd(string input);
void parseInput(string&, vector<string>&);
void parseDelims(string&, vector<char>&);
void myExit(string);
void execJobQueue(queue<DShellJob>&, FuncMap&);
FuncMap setupMap();
FuncMap setupDelimMap();
queue<DShellJob> buildJobQueue(vector<string>&);
void doJob(DShellJob&, FuncMap&);

/**
 * @brief Tokenize a string 
 * 
 * @param str - The string to tokenize
 * @param delim - The string containing delimiter character(s)
 * @return std::vector<std::string> - The list of tokenized strings. Can be empty
 */
std::vector<std::string> tokenize(const std::string &str, const char *delim) {
  char* cstr = new char[str.size() + 1];
  std::strcpy(cstr, str.c_str());

  char* tokenized_string = strtok(cstr, delim);

  std::vector<std::string> tokens;
  while (tokenized_string != NULL)
  {
    tokens.push_back(std::string(tokenized_string));
    tokenized_string = strtok(NULL, delim);
  }
  delete[] cstr;

  return tokens;
}



int main(int argc, char **argv) {
  // print the string prompt without a newline, before beginning to read
  // tokenize the input, run the command(s), and print the result
  // do this in a loop

  cout << WELCOME_MESSAGE << endl;
  vector<string> tokens;
  vector<char> inDelims;
  string input;
  FuncMap funcMap = setupMap();

  while (1) {
    cout << BASE_STRING;
    getline(cin,input);
    parseInput(input, tokens);
    if (tokens.size() > 0) {
      queue<DShellJob> jobQueue = buildJobQueue(tokens);
      execJobQueue(jobQueue, funcMap);
      // cout << "QUEUE: ";
      // cout << jobQueue.size() << endl;
      // while (!jobQueue.empty()) {
      //   cout << jobQueue.front().op1 << endl;
      //   cout << jobQueue.front().op2 << endl;
      //   cout << jobQueue.front().args1 << endl;
      //   cout << jobQueue.front().args2 << endl;
      //   cout << "_________________" << endl;
      //   jobQueue.pop();
      // }
    }
    
  }
  return 0;
}


//////////////////////////////////////////////////////////
// HELPER FUNCTIONS
//////////////////////////////////////////////////////////

void parseInput(string &input, vector<string> &tokens) {
  tokens = tokenize(input,DELIMS.c_str());
}

void parseDelims(string &input, vector<char> &inDelims) {
  inDelims.clear();
  for (int i = 0; i < input.size(); i++) {
    for (int j = 0; j < DELIMS.size(); j++) {
      if (input.at(i) == DELIMS.at(j)) {
        inDelims.push_back(input.at(i));
      }
    }
  }
}

FuncMap setupMap() {
  FuncMap map;
  map["pwd"] = myPwd;
  map["cd"] = myCd;
  map["exit"] = myExit;

  return map; 
}

// FuncMap setupDelimMap() {
//   FuncMap map;
//   map[";"];
//   map["|"];
//   map["&"];
// }

bool isNotDelim(string input) {
  string allDelimTokens[] = {";","|"};
  for (int i = 0; i < 2; i++) {
    if (input.compare(allDelimTokens[i]) == 0) {
      return false;
    }
  }
  return true;
}

bool isPipeToken(string input) {
  return input.compare(PIPETOKEN) == 0;
}

bool isNotSemiColon(string input) {
  return input.compare(SEMICOLON) != 0;
}

//////////////////////////////////////////////////////////
// JOB EXECUTION HELPER FUNCTIONS
//////////////////////////////////////////////////////////

queue<DShellJob> buildJobQueue(vector<string> &tokens) {
  queue<DShellJob> jobQueue;
  int i = 0;
  while (i < tokens.size()) {
    if (i + 1 < tokens.size() && isNotDelim(tokens[i+1]) && isNotSemiColon(tokens[i])) {
      DShellJob newJob = {
        tokens[i],
        "",
        false,
        false,
        true,
        tokens[i+1],
        ""
      };
      jobQueue.push(newJob);
      i++;
    } else if (isNotDelim(tokens[i])) {
      DShellJob newJob = {
        tokens[i],
        "",
        false,
        false,
        false,
        "",
        ""
        };
        jobQueue.push(newJob);
      }
      // } else if (i + 1 >= tokens.size() && isNotDelim(tokens[i])) {
      //   DShellJob newJob = {
      //     tokens[i],
      //     "",
      //     false,
      //     false,
      //     false,
      //     "",
      //     ""
      //   };
      //   jobQueue.push(newJob);
      // } else if (i + 1 < tokens.size() && !(isNotDelim(tokens[i + 1]))) {
      //   DShellJob newJob = {
      //     tokens[i],
      //     "",
      //     false,
      //     false,
      //     false,
      //     "",
      //     ""
      //   };
      //   jobQueue.push(newJob);
      // }
      i++;
    }
    return jobQueue;
}

void execJobQueue(queue<DShellJob> &jobQueue, FuncMap &funcMap) {
  while (!jobQueue.empty()) {
    doJob(jobQueue.front(), funcMap);
    jobQueue.pop();
  }
}

void doJob(DShellJob &job, FuncMap &funcMap) {
  if (funcMap.find(job.op1) != funcMap.end()) {
    funcMap[job.op1](job.args1);
  } else {
    cout << "Invalid command " << job.op1 << endl;
  }
}

//////////////////////////////////////////////////////////
// BASE COMMANDS
//////////////////////////////////////////////////////////

// implementation of the pwd command
// @brief prints the current working directory
void myPwd(string input) {
  int n = PATH_MAX;
  char buff[n];
  getcwd(buff,n);
  cout << buff << endl;
}

void myCd(string input) {
  if (chdir(input.c_str())) {
    cout << strerror(errno) << endl;
  }
}

void myExit(string input) {
  _exit(1);
}





