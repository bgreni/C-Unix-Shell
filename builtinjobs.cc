#include <string>
#include <iostream>
#include <unistd.h>
#include <cerrno>
#include "builtinjobs.h"
#include "dshellconstants.h"
#include "tokenize.h"
#include "dshellhelpers.h"
using namespace std;
extern string PATH;
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
  cout << "Exiting" << endl;
  _exit(1);
}

void myPath(string input) {
 vector<string> paths;
  parseInput(input, paths, COLON);
  if (paths[0].compare("$PATH") == 0) {
    for (int i = 1; i < paths.size(); i++) {
      PATH += ":" + paths[i];
    }
  } else {
    PATH.clear();
    PATH.append(input);
  }
}

void printPath(string input) {
  cout << PATH << endl;
}