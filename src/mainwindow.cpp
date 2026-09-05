#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "SevenzipArchiver/easyarchiver.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QFileSystemModel> // Model-View
#include <QMessageBox>
#include <QMimeDatabase>
#include <QStandardItemModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pFileModel(nullptr)
    , m_pStandardModel(nullptr)
    , m_pEasyArchiver(nullptr)
{
    ui->setupUi(this);

    // Initialization
    m_pEasyArchiver = CEasyArchiver::GetInstance("7z.dll", bit7z::BitFormat::Auto);
    m_pFileModel = new QFileSystemModel(this);
    m_qsDesktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    m_pFileModel->setRootPath(m_qsDesktopPath);

    m_pStandardModel = new QStandardItemModel(this);
    InitWidgets();
    InitConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitWidgets()
{
    setWindowTitle(tr("YH-Zip"));

    Q_ASSERT(m_pFileModel);
    if (m_pFileModel)
    {
        // TreeView
        {
            ui->treeView->setModel(m_pFileModel);
            ui->treeView->setRootIndex(m_pFileModel->index(m_pFileModel->rootPath()));
            ui->treeView->setColumnWidth(0, 180);
            ui->treeView->setColumnWidth(2, 150);
        }

        // ComboBox
        {
            QString rootPath = m_pFileModel->rootPath();
            QStringList sList = rootPath.split("/");
            QStandardItem* rootItem = m_pStandardModel->invisibleRootItem();
            if (rootItem)
            {
                QStandardItem* insertItem = rootItem;
                for(auto& it : sList)
                {
                    QStandardItem* item = new QStandardItem(it);
                    insertItem->appendRow(item);
                    insertItem = item;
                }
                QStandardItem* docItem = new QStandardItem(tr("文档"));
                QStandardItem* computerItem = new QStandardItem(tr("电脑"));
                // 添加系统所有磁盘
                computerItem->appendRow(new QStandardItem("C"));

                QStandardItem* netItem = new QStandardItem(tr("网络"));
                rootItem->appendRow(docItem);
                rootItem->appendRow(computerItem);
                rootItem->appendRow(netItem);
                QTreeView *pCbView = new QTreeView(ui->cb_path);
                pCbView->setModel(m_pStandardModel);
                pCbView->setHeaderHidden(true);
                pCbView->setRootIsDecorated(false);
                pCbView->setStyleSheet(
                    "QTreeView::branch {"
                    "   border-image: none;"
                    "   image: none;"
                    "}"
                    );

                ui->cb_path->setView(pCbView);
                ui->cb_path->setModel(m_pStandardModel);
                pCbView->expandAll();
            }

        }
    }
}


void MainWindow::InitConnect()
{
    connect(ui->tb_add, &QToolButton::clicked, this, &MainWindow::OnBtnAddClicked);
    connect(ui->tb_decompress, &QToolButton::clicked, this, &MainWindow::OnBtnExtractClicked);
    connect(ui->cb_path, &QComboBox::currentTextChanged, this, &MainWindow::OnCbCurrentTextChanged);
}

void MainWindow::OnBtnAddClicked()
{

}

void MainWindow::OnBtnExtractClicked()
{
    QModelIndexList selectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    bool bSelectedArchiver = false;
    if (!selectedIndexes.isEmpty())
    {
        bSelectedArchiver = CheckSelectFileIsArchiver(selectedIndexes.at(0));
    }

    if (!bSelectedArchiver)
    {
        QMessageBox::critical(this, "YH-Zip", tr("请选择需要解压的压缩包"));
        return;
    }

    // 弹窗

}

void MainWindow::OnCbCurrentTextChanged(const QString& sText)
{
    if (sText == "电脑")
    {
        qDebug() << "##########################";
    }
}

void MainWindow::OnLoadDirectoryFiles(const QString &sDirPath)
{

}

bool MainWindow::CheckSelectFileIsArchiver(const QModelIndex &index)
{
    QFileInfo fileInfo = m_pFileModel->fileInfo(index);
    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileInfo);
    QString mimeName = mimeType.name();
    if (mimeName.startsWith("application/") &&
        (mimeName.contains("zip") ||
         mimeName.contains("compressed") ||
         mimeName.contains("archive") ||
         mimeName == "application/x-7z-compressed" ||
         mimeName == "application/vnd.rar"))
    {
        // 这是一个压缩包
        return true;
    }

    return false;
}

