#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <windows.h>
#include <tlhelp32.h>
#include <QDebug>

void testSnapshot() {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);
    if (Process32First(snapshot, &pe)) {
        do {
            qDebug() << QString::fromWCharArray(pe.szExeFile);
        } while (Process32Next(snapshot, &pe));
    } else {
        qDebug() << "No process found";
    }
    CloseHandle(snapshot);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFile styleFile("style.qss");
    if (styleFile.open(QIODevice::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        a.setStyleSheet(style);
    }

    MainWindow w;
    w.show();
    return a.exec();
}

