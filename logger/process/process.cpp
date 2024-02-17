#include "process.h"

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens(0);
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

std::string exec(const char *cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), sizeof(buffer), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

std::string getProcName(DWORD pid)
{
    std::string str = std::string("tasklist /svc /FI \"PID eq ") + std::to_string(pid) + std::string(" \"");
    const char *cmd = str.c_str();
    auto result = exec(cmd);
    auto tokens = split(result, '\n');

    for (auto &token : tokens)
    {
        if (token.find(std::to_string(pid)) != std::string::npos)
        {
            return token.substr(0, token.find(" "));
        }
    }
    return "unknown";
}

DWORD getCurrentPID()
{
    HWND hWnd;
    DWORD pid = 0;

    hWnd = GetForegroundWindow();
    GetWindowThreadProcessId(hWnd, &pid);
    return pid;
}