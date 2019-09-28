#include <string>
#include <queue>
#include "dshelltypes.h"
#include "dshellhelpers.h"
#include "dshellconstants.h"
#include <iostream>
#include "dshelljob.h"
#include "unistd.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <typeinfo> 
#include <algorithm>
#include <functional>
#include <sys/stat.h>
 #include <fcntl.h>
using namespace std;
extern string PATH;
extern vector<int> childProcesses;

DShellJob::DShellJob() {
    this->map = setupMap();
}

void DShellJob::createCommandQueue(string &input) {
    vector<string> tokens;
    parseInput(input, tokens, SEMICOLON);
    for (int i = 0; i < tokens.size(); i++) {
        vector<string> delims = findDelims(tokens[i]);
        vector<string> commandTokens;
        parseInput(tokens[i], commandTokens, DELIMS);
        assembleRelatedCommands(commandTokens, delims);
    }
}

void DShellJob::assembleRelatedCommands(vector<string> &tokens, vector<string> &delims) {
    
    for (int i = 0; i < tokens.size(); i++) {
        CommandPair pair;
        vector<bool> commandBools = isCompoundCommand(tokens[i]);
        if (commandBools[0]) {
            vector<string> half;
            parseInput(tokens[i], half, PIPETOKEN);
            pair.command1 = createCommand(half[0], true);
            pair.command2 = createCommand(half[1], true);
            pair.isPiped = true;
        } else if (commandBools[1]) {
            pair.command1 = createOutRedCommand(tokens[i]);
        } else {
            pair.command1 = createCommand(tokens[i], false);
        }
        commandQueue.push(pair);
    }

}

DShellCommand DShellJob::createCommand(string &input, bool piped) {
    vector<string> argv;
    DShellCommand command = {"",argv,"",piped,false,false,false};
    vector<string> tokens;
    parseInput(input, tokens, SPACE);
    command.op = tokens[0];
    for (int i = 1; i < tokens.size(); i++) {
        if (isBackToken(tokens[i])) {
            command.isBackground = true;
        } else {
            command.argv.push_back(tokens[i]);
            command.hasArg = true;
        }
    }
    if (command.hasArg == false) {
        command.argv.push_back("");
    }
    return command;
}

DShellCommand DShellJob::createOutRedCommand(string &input) {
    vector<string> argv;
    DShellCommand command = {"",argv,"",false,false,false,false};
    vector<string> tokens;
    parseInput(input, tokens, SPACE);
    command.isOutRedirected = true;
    command.op = tokens[0];
    int i = 1;
    while (!isRedicrectToken(tokens[i])) {
        command.argv.push_back(tokens[i]);
        i++;
    }
    i++;
    if (i == 2) {
        command.argv.push_back("");
    }
    command.filename = tokens[i]; // name of the file to redirect to
    if (i + 1 < tokens.size() && isBackToken(tokens[i+1])) {
        command.isBackground = true;
    }
    return command;

}

// DShellCommand DShellJob::createPipedCommand(string &input, DShellCommand &command) {
//     vector<string> argv;
//     DShellCommand newCommand = {"",argv,"",false,false,false,false};
//     command.isPiped = true;
//     vector<string> temp;
//     parseInput(input, temp, SPACE);
//     command.op = temp[0];
//     int argNum = temp.size();
//     if (argNum > 1) {
//         command.hasArg = true;
//         for (int i = 1; i < argNum; i++) {
//             command.argv.push_back(temp[i]);
//         }
//     } else {
//         command.hasArg = false;
//         command.argv.push_back("");
//     }
// }


void DShellJob::doJob() {
    while(!commandQueue.empty()) {
        CommandPair *pair = &commandQueue.front();
        if (pair->command1.isOutRedirected) {
            runOutRedirect(&pair->command1);
        } else if (pair->command1.isPiped){
            runPiped(pair);
        } else {
            runCommand(&pair->command1);
        }
        commandQueue.pop();
    }
}
void DShellJob::tryExternal(DShellCommand *command) {
    vector<string> paths; 
    int e;
    extern char * const environ[];
    parseInput(PATH, paths, COLON);
    int pid = fork();
    if (pid < 0) {
        perror("forking error");
    } else if (pid == 0) {
        int actualPid = getpid();
        childProcesses.push_back(actualPid);
        int argSize = command->argv.size();
        char **newArgv = new char*[argSize+2];
        convertVectorToArray(command->argv, newArgv);  
        newArgv[0] = const_cast<char*>(command->op.c_str());

        vector<string> fullpaths;
        for (int i = 0; i < paths.size(); i++) {
            fullpaths.push_back(paths[i]+command->op);
        }
        fullpaths.push_back(command->op);
        char buff[PATH_MAX];
        getcwd(buff, PATH_MAX);
        fullpaths.push_back(string(buff) + "/" + command->op);

        for (int i = 0; i < fullpaths.size(); i++) {
            e = 0;
            e = execve(fullpaths[i].c_str(), newArgv, NULL);
        }
        perror("dragonshell: Command not found");
        delete[] newArgv;
        _exit(0);
    } else if (pid > 0) {
        if (!command->isBackground) {
            waitpid(pid,NULL,0);
        } else {
            cout << "PID " << pid << " is running in the background" << endl;
            waitpid(pid, NULL, WNOHANG);
        }
        childProcesses.pop_back();
    }
}

void DShellJob::runCommand(DShellCommand *command) {
    // command->toString();
    if (map.find(command->op) != map.end()) {
        map[command->op](command->argv[0]);
    } else {
        tryExternal(command);
    }
}

void DShellJob::runPiped(CommandPair *pair) {
    int fd[2];
    pipe(fd);
    dup2(fd[1], STDOUT_FILENO);
    int ret = fork();
    if (ret < 0) {
        perror("fork error");
    } else if (ret == 0) {
        close(fd[0]);
        close(fd[1]);
        runCommand(&pair->command1);
    } else {
        char buffer[1024];
        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
        string input(buffer);
        pair->command2.argv.push_back(input);
        runCommand(&pair->command2);
    }
}

void DShellJob::runOutRedirect(DShellCommand *command) {
    int copy = dup(STDOUT_FILENO);
    int target = open(command->filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC);
    dup2(target,STDOUT_FILENO);
    close(target);
    runCommand(command);
    dup2(copy, STDOUT_FILENO);
}

void DShellJob::convertVectorToArray(vector<string> &input, char** v) {
    int argSize = input.size();
    v[argSize+1] = NULL;
    if (isNoArg(input[0])) {
        v[1] = NULL;
    } else {
        for (int i = 1; i < argSize+1; i++) {
            v[i] = const_cast<char*>(input[i-1].c_str());
        }
    }
}


void DShellCommand::toString() {
    cout << "--------------------------------" << endl;
    cout << "OP: " + op << endl;
    cout << "ARGS: ";
    for (int i = 0; i < argv.size(); i++) {
        cout << argv[i] + " " << endl;
    }
    cout << "FILE: " + filename << endl;
    cout << "PIPED: " + boolToString(isPiped) << endl;
    cout << "BACKGROUND: " + boolToString(isBackground) << endl;
    cout << "OUTREDIRECT: " + boolToString(isOutRedirected) << endl;
    cout << "--------------------------------" << endl;
}
