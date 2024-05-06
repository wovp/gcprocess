#include "gxGetProcess.h"

std::vector<ProcessInfo> gxGetAllProcesses() {
    std::vector<ProcessInfo> processes;

    DWORD processIds[1024], cbNeeded, cProcesses;
    if (EnumProcesses(processIds, sizeof(processIds), &cbNeeded))
    {
        cProcesses = cbNeeded / sizeof(DWORD);
        for (unsigned int i = 0; i < cProcesses; ++i)
        {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);
            if (hProcess)
            {
                WCHAR szProcessName[MAX_PATH] = L"<unknown>";
                HMODULE hMod;
                DWORD cbNeeded;
                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                {
                    GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(WCHAR));
                }
                ProcessInfo processInfo;
                processInfo.processId = processIds[i];
                processInfo.processName = szProcessName;
                processes.push_back(processInfo);
                CloseHandle(hProcess);
            }
        }
    }

    return processes;
}
