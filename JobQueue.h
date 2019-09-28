#pragma once

#include "dshelljob.h"
#include "tokenize.h"
#include "dshellhelpers.h"
#include "dshellconstants.h"
#include <queue>
#include <vector>
#include <string>

class JobQueue {
    private:
        queue<DShellJob> jobQueue;
    public:
        JobQueue();
        void createJobQueue(string &input);
        void execQueue();
};