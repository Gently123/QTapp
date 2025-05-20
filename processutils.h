#ifndef PROCESSUTILS_H
#define PROCESSUTILS_H

#include <QIcon>
#include <windows.h>
#include <map>

std::map<DWORD, int> getTcpConnectionCounts();

bool terminateProcess(int pid);
void openProcessLocation(int pid);
QIcon extractIcon(const WCHAR *exePath);
double getFakeCPUUsage(int pid);

#endif // PROCESSUTILS_H
