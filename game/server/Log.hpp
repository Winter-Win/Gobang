#pragma once

#include <iostream>
#include <vector>
#include <string>

#define INFO 0
#define WARNING 1
#define FATAL 2

using namespace std;

vector g_error_level={"INFO", "WARNING", "FATAL",}; //最后的逗号可加可不加

void log(int level, string message, string file, int line)
{
    cout << "[ " << g_error_level[level] << " ][ " << message << " ] : " << file << " : " << line << endl;
}

#define LOG(level, message) log(level, message, __FILE__, __LINE__)
