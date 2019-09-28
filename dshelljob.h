
#pragma once
#include <string>
#include <vector>
#include <queue>
#include "dshelltypes.h"
#include <iostream>
#include "dshellhelpers.h"
using namespace std;

struct DShellCommand {
    string op;
    vector<string> argv;
    string filename;
    bool isPiped;
    bool isBackground;
    bool isOutRedirected;
    bool hasArg;

    void toString();
};


struct CommandPair {
    DShellCommand command1;
    DShellCommand command2;
    bool isPiped;
};


class DShellJob {
    private:
        queue<CommandPair> commandQueue;
        FuncMap map;
        void assembleRelatedCommands(vector<string>&, vector<string>&);
        void runCommand(DShellCommand *command);
        void runOutRedirect(DShellCommand *command);
        void runPiped(CommandPair *pair);
        void runFork(DShellCommand *command);
        void tryExternal(DShellCommand *command);
        void convertVectorToArray(vector<string> &input, char** v);
        DShellCommand createPipedCommand(string &input, DShellCommand &command);
        DShellCommand createOutRedCommand(string &input);
        DShellCommand createCommand(string &input, bool piped);
       
    public:
        DShellJob();
        void doJob();
        void createCommandQueue(string &input);
};