#ifndef PROCESSFETCHER_H
#define PROCESSFETCHER_H

#include <QThread>
#include <QList>
#include <QIcon>
#include <QString>

struct ProcessInfo {
    QString name;
    int pid;
    QString status;
    double cpu;
    double memory;
    int threads;       // ✅ 添加线程数
    int priorityClass; // ✅ 添加优先级（原始值如 0x20）
    QIcon icon;
    int tcpConnections = 0; //添加tcp连接数量
};


class ProcessFetcher : public QThread {
    Q_OBJECT
public:
    explicit ProcessFetcher(QObject *parent = nullptr);
protected:
    void run() override;
signals:
    void processListReady(const QList<ProcessInfo> &list);
};

#endif // PROCESSFETCHER_H
