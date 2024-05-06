#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")
struct ProcessInfo {
    DWORD processId;
    std::wstring processName;
};
std::vector<ProcessInfo> gxGetAllProcesses();