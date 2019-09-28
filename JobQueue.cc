#include "dshelljob.h"
#include "tokenize.h"
#include "dshellhelpers.h"
#include "dshellconstants.h"
#include <queue>
#include <vector>
#include <string>
#include "JobQueue.h"


JobQueue::JobQueue() {}

void JobQueue::createJobQueue(string &input) {
    vector<string> tokens;
    parseInput(input, tokens, SEMICOLON);
    for (int i = 0; i < tokens.size(); i++) {
        DShellJob newJob;
        newJob.createCommandQueue(tokens[i]);
        jobQueue.push(newJob);
    }
}

void JobQueue::execQueue() {
    while (!jobQueue.empty()) {
        jobQueue.front().doJob();
        jobQueue.pop();
    }
}