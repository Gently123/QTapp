#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0600

#include "processutils.h"
#include <windows.h>
#include <psapi.h>
#include <shellapi.h>
#include <QString>
#include <QFileInfo>
#include <QPixmap>
#include <QtWin>

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




