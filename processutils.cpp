#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0600

#include "processutils.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <psapi.h>
#include <shellapi.h>
#include <QString>
#include <QFileInfo>
#include <QPixmap>
#include <QtWin>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#include <map>


bool terminateProcess(int pid) {
    HANDLE h = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (!h) return false;
    BOOL result = TerminateProcess(h, 0);
    CloseHandle(h);
    return result;
}

void openProcessLocation(int pid) {
    HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!h) return;
    WCHAR path[MAX_PATH];
    DWORD size = MAX_PATH;
    if (QueryFullProcessImageNameW(h, 0, path, &size)) {
        QString qpath = QString::fromWCharArray(path);
        QFileInfo info(qpath);
        ShellExecuteW(NULL, L"open", (LPCWSTR)info.absolutePath().utf16(), NULL, NULL, SW_SHOW);
    }
    CloseHandle(h);
}

QIcon extractIcon(const WCHAR *exePath) {
    HICON hIcon;
    ExtractIconExW(exePath, 0, NULL, &hIcon, 1);
    if (hIcon)
        return QIcon(QtWin::fromHICON(hIcon));
    return QIcon(":/default.png");
}

double getFakeCPUUsage(int /*pid*/) {
    return qrand() % 100 / 10.0;
}
// 获取每个进程拥有的 TCP 连接数
std::map<DWORD, int> getTcpConnectionCounts() {
    std::map<DWORD, int> result;

    DWORD size = 0;
    GetExtendedTcpTable(nullptr, &size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
    auto buffer = (PMIB_TCPTABLE_OWNER_PID)malloc(size);

    if (GetExtendedTcpTable(buffer, &size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == NO_ERROR) {
        for (DWORD i = 0; i < buffer->dwNumEntries; ++i) {
            DWORD pid = buffer->table[i].dwOwningPid;
            result[pid]++;
        }
    }

    free(buffer);
    return result;
}



