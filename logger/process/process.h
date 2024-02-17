#pragma once

#ifdef _WIN32
#include <windows.h>
#define popen _popen
#define pclose _pclose
#endif

#include <iostream>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <sstream>

std::vector<std::string> split(const std::string &s, char delimiter);

std::string getProcName(DWORD pid);

DWORD getCurrentPID();