#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "SevenzipArchiver/easyarchiver.h"

#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitWidgets()
{

}

void MainWindow::InitConnect()
{
    connect(ui->tb_add, &QToolButton::clicked, this, &MainWindow::OnBtnAddClicked);
    connect(ui->tb_decompress, &QToolButton::clicked, this, &MainWindow::OnBtnExtractClicked);
}

void MainWindow::OnBtnAddClicked()
{

}

void MainWindow::OnBtnExtractClicked()
{
    // 实现进度条，实现解压文件的显示和完成百分比
    QString qsDesktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString sArFile = QFileDialog::getOpenFileName(this, "Open", qsDesktopPath, "*");
    QString sExtractPath = qsDesktopPath;
    if (!sArFile.isEmpty())
    {
        CEasyArchiver ar("7z.dll", bit7z::BitFormat::Zip);
        ar.Extract(sArFile.toUtf8().data(), sExtractPath.toUtf8().data());
        // TODO
    }
}
