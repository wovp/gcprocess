#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <Psapi.h>
#include <iostream>
#include <tlhelp32.h>
#include <tchar.h>
#pragma comment(lib, "Psapi.lib")
struct ProcessInfo {
    DWORD processId;
    std::wstring processName;
};

class gxControlProcess
{
public:
    gxControlProcess();
    ~gxControlProcess();
    std::vector<ProcessInfo> gxGetAllProcesses();
    bool gxPauseProcess(DWORD processId);
    bool gxResumeProcess(DWORD processId);
    std::vector<DWORD>ListProcessThreads(DWORD processId);
    bool gxCompressProcessMemory(DWORD processId);
};

