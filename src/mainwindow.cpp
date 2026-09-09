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
    , m_pCbView(nullptr)
    , m_pPathChainRoot(nullptr)
    , m_bSyncing(false)
{
    ui->setupUi(this);

    // Initialization
    m_pEasyArchiver = CEasyArchiver::GetInstance("7z.dll", bit7z::BitFormat::Auto);
    m_pFileModel = new QFileSystemModel(this);
    m_qsDesktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    m_pFileModel->setRootPath(m_qsDesktopPath);

    m_pStandardModel = new QStandardItemModel(this);
    InitWidgets();
    InitConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitWidgets()
{
    setWindowTitle(tr("YH-Zip"));

    Q_ASSERT(m_pFileModel && m_pStandardModel);
    if (!m_pFileModel || !m_pStandardModel)
        return;

    InitFileTreeView();
    InitPathComboBox();
}

void MainWindow::InitFileTreeView()
{
    ui->treeView->setModel(m_pFileModel);
    ui->treeView->setRootIndex(m_pFileModel->index(m_pFileModel->rootPath()));
    ui->treeView->setColumnWidth(Sth::kNameColumn, Sth::kNameColumnWidth);
    ui->treeView->setColumnWidth(Sth::kTypeColumn, Sth::kTypeColumnWidth);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu); // 使用自定义菜单
}

void MainWindow::InitPathComboBox()
{
    m_pStandardModel->clear();
    QStandardItem* rootItem = m_pStandardModel->invisibleRootItem();

    // 当前路径逐级下钻，边走边拼出完整路径用于取图标
    // QStandardItem* parent = rootItem;
    // QString accumulated;
    // const QStringList parts = m_pFileModel->rootPath().split('/', Qt::SkipEmptyParts);
    // for (const QString& part : parts)
    // {
    //     accumulated += part + '/';                 // "C:/" -> "C:/Users/" -> ...

    //     QStandardItem* item = new QStandardItem(part);
    //     item->setIcon(PathIcon(accumulated));
    //     item->setData(accumulated, Qt::UserRole);
    //     parent->appendRow(item);
    //     parent = item;
    // }

    // 文档：用真实的"我的文档"路径取图标，拿到的就是系统那个带纹理的文件夹
    const QString docPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QStandardItem* docItem = new QStandardItem(tr("文档"));
    docItem->setIcon(PathIcon(docPath));
    docItem->setData(docPath, Qt::UserRole);

    QStandardItem* netItem = new QStandardItem(tr("网络"));
    netItem->setIcon(style()->standardIcon(QStyle::SP_DriveNetIcon));

    rootItem->appendRow(docItem);
    rootItem->appendRow(CreateComputerItem());
    // rootItem->appendRow(netItem);

    m_pCbView = new QTreeView;
    m_pCbView->setHeaderHidden(true);
    m_pCbView->setRootIsDecorated(false);
    m_pCbView->setIndentation(16);                   // 有图标后缩进才看得出层级
    m_pCbView->setIconSize(QSize(16, 16));
    m_pCbView->setStyleSheet(Sth::kComboViewStyle);

    ui->cb_path->setIconSize(QSize(16, 16));       // 收起状态的图标尺寸，单独设
    ui->cb_path->setModel(m_pStandardModel);
    ui->cb_path->setView(m_pCbView);
    m_pCbView->expandAll();
}

QStandardItem* MainWindow::CreateComputerItem() const
{
    QStandardItem* computerItem = new QStandardItem(tr("电脑"));
    computerItem->setData(QDir::rootPath(), Qt::UserRole);
    computerItem->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));

    for (const QFileInfo& drive : QDir::drives())
    {
        const QString path = drive.absoluteFilePath();  // "C:/"
        auto* item = new QStandardItem(QDir::toNativeSeparators(path));
        item->setIcon(m_iconProvider.icon(drive));   // 系统盘、U盘、光驱图标各不相同
        item->setData(path, Qt::UserRole);
        computerItem->appendRow(item);
    }
    return computerItem;
}


