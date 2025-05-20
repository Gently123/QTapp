#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0600
#include "processfetcher.h"
#include "processutils.h"
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <QDebug>


ProcessFetcher::ProcessFetcher(QObject *parent) : QThread(parent) {}

void ProcessFetcher::run() {
    qDebug() << "Fetcher thread started!";

    while (!isInterruptionRequested()) {
        QList<ProcessInfo> list;

        // 🔥 获取每个进程的 TCP 连接数映射
        auto tcpMap = getTcpConnectionCounts();

        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap == INVALID_HANDLE_VALUE) {
            qDebug() << "Failed to create process snapshot.";
            sleep(1);
            continue;
        }

        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(snap, &pe)) {
            do {
                ProcessInfo info;
                info.name = QString::fromWCharArray(pe.szExeFile);
                info.pid = pe.th32ProcessID;
                info.status = "Running";  // 默认 Running
                info.threads = pe.cntThreads;

                // 🔥 设置该进程的 TCP 连接数
                info.tcpConnections = tcpMap[info.pid];

                HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
                if (h) {
                    DWORD pri = GetPriorityClass(h);
                    info.priorityClass = (pri == 0) ? -1 : static_cast<int>(pri);

                    PROCESS_MEMORY_COUNTERS pmc;
                    if (GetProcessMemoryInfo(h, &pmc, sizeof(pmc))) {
                        info.memory = pmc.WorkingSetSize / 1024.0 / 1024.0;
                    } else {
                        info.memory = 0;
                    }

                    WCHAR fullPath[MAX_PATH];
                    DWORD len = MAX_PATH;
                    if (QueryFullProcessImageNameW(h, 0, fullPath, &len)) {
                        info.icon = extractIcon(fullPath);
                    }

                    info.cpu = getFakeCPUUsage(info.pid);
                    CloseHandle(h);
                } else {
                    info.priorityClass = -1;
                    info.memory = 0;
                    info.cpu = 0;
                    info.icon = QIcon(":/default.png");
                }

                list.append(info);
            } while (Process32Next(snap, &pe));
        }

        CloseHandle(snap);
        emit processListReady(list);
        qDebug() << "Collected processes:" << list.size();

        sleep(1);
    }
}



