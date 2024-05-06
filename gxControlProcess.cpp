#include "gxControlProcess.h"

void printError(TCHAR const* msg);
gxControlProcess::gxControlProcess()
{
    // 在构造函数中可以进行初始化操作
}

gxControlProcess::~gxControlProcess()
{
    // 在析构函数中可以进行资源释放等操作
}

bool gxControlProcess::gxPauseProcess(DWORD processId)
{

    // 获取进程所有线程的id
    std::vector<DWORD> threadIds = ListProcessThreads(processId);

    // 暂停进程中的所有线程
    for (const auto& threadid : threadIds) {
        HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, threadid);
        DWORD threadCount = Wow64SuspendThread(hThread);
        if (threadCount == (DWORD)-1)
        {
            // 暂停线程失败
            DWORD dwError = GetLastError();
            std::cout << "Wow64SuspendThread failed with error: " << dwError << std::endl;
            return false;
        }
    }
    return true;
}
bool gxControlProcess::gxResumeProcess(DWORD processId)
{
    // 获取进程所有线程的id
    std::vector<DWORD> threadIds = ListProcessThreads(processId);

    // 暂停进程中的所有线程
    for (const auto& threadid : threadIds) {
        HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, threadid);
        DWORD threadCount = ResumeThread(hThread);
        if (threadCount == (DWORD)-1)
        {
            // 暂停线程失败
             // 暂停线程失败
            DWORD dwError = GetLastError();
            std::cout << "Wow64SuspendThread failed with error: " << dwError << std::endl;
            return false;
        }
    }
    return true;
}
std::vector<ProcessInfo> gxControlProcess::gxGetAllProcesses() {
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

std::vector<DWORD> gxControlProcess::ListProcessThreads(DWORD dwOwnerPID)
{
    std::vector<DWORD> res;
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    THREADENTRY32 te32;

    // Take a snapshot of all running threads  
    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap == INVALID_HANDLE_VALUE)
        return res;

    // Fill in the size of the structure before using it. 
    te32.dwSize = sizeof(THREADENTRY32);

    // Retrieve information about the first thread,
    // and exit if unsuccessful
    if (!Thread32First(hThreadSnap, &te32))
    {
        printError(TEXT("Thread32First"));  // Show cause of failure
        CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
        return res;
    }

    // Now walk the thread list of the system,
    // and display information about each thread
    // associated with the specified process
    do
    {
        if (te32.th32OwnerProcessID == dwOwnerPID)
        {
            res.push_back(te32.th32ThreadID);
            _tprintf(TEXT("\n     THREAD ID      = 0x%08X"), te32.th32ThreadID);
            _tprintf(TEXT("\n     base priority  = %d"), te32.tpBasePri);
            _tprintf(TEXT("\n     delta priority = %d"), te32.tpDeltaPri);

        }
    } while (Thread32Next(hThreadSnap, &te32));

    _tprintf(TEXT("\n"));

    //  Don't forget to clean up the snapshot object.
    CloseHandle(hThreadSnap);
    return res;
}

void printError(TCHAR const* msg)
{
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR* p;

    eNum = GetLastError();
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, eNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        sysMsg, 256, NULL);

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while ((*p > 31) || (*p == 9))
        ++p;
    do { *p-- = 0; } while ((p >= sysMsg) &&
        ((*p == '.') || (*p < 33)));

    // Display the message
    _tprintf(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg);
}