QIcon MainWindow::PathIcon(const QString& path) const
{
    const QIcon icon = m_iconProvider.icon(QFileInfo(path));
    return icon.isNull() ? style()->standardIcon(QStyle::SP_DirIcon) : icon;
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

void MainWindow::SetCurrentPath(const QString& path)
{
    if (m_bSyncing)
        return;

    const QString normalized = QDir::cleanPath(QDir(path).absolutePath());
    if (normalized.isEmpty() || !QFileInfo::exists(normalized))
        return;
    if (normalized == m_strCurrentPath)   // 幂等：路径没变就不往下传，回环在此终止
        return;

    m_bSyncing = true;
    m_strCurrentPath = normalized;

    // QFileSystemModel 是异步加载的。目录尚未展开时 index(path) 会返回无效索引，
    // 必须用 setRootPath() 的返回值 —— 它会强制模型开始填充并给出可用索引。
    ui->treeView->setRootIndex(m_pFileModel->setRootPath(normalized));

    RebuildComboPath(normalized);

    m_bSyncing = false;
}


void MainWindow::InitConnections()
{
    connect(ui->tb_add, &QToolButton::clicked, this, &MainWindow::OnBtnAddClicked);
    connect(ui->tb_decompress, &QToolButton::clicked, this, &MainWindow::OnBtnExtractClicked);
    connect(ui->treeView, &QTreeView::activated, this, &MainWindow::OnTreeActivated);

    // Qt5 里 activated 有 int / QString 两个重载，需要消歧义；Qt6 可直接取地址
    connect(ui->cb_path, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::OnComboActivated);

    connect(ui->treeView, &QWidget::customContextMenuRequested, this, &MainWindow::OnTreeContextMenu);
}

// 树 -> 组合框：双击或回车进入目录
void MainWindow::OnTreeActivated(const QModelIndex& index)
{
    if (!m_pFileModel->isDir(index))
        return;                      // 文件交给解压/预览逻辑，不改路径
    SetCurrentPath(m_pFileModel->filePath(index));
}

// 组合框 -> 树
void MainWindow::OnComboActivated(int /*row*/)
{
    // activated(int) 给的 row 是相对于当前 rootModelIndex 的，树形模型下不足以定位。
    // 直接取弹出视图的 currentIndex，它此刻已被 QComboBox 更新为被点中的项。
    const QModelIndex idx = ui->cb_path->view()->currentIndex();

    const QString path = idx.data(Qt::UserRole).toString();
    if (path.isEmpty())
        return;

    SetCurrentPath(path);
}


void MainWindow::RebuildComboPath(const QString& path)
{
    QStandardItem* rootItem = m_pStandardModel->invisibleRootItem();

    if (m_pPathChainRoot)            // 整条旧链连同子节点一起移除
        rootItem->removeRow(0);
    m_pPathChainRoot = nullptr;

    QStandardItem* parent  = rootItem;
    QStandardItem* deepest = nullptr;
    QString accumulated;

    const QStringList parts = path.split('/', Qt::SkipEmptyParts);
    for (const QString& part : parts)
    {
        accumulated += part + '/';

        auto* item = new QStandardItem(part);
        item->setIcon(PathIcon(accumulated));
        item->setData(accumulated, Qt::UserRole);

        if (parent == rootItem)
        {
            rootItem->insertRow(0, item);   // 链头固定在第 0 行
            m_pPathChainRoot = item;
        }
        else
        {
            parent->appendRow(item);
        }
        parent  = item;
        deepest = item;
    }

    if (m_pCbView)
        m_pCbView->expandAll();      // 新插入的节点默认折叠，重新展开

    SelectComboItem(deepest);        // 显示最深一级，即当前目录
}

void MainWindow::SelectComboItem(QStandardItem* item)
{
    if (!item)
        return;

    // setCurrentIndex(int) 的行号是相对 rootModelIndex 的，要选中嵌套项
    // 必须先把 root 挪到它父节点，选完再还原，否则下拉只剩那一层能看。
    // QComboBox 内部把 currentIndex 存为 QPersistentModelIndex，
    // 还原 root 不会影响已选中项的显示。
    const QModelIndex idx = item->index();
    ui->cb_path->setRootModelIndex(idx.parent());
    ui->cb_path->setCurrentIndex(idx.row());
    ui->cb_path->setRootModelIndex(QModelIndex());
}

void MainWindow::OnTreeContextMenu(const QPoint& pos)
{
    // customContextMenuRequested 给的是 viewport 坐标，indexAt 和
    // mapToGlobal 都必须走 viewport，用 treeView 本身会偏掉一个表头的高度
    const QModelIndex index = ui->treeView->indexAt(pos);

    QMenu menu(this);

    if (!index.isValid())
    {
        BuildBlankMenu(&menu);
    }
    else
    {
        // 关键：先把当前项定到右键点中的那一项。
        // 右键虽然通常会选中条目，但在已有多选、或点在已选中项上时行为不一致，
        // 而 CheckSelectFileIsArchiver() 读的是"当前选中"，不定死会判错对象。
        ui->treeView->setCurrentIndex(index);

        const QString path = m_pFileModel->filePath(index);

        if (m_pFileModel->isDir(index))
            BuildDirMenu(&menu, path);
        else if (CheckSelectFileIsArchiver(index))
            BuildArchiveMenu(&menu, path);
        else
            BuildFileMenu(&menu, path);
    }

    if (menu.isEmpty())
        return;

    menu.exec(ui->treeView->viewport()->mapToGlobal(pos));
}

void MainWindow::BuildArchiveMenu(QMenu* menu, const QString& path)
{
    const QFileInfo fi(path);
    const QString bareName = fi.completeBaseName();          // 去掉 .zip
    const QString dir      = fi.absolutePath();

    menu->addAction(tr("打开压缩包")/*, this, [this, path] { OpenArchive(path); }*/);
    menu->addSeparator();

    menu->addAction(tr("解压到当前文件夹")/*, this, [this, path, dir] {
        ExtractTo(path, dir);}*/);
    menu->addAction(tr("解压到 %1\\").arg(bareName)/*, this, [this, path, dir, bareName] {
        ExtractTo(path, dir + '/' + bareName);}*/);
    menu->addAction(tr("解压到...")/*, this, [this, path] { ExtractWithDialog(path); }*/);

    menu->addSeparator();
    menu->addAction(tr("测试压缩文件")/*, this, [this, path] { TestArchive(path); }*/);

    AppendCommonFileActions(menu, path);
}

void MainWindow::BuildFileMenu(QMenu* menu, const QString& path)
{
    const QString zipName = QFileInfo(path).completeBaseName() + ".zip";

    menu->addAction(tr("添加到压缩文件...")/*, this, [this, path] {
        CompressWithDialog(QStringList{path});}*/);
    menu->addAction(tr("添加到 %1").arg(zipName)/*, this, [this, path, zipName] {
        CompressTo(QStringList{path}, QFileInfo(path).absolutePath() + '/' + zipName);}*/);

    AppendCommonFileActions(menu, path);
}

void MainWindow::BuildDirMenu(QMenu* menu, const QString& path)
{
    menu->addAction(tr("打开"), this, [this, path] { SetCurrentPath(path); });
    menu->addSeparator();

    const QString zipName = QDir(path).dirName() + ".zip";
    menu->addAction(tr("添加到压缩文件...")/*, this, [this, path] {
        CompressWithDialog(QStringList{path});}*/);
    menu->addAction(tr("添加到 %1").arg(zipName)/*, this, [this, path, zipName] {
        CompressTo(QStringList{path}, QFileInfo(path).absolutePath() + '/' + zipName);}*/);

    AppendCommonFileActions(menu, path);
}

void MainWindow::BuildBlankMenu(QMenu* menu)
{
    menu->addAction(tr("新建文件夹")/*, this, &MainWindow::CreateFolderHere*/);
    menu->addAction(tr("刷新"), this, [this] {
        // QFileSystemModel 没有公开的 refresh()，重设 rootPath 会触发重新枚举
        const QString cur = m_strCurrentPath;
        m_strCurrentPath.clear();
        SetCurrentPath(cur);
    });
}


void MainWindow::AppendCommonFileActions(QMenu* menu, const QString& path)
{
    menu->addSeparator();
    menu->addAction(tr("复制")/*, this, [this, path] { CopyToClipboard(path); }*/);

    // 磁盘根（右键点在"电脑"层的 C:\ 上）不允许改名和删除
    const bool bMutable = !QDir(path).isRoot();

    QAction* pRename = menu->addAction(tr("重命名")/*, this, [this, path] { RenamePath(path); }*/);
    QAction* pDelete = menu->addAction(tr("删除")/*,   this, [this, path] { DeletePath(path); }*/);
    pRename->setEnabled(bMutable);
    pDelete->setEnabled(bMutable);

    menu->addSeparator();
    menu->addAction(tr("属性")/*, this, [this, path] { ShowProperties(path); }*/);
}

