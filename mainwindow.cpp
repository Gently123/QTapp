#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "processutils.h"
#include <QMenu>
#include <QIcon>
#include <QLabel>  // 包含 QLabel 类的定义
#include <QMetaType>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setupTable();
    qRegisterMetaType<QList<ProcessInfo>>("QList<ProcessInfo>");

    // 搜索框连接
    connect(ui->lineEdit, &QLineEdit::textChanged,
            this, &MainWindow::searchTextChanged);

    // 右键菜单
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested,
            this, &MainWindow::onCustomContextMenuRequested);

    // 启动后台线程刷新
    fetcher = new ProcessFetcher(this);
    connect(fetcher, &ProcessFetcher::processListReady,
            this, &MainWindow::updateProcessTable);
    fetcher->start();
}

MainWindow::~MainWindow() {
    fetcher->requestInterruption();
    fetcher->wait();
    delete ui;
}
void MainWindow::setupTable() {
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels({
        "图标", "进程名", "PID", "状态", "CPU%", "内存(MB)", "线程数", "优先级"
    });

    ui->tableWidget->setColumnWidth(0, 32); // 图标列窄一点
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

}

void MainWindow::updateProcessTable(const QList<ProcessInfo> &list) {
    allProcesses = list;
    QString filter = ui->lineEdit->text();
    ui->tableWidget->setRowCount(0);

    for (const ProcessInfo &info : list) {
        if (!filter.isEmpty()) {
            if (!info.name.contains(filter, Qt::CaseInsensitive)
                && !QString::number(info.pid).contains(filter))
                continue;
        }

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        QLabel *iconLabel = new QLabel;
        iconLabel->setPixmap(info.icon.pixmap(20, 20));
        ui->tableWidget->setCellWidget(row, 0, iconLabel);
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(info.name));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(info.pid)));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(info.status));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(info.cpu, 'f', 1)));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(info.memory, 'f', 1)));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(info.threads)));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(info.priorityClass)));

    }
}
void MainWindow::onCustomContextMenuRequested(const QPoint &pos) {
    int row = ui->tableWidget->rowAt(pos.y());
    if (row >= 0)
        showContextMenu(row, ui->tableWidget->viewport()->mapToGlobal(pos));
}

void MainWindow::showContextMenu(int row, const QPoint &globalPos) {
    QMenu menu;
    QAction *killAction = menu.addAction("结束进程");
    QAction *locateAction = menu.addAction("打开文件位置");
    QAction *chosen = menu.exec(globalPos);
    if (!chosen) return;

    int pid = ui->tableWidget->item(row, 2)->text().toInt();
    if (chosen == killAction) {
        terminateProcess(pid);
        QMessageBox::information(nullptr, "提示", "进程终止成功");
    } else if (chosen == locateAction) {
        openProcessLocation(pid);
    }
}

void MainWindow::searchTextChanged(const QString &text) {
    updateProcessTable(allProcesses); // 过滤功能
}
