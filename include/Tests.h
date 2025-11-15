#pragma once
#include "Memory.h"
#include <vector>
#include <string>

struct TestResult {
    std::string name;
    int errors = 0;
    std::vector<std::string> log;
};

class Tests {
public:
    static TestResult ZeroOne(Memory& m);
    static TestResult Checkerboard(Memory& m);
    static TestResult MarchC(Memory& m);
    static TestResult ReadDisturb(Memory& m);
};
