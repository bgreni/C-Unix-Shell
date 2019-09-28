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
#include "builtinjobs.h"
#include "tokenize.h"
#include "JobQueue.h"
using namespace std;
string PATH = "/bin/:/usr/bin/";
vector<int> childProcesses;

void sigHandler(int sig) { 
  for (int i = 0; i < childProcesses.size(); i++) {
    kill(childProcesses[i], SIGKILL);
  }
  childProcesses.clear();
}

int main(int argc, char **argv) {

  signal(SIGINT, &sigHandler);
  signal(SIGTSTP, &sigHandler);

  cout << WELCOME_MESSAGE << endl;
  vector<string> tokens;
  vector<char> inDelims;
  string input;

  while (1) {
    cout << BASE_STRING;
    getline(cin,input);
    JobQueue jobQueue;
    jobQueue.createJobQueue(input);
    jobQueue.execQueue();
    
  }
  return 0;
}







