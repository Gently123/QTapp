#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "processfetcher.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateProcessTable(const QList<ProcessInfo> &list);
    void onCustomContextMenuRequested(const QPoint &pos);
    void searchTextChanged(const QString &text);

private:
    Ui::MainWindow *ui;
    ProcessFetcher *fetcher;
    QList<ProcessInfo> allProcesses;

    void setupTable();
    void showContextMenu(int row, const QPoint &globalPos);
};

#endif // MAINWINDOW_H